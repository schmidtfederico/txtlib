#if !defined(ARMA_64BIT_WORD)
#define ARMA_64BIT_WORD
#endif

#ifndef _VECTORIZERS_
#define _VECTORIZERS_

#include <RcppArmadillo.h>
#include <RcppParallel.h>
#include <string>
#include <algorithm>
#include <iterator>

#include "utf8.h"
#include "mutable_string_view.h"
#include "stemming.h"
#include "parsers.h"


// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::depends(RcppParallel)]]

#include <sparsepp/spp.h>

namespace txtlib {

    typedef spp::sparse_hash_map< size_t, std::wstring > aliases_map_t;
    typedef spp::sparse_hash_map< size_t, size_t > vocabulary_map_t;

    inline aliases_map_t as_alias_map(const Rcpp::StringVector &named_list) {
        aliases_map_t out;

        if(named_list.size() == 0) return out;

        Rcpp::StringVector names = named_list.names();

        for (size_t i = 0; i < named_list.size(); ++i) {
            std::wstring term_string = utf8_to_ws(names[i]);
            mutable_wstring_view term_view(&term_string[0], term_string.size());

            std::wstring replacement_string = utf8_to_ws(named_list[i]);

            out[term_view.hash()] = replacement_string;
        }

        return out;
    }

    inline void do_replacement(mutable_wstring_view &term, const aliases_map_t &replacement_map) {
        aliases_map_t::iterator element = replacement_map.find(term.hash());

        if(element != replacement_map.end()) {
            mutable_wstring_view term_view(&element->second[0], element->second.length());
            term = term_view;
        }
    }

    // Ngrams generators forward declaration.
    class NGramView;
    template < class document_vector_t > class NGramsGenerator;
    template < class document_vector_t > class BigramsGenerator;
    template < class document_vector_t > class NoOpNGramsGenerator;

    class UAX29Vectorizer {

    public:
        typedef std::vector< std::string > document_t;
        typedef spp::sparse_hash_map< size_t, size_t > document_vector_t;

        UAX29Vectorizer(std::vector< std::string > vocabulary,
                        unsigned int ngrams_size,
                        unsigned int min_term_length,
                        uint64_t word_token_mask,
                        uint64_t non_word_token_mask,
                        std::string casing_transformation,
                        aliases_map_t case_sensitive_aliases,
                        aliases_map_t case_insensitive_aliases,
                        std::string stem_language,
                        std::string locale);

        ~UAX29Vectorizer();

        // Public properties.
        std::vector< std::string > vocabulary;
        vocabulary_map_t vocabulary_map;
        // User-configurable properties.
        unsigned int ngrams_size = 1;
        unsigned int min_term_length = 1;
        uint64_t word_token_mask = 2147483647ULL;
        uint64_t non_word_token_mask = 0ULL;
        std::string stem_language;
        std::string locale;
        aliases_map_t case_sensitive_aliases;
        aliases_map_t case_insensitive_aliases;

        // Public methods.
        std::vector< document_t > tokenize(const std::vector< std::string > &documents, bool parallel);

        std::vector< std::vector < document_t > > tokenize_sentences(const std::vector< std::string > &documents, bool parallel);

        std::vector< document_vector_t > vectorize(const std::vector< std::string > &documents, bool parallel);

        std::vector< std::vector < document_vector_t > > vectorize_sentences(const std::vector< std::string > &documents, bool parallel);

        void put_token(mutable_wstring_view &token, document_vector_t &document_vector);
        void put_token(const NGramView &token, document_vector_t &document_vector);

        void put_token(mutable_wstring_view &token, document_t &document);
        void put_token(const NGramView &token, document_t &document);

        // Token sentences.
        void put_token(mutable_wstring_view &token, std::vector< document_t > &document) { this->put_token(token, document.back()); };
        void put_token(const NGramView &token, std::vector< document_t > &document) { this->put_token(token, document.back()); };

        // Sentence vectors.
        void put_token(mutable_wstring_view &token, std::vector< document_vector_t > &document) { this->put_token(token, document.back()); };
        void put_token(const NGramView &token, std::vector< document_vector_t > &document) { this->put_token(token, document.back()); };

        void new_sentence(document_t &document) {};  // Nothing to do here, as these return types don't include sentences.
        void new_sentence(document_vector_t &document) {};
        void new_sentence(std::vector< document_t > &document);  // Token sentences.
        void new_sentence(std::vector< document_vector_t > &document);  // Sentence vectors.

        void trim_document(document_t &document) {};
        void trim_document(document_vector_t &document) {};
        void trim_document(std::vector< document_t > &document);
        void trim_document(std::vector< document_vector_t > &document);


        Rcpp::S4 as_dgCMatrix(const std::vector< document_vector_t > &doc, const Rcpp::List &dimnames);

    protected:
        // Internal attributes.
        UAX29Parser< mutable_wstring_view > *parser;
        txtlib::Stemmer* stemmer;
        std::function< void(wchar_t &code) > casing_transform_function = txtlib::preserve_case;

        // Internal methods.
        template < class return_document_t >
        std::vector< return_document_t > process_texts(const std::vector< std::string > &documents, bool parallel = false);

        template < class return_document_t >
        return_document_t parse_text(const std::string &text,
                                     txtlib::UAX29Parser< mutable_wstring_view > &parser,
                                     txtlib::Stemmer &stemmer,
                                     NGramsGenerator< return_document_t > *ngrams_generator);

        template < class document_vector_t >
        NGramsGenerator< document_vector_t >* create_ngrams_generator_pointer() {
            switch(this->ngrams_size) {
                case 1: return new NoOpNGramsGenerator< document_vector_t >(this);
                case 2: return new BigramsGenerator< document_vector_t >(this);
                default: return new NGramsGenerator< document_vector_t >(this);
            }
        }

        template <class return_document_t>
        class UAX29VectorizerWorker : public RcppParallel::Worker {
        private:
            const std::vector< std::string > &texts;
            std::vector< return_document_t > &documents;
            UAX29Vectorizer &vectorizer;

        public:
            UAX29VectorizerWorker(const std::vector< std::string > &texts,
                                  std::vector< return_document_t > &documents,
                                  UAX29Vectorizer &vectorizer) : texts(texts), documents(documents), vectorizer(vectorizer) {}


            void operator()(size_t begin, size_t end) {
                NGramsGenerator< return_document_t > *ngrams_generator = vectorizer.create_ngrams_generator_pointer< return_document_t >();
                Stemmer* stemmer = create_stemmer_pointer(vectorizer.stem_language);
                UAX29Parser< mutable_wstring_view > parser(vectorizer.locale);

                for (size_t i = begin; i < end; ++i) {
                    documents[i] = vectorizer.parse_text< return_document_t >(texts[i], parser, *stemmer, ngrams_generator);
                }

                delete stemmer;
                delete ngrams_generator;
            };
        };

    };

}



#endif
