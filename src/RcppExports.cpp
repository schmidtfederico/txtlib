// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <RcppArmadillo.h>
#include <Rcpp.h>

using namespace Rcpp;

// icu_info
Rcpp::List icu_info();
RcppExport SEXP _txtlib_icu_info() {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(icu_info());
    return rcpp_result_gen;
END_RCPP
}
// unicode_general_categories
const std::vector< std::string > unicode_general_categories();
RcppExport SEXP _txtlib_unicode_general_categories() {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(unicode_general_categories());
    return rcpp_result_gen;
END_RCPP
}
// create_uax29_vectorizer_pointer
SEXP create_uax29_vectorizer_pointer(std::vector < std::string > vocabulary, Rcpp::StringVector ignored_terms, std::string casing_transformation, const Rcpp::StringVector& case_sensitive_aliases, const Rcpp::StringVector& case_insensitive_aliases, size_t ngrams_size, size_t min_term_length, std::string stemming_language, std::vector< std::string > word_token_categories, std::vector< std::string > non_word_token_categories, std::string locale);
RcppExport SEXP _txtlib_create_uax29_vectorizer_pointer(SEXP vocabularySEXP, SEXP ignored_termsSEXP, SEXP casing_transformationSEXP, SEXP case_sensitive_aliasesSEXP, SEXP case_insensitive_aliasesSEXP, SEXP ngrams_sizeSEXP, SEXP min_term_lengthSEXP, SEXP stemming_languageSEXP, SEXP word_token_categoriesSEXP, SEXP non_word_token_categoriesSEXP, SEXP localeSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::vector < std::string > >::type vocabulary(vocabularySEXP);
    Rcpp::traits::input_parameter< Rcpp::StringVector >::type ignored_terms(ignored_termsSEXP);
    Rcpp::traits::input_parameter< std::string >::type casing_transformation(casing_transformationSEXP);
    Rcpp::traits::input_parameter< const Rcpp::StringVector& >::type case_sensitive_aliases(case_sensitive_aliasesSEXP);
    Rcpp::traits::input_parameter< const Rcpp::StringVector& >::type case_insensitive_aliases(case_insensitive_aliasesSEXP);
    Rcpp::traits::input_parameter< size_t >::type ngrams_size(ngrams_sizeSEXP);
    Rcpp::traits::input_parameter< size_t >::type min_term_length(min_term_lengthSEXP);
    Rcpp::traits::input_parameter< std::string >::type stemming_language(stemming_languageSEXP);
    Rcpp::traits::input_parameter< std::vector< std::string > >::type word_token_categories(word_token_categoriesSEXP);
    Rcpp::traits::input_parameter< std::vector< std::string > >::type non_word_token_categories(non_word_token_categoriesSEXP);
    Rcpp::traits::input_parameter< std::string >::type locale(localeSEXP);
    rcpp_result_gen = Rcpp::wrap(create_uax29_vectorizer_pointer(vocabulary, ignored_terms, casing_transformation, case_sensitive_aliases, case_insensitive_aliases, ngrams_size, min_term_length, stemming_language, word_token_categories, non_word_token_categories, locale));
    return rcpp_result_gen;
END_RCPP
}
// tokenize_impl
std::vector< std::vector < std::string > > tokenize_impl(SEXP vectorizer_handle, std::vector<std::string> texts, bool parallel);
RcppExport SEXP _txtlib_tokenize_impl(SEXP vectorizer_handleSEXP, SEXP textsSEXP, SEXP parallelSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type vectorizer_handle(vectorizer_handleSEXP);
    Rcpp::traits::input_parameter< std::vector<std::string> >::type texts(textsSEXP);
    Rcpp::traits::input_parameter< bool >::type parallel(parallelSEXP);
    rcpp_result_gen = Rcpp::wrap(tokenize_impl(vectorizer_handle, texts, parallel));
    return rcpp_result_gen;
END_RCPP
}
// sentence_tokenize_impl
std::vector< std::vector < std::vector < std::string > > > sentence_tokenize_impl(SEXP vectorizer_handle, std::vector<std::string> texts, bool parallel);
RcppExport SEXP _txtlib_sentence_tokenize_impl(SEXP vectorizer_handleSEXP, SEXP textsSEXP, SEXP parallelSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type vectorizer_handle(vectorizer_handleSEXP);
    Rcpp::traits::input_parameter< std::vector<std::string> >::type texts(textsSEXP);
    Rcpp::traits::input_parameter< bool >::type parallel(parallelSEXP);
    rcpp_result_gen = Rcpp::wrap(sentence_tokenize_impl(vectorizer_handle, texts, parallel));
    return rcpp_result_gen;
END_RCPP
}
// vectorize_impl
S4 vectorize_impl(SEXP vectorizer_handle, std::vector< std::string > texts, bool parallel, bool with_dimnames);
RcppExport SEXP _txtlib_vectorize_impl(SEXP vectorizer_handleSEXP, SEXP textsSEXP, SEXP parallelSEXP, SEXP with_dimnamesSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type vectorizer_handle(vectorizer_handleSEXP);
    Rcpp::traits::input_parameter< std::vector< std::string > >::type texts(textsSEXP);
    Rcpp::traits::input_parameter< bool >::type parallel(parallelSEXP);
    Rcpp::traits::input_parameter< bool >::type with_dimnames(with_dimnamesSEXP);
    rcpp_result_gen = Rcpp::wrap(vectorize_impl(vectorizer_handle, texts, parallel, with_dimnames));
    return rcpp_result_gen;
END_RCPP
}
// sentence_vectorize_impl
List sentence_vectorize_impl(SEXP vectorizer_handle, std::vector<std::string> texts, bool parallel, bool with_dimnames);
RcppExport SEXP _txtlib_sentence_vectorize_impl(SEXP vectorizer_handleSEXP, SEXP textsSEXP, SEXP parallelSEXP, SEXP with_dimnamesSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type vectorizer_handle(vectorizer_handleSEXP);
    Rcpp::traits::input_parameter< std::vector<std::string> >::type texts(textsSEXP);
    Rcpp::traits::input_parameter< bool >::type parallel(parallelSEXP);
    Rcpp::traits::input_parameter< bool >::type with_dimnames(with_dimnamesSEXP);
    rcpp_result_gen = Rcpp::wrap(sentence_vectorize_impl(vectorizer_handle, texts, parallel, with_dimnames));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_txtlib_icu_info", (DL_FUNC) &_txtlib_icu_info, 0},
    {"_txtlib_unicode_general_categories", (DL_FUNC) &_txtlib_unicode_general_categories, 0},
    {"_txtlib_create_uax29_vectorizer_pointer", (DL_FUNC) &_txtlib_create_uax29_vectorizer_pointer, 11},
    {"_txtlib_tokenize_impl", (DL_FUNC) &_txtlib_tokenize_impl, 3},
    {"_txtlib_sentence_tokenize_impl", (DL_FUNC) &_txtlib_sentence_tokenize_impl, 3},
    {"_txtlib_vectorize_impl", (DL_FUNC) &_txtlib_vectorize_impl, 4},
    {"_txtlib_sentence_vectorize_impl", (DL_FUNC) &_txtlib_sentence_vectorize_impl, 4},
    {NULL, NULL, 0}
};

RcppExport void R_init_txtlib(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
