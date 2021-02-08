// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::depends(RcppArmadillo)]]
// [[Rcpp::depends(BH)]]

#include "vectorizers.h"
#include "ngrams.h"

#include <RcppParallel.h>
#include <boost/algorithm/string.hpp>
#include <boost/functional/hash.hpp>

using namespace std;
using namespace txtlib;
using namespace Rcpp;

namespace txtlib {


// Vectorizer constructor.
UAX29Vectorizer::UAX29Vectorizer(std::vector< std::string > vocabulary,
                                 unsigned int ngrams_size,
                                 unsigned int min_term_length,
                                 uint64_t word_token_mask,
                                 uint64_t non_word_token_mask,
                                 std::string casing_transformation,
                                 aliases_map_t case_sensitive_aliases,
                                 aliases_map_t case_insensitive_aliases,
                                 std::string stem_language,
                                 std::string locale) {
    this->vocabulary = vocabulary;
    this->ngrams_size = ngrams_size;
    this->min_term_length = min_term_length;
    this->word_token_mask = word_token_mask;
    this->non_word_token_mask = non_word_token_mask;
    this->case_sensitive_aliases = case_sensitive_aliases;
    this->case_insensitive_aliases = case_insensitive_aliases;
    this->stem_language = stem_language;
    this->locale = locale;

    // Populate vocabulary map.
    for(size_t idx = 0; idx < vocabulary.size(); ++idx) {
        std::wstring term_string = utf8_to_ws(vocabulary[idx]);

        std::vector< std::wstring > tokens;
        boost::split(tokens, term_string, boost::is_any_of(L"_"));

        if(tokens.size() > 1) {
            // Calculate the term hash as the txtlib::NGramView class does.
            size_t term_hash = 0;

            for(std::wstring &token : tokens) {
                mutable_wstring_view term_view(&token[0], token.length());
                term_hash ^= term_view.hash() + 0x9e3779b9 + (term_hash << 6) + (term_hash >> 2);
            }

            // std::wcout << "term: \"" << term_string << "\", hash: " << term_hash << std::endl;
            this->vocabulary_map[term_hash] = idx;
        } else {
            mutable_wstring_view term_view(&term_string[0], term_string.length());
            // std::wcout << "term: \"" << term_view << "\", hash: " << term_view.hash() << std::endl;
            this->vocabulary_map[term_view.hash()] = idx;
        }
    }

    if(casing_transformation.size() > 0) {
        if(casing_transformation == "lower") {
            this->casing_transform_function = txtlib::to_lowercase;
        } else {
            throw std::invalid_argument("invalid casing transformation: " + casing_transformation);
        }
    }

    this->stemmer = create_stemmer_pointer(this->stem_language);
    this->parser = new UAX29Parser< mutable_wstring_view >(this->locale);
}

UAX29Vectorizer::~UAX29Vectorizer() {
    delete this->stemmer;
    delete this->parser;
}

void UAX29Vectorizer::put_token(mutable_wstring_view &token, document_vector_t &document_vector) {
    vocabulary_map_t::const_iterator map_item = this->vocabulary_map.find(token.hash());
    if(map_item != this->vocabulary_map.end()) document_vector[map_item->second]++;
}

void UAX29Vectorizer::put_token(const NGramView &token, document_vector_t &document_vector) {
    vocabulary_map_t::const_iterator map_item = this->vocabulary_map.find(token.hash());
    if(map_item != this->vocabulary_map.end()) document_vector[map_item->second]++;
}

void UAX29Vectorizer::put_token(mutable_wstring_view &token, document_t &document) {
    document.push_back(ws_to_utf8(token));
}

void UAX29Vectorizer::put_token(const NGramView &token, document_t &document) {
    document.push_back(ws_to_utf8(token.as_string()));
}

void UAX29Vectorizer::new_sentence(std::vector< UAX29Vectorizer::document_t > &document) {
    UAX29Vectorizer::document_t s;
    document.push_back(s);
};

void UAX29Vectorizer::new_sentence(std::vector< UAX29Vectorizer::document_vector_t > &document) {
    UAX29Vectorizer::document_vector_t s;
    document.push_back(s);
};

void UAX29Vectorizer::trim_document(std::vector< UAX29Vectorizer::document_t > &document) {
    // while(document.back().empty()) document.pop_back();
};

void UAX29Vectorizer::trim_document(std::vector< UAX29Vectorizer::document_vector_t > &document) {
    // while(document.back().empty()) document.pop_back();
};

std::vector< UAX29Vectorizer::document_t > UAX29Vectorizer::tokenize(const std::vector< std::string > &documents, bool parallel) {
    return this->process_texts< UAX29Vectorizer::document_t >(documents, parallel);
}

std::vector< std::vector < UAX29Vectorizer::document_t > > UAX29Vectorizer::tokenize_sentences(const std::vector< std::string > &documents, bool parallel) {
    return this->process_texts< std::vector < UAX29Vectorizer::document_t > >(documents, parallel);
}

std::vector< UAX29Vectorizer::document_vector_t > UAX29Vectorizer::vectorize(const std::vector< std::string > &documents, bool parallel) {
    return this->process_texts< UAX29Vectorizer::document_vector_t >(documents, parallel);
}

std::vector< std::vector < UAX29Vectorizer::document_vector_t > > UAX29Vectorizer::vectorize_sentences(const std::vector< std::string > &documents, bool parallel) {
    return this->process_texts< std::vector < UAX29Vectorizer::document_vector_t > >(documents, parallel);
}

template < class return_document_t >
std::vector< return_document_t > UAX29Vectorizer::process_texts(const std::vector< std::string > &documents, bool parallel) {
    std::vector< return_document_t > vectors(documents.size());


#if RCPP_PARALLEL_USE_TBB
    if(!parallel) {
#endif

    NGramsGenerator< return_document_t > *ngrams_generator = this->create_ngrams_generator_pointer< return_document_t >();

    for(size_t idx = 0; idx < documents.size(); ++idx) {
        vectors[idx] = this->parse_text< return_document_t >(documents[idx], *this->parser, *this->stemmer, ngrams_generator);
    }

    delete ngrams_generator;

#if RCPP_PARALLEL_USE_TBB
    } else {
        UAX29Vectorizer::UAX29VectorizerWorker< return_document_t > w(documents, vectors, *this);
        parallelFor(0, documents.size(), w, 100L);
    }
#endif

    return(vectors);
}


template < class return_document_t >
return_document_t UAX29Vectorizer::parse_text(const std::string &text,
                                              txtlib::UAX29Parser< mutable_wstring_view > &parser,
                                              txtlib::Stemmer &stemmer,
                                              NGramsGenerator< return_document_t > *ngrams_generator) {
    return_document_t doc;

    if(text.empty()) return doc;

    ngrams_generator->reset();

    std::wstring wide_string = utf8_to_ws(text);

    mutable_wstring_view text_view(&wide_string[0], wide_string.length());
    parser.set_str(text_view);

    size_t token_initial_length;
    mutable_wstring_view current_token;

    this->new_sentence(doc);

    while(parser.has_tokens()) {
        if(parser.current_token.new_sentence) {
            this->new_sentence(doc);
            ngrams_generator->reset();
        }

        current_token = parser.current_token.token;

        if(current_token.empty() or !(parser.current_token.token_mask & this->word_token_mask) or parser.current_token.token_mask & this->non_word_token_mask) continue;

        do_replacement(current_token, this->case_sensitive_aliases);

        token_initial_length = current_token.length();

        std::for_each(current_token.begin(), current_token.end(), this->casing_transform_function);

        stemmer.stem(current_token);

        do_replacement(current_token, this->case_insensitive_aliases);

        if(current_token.length() == 0) continue;

        if(token_initial_length >= min_term_length) this->put_token(current_token, doc);

        ngrams_generator->create_ngrams(current_token, doc);
    }

    // this->trim_document(doc);

    return(doc);
}

}


// [[Rcpp::export]]
const std::vector< std::string > unicode_general_categories() {
    std::vector< std::string > categories;

    for(const auto& item : txtlib::GeneralCategoryValues) {
        categories.push_back(item.first);
    }
    return categories;
}

// [[Rcpp::export]]
SEXP create_uax29_vectorizer_pointer(std::vector < std::string > vocabulary,
                                     Rcpp::StringVector ignored_terms,
                                     std::string casing_transformation,
                                     const Rcpp::StringVector &case_sensitive_aliases,
                                     const Rcpp::StringVector &case_insensitive_aliases,
                                     size_t ngrams_size,
                                     size_t min_term_length,
                                     std::string stemming_language,
                                     std::vector< std::string > word_token_categories,
                                     std::vector< std::string > non_word_token_categories,
                                     std::string locale) {
    aliases_map_t case_sensitive_aliases_map = as_alias_map(case_sensitive_aliases);
    aliases_map_t case_insensitive_aliases_map = as_alias_map(case_insensitive_aliases);

    uint64_t word_token_mask = 0UL, non_word_token_mask = 0UL;

    for(const std::string& c : word_token_categories) {
        auto e = GeneralCategoryValues.find(c);

        if(e == GeneralCategoryValues.end()) Rcpp::stop("Unknown general category '" + c  + "'");
        word_token_mask |= e->second;
    }

    for(const std::string& c : non_word_token_categories) {
        auto e = GeneralCategoryValues.find(c);

        if(e == GeneralCategoryValues.end()) Rcpp::stop("Unknown general category '" + c  + "'");
        non_word_token_mask |= e->second;
    }

    if(ignored_terms.size() > 0) {
        Stemmer* s = create_stemmer_pointer(stemming_language);

        for(size_t i = 0; i < ignored_terms.size(); i++) {
            std::wstring ignored_term = utf8_to_ws(ignored_terms[i]);
            mutable_wstring_view excluded_term_view(&ignored_term[0], ignored_term.size());

            s->stem(excluded_term_view);
            case_insensitive_aliases_map[excluded_term_view.hash()] = L"";
        }

        delete s;
    }

    UAX29Vectorizer *vectorizer = new UAX29Vectorizer(
        vocabulary,
        ngrams_size,
        min_term_length,
        word_token_mask,
        non_word_token_mask,
        casing_transformation,
        case_sensitive_aliases_map,
        case_insensitive_aliases_map,
        stemming_language,
        locale
    );

    return XPtr< txtlib::UAX29Vectorizer >(vectorizer, true);
}


S4 UAX29Vectorizer::as_dgCMatrix(const std::vector< document_vector_t > &document_vectors, const List &dimnames) {
    // dgCMatrix has three properties: i (row index), p (column pointer) and x (matrix values).
    // This method borrows from Armadillo's bulk insert method init_batch_std (https://gitlab.com/conradsnicta/armadillo-code/-/blob/10.2.x/include/armadillo_bits/SpMat_meat.hpp#L5200).
    size_t n_elem = 0;

    for(const document_vector_t &d : document_vectors) n_elem += d.size();

    using namespace arma;

    Mat< uword > tuples(3, n_elem);
    uvec elem_weight(n_elem);

    size_t elem_idx = 0;
    const size_t doc_count = document_vectors.size();
    const size_t vocabulary_size = this->vocabulary.size();

    for(size_t row_number = 0; row_number < doc_count; row_number++) {
        for(const auto& elem : document_vectors[row_number]) {
            // elem.first is the term's index in the vocabulary.
            // elem.second is the count of a term in the document.
            elem_weight[elem_idx] = elem.first * doc_count + row_number;

            tuples(0, elem_idx) = row_number;
            tuples(1, elem_idx) = elem.first;
            tuples(2, elem_idx) = elem.second;

            ++elem_idx;
        }
    }

    IntegerVector i(n_elem);
    IntegerVector p(vocabulary_size + 1);
    NumericVector x(n_elem);

    uvec elements_order = sort_index(elem_weight);

    for (arma::uword elem_idx = 0; elem_idx < n_elem; ++elem_idx) {
        const arma::uword* tuple = tuples.colptr(elements_order[elem_idx]);

        i[elem_idx] = tuple[0];
        p[tuple[1] + 1]++;
        x[elem_idx] = tuple[2];
    }

    // Cumsum columns counts
    std::partial_sum(p.begin(), p.end(), p.begin());

    S4 mat("dgCMatrix");
    mat.slot("i") = i;
    mat.slot("p") = p;
    mat.slot("x") = x;
    mat.slot("Dim") = IntegerVector::create(doc_count, vocabulary_size);
    mat.slot("Dimnames") = dimnames;

    return(mat);
}


// [[Rcpp::export]]
std::vector< std::vector < std::string > > tokenize_impl(SEXP vectorizer_handle, std::vector<std::string> texts, bool parallel = false) {
    if(!vectorizer_handle) Rcpp::stop("Null pointer");

    XPtr< txtlib::UAX29Vectorizer > vectorizer(vectorizer_handle);

    return vectorizer->tokenize(texts, parallel);
}

// [[Rcpp::export]]
std::vector< std::vector < std::vector < std::string > > > sentence_tokenize_impl(SEXP vectorizer_handle, std::vector<std::string> texts, bool parallel = false) {
    if(!vectorizer_handle) Rcpp::stop("Null pointer");

    XPtr< txtlib::UAX29Vectorizer > vectorizer(vectorizer_handle);

    return vectorizer->tokenize_sentences(texts, parallel);
}


// [[Rcpp::export]]
S4 vectorize_impl(SEXP vectorizer_handle, std::vector< std::string > texts, bool parallel = false, bool with_dimnames = true) {
    if(!vectorizer_handle) Rcpp::stop("Null pointer");

    XPtr< txtlib::UAX29Vectorizer > vectorizer(vectorizer_handle);

    auto docs = vectorizer->vectorize(texts, parallel);

    List dimnames;

    if(with_dimnames) {
        dimnames = List::create(R_NilValue, Rcpp::wrap(vectorizer->vocabulary));
    } else {
        dimnames = List::create(R_NilValue, R_NilValue);
    }

    return vectorizer->as_dgCMatrix(docs, dimnames);
}

// [[Rcpp::export]]
List sentence_vectorize_impl(SEXP vectorizer_handle, std::vector<std::string> texts, bool parallel = false, bool with_dimnames = true) {
    if(!vectorizer_handle) Rcpp::stop("Null pointer");

    XPtr< txtlib::UAX29Vectorizer > vectorizer(vectorizer_handle);

    List output_list(texts.size());

    auto docs = vectorizer->vectorize_sentences(texts, parallel);

    List dimnames;

    if(with_dimnames) {
        dimnames = List::create(R_NilValue, Rcpp::wrap(vectorizer->vocabulary));
    } else {
        dimnames = List::create(R_NilValue, R_NilValue);
    }

    for(size_t i = 0; i < docs.size(); ++i) {
        output_list[i] = vectorizer->as_dgCMatrix(docs[i], dimnames);
    }

    return output_list;
}


