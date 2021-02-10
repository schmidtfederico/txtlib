

UAX29Vectorizer_impl <- R6::R6Class(
    classname = 'UAX29Vectorizer',
    inherit = UAX29Tokenizer_impl,
    public = list(
        vocabulary = NULL,
        initialize = function(vocabulary = as.character(c()), ...) {
            if(!is.character(vocabulary)) stop('vocabulary should be of type character')
            self$vocabulary <- vocabulary

            super$initialize(...)
        },
        tokenize = function(X, ...) {
            super$transform(X, ...)
        },
        set_vocabulary = function(vocabulary) {
            if(!is.character(vocabulary)) stop('vocabulary should be of type character')
            self$vocabulary <- vocabulary
            private$config_updated()
        },
        transform = function(X, y = NULL, parallel = F, ...) {
            private$check_pointer()
            if(length(self$vocabulary) == 0) stop('vectorizer vocabulary is empty')
            txtlib:::vectorize_impl(private$vectorizer_pointer, X, parallel = parallel)
        }
    )
)

UAX29SentenceVectorizer_impl <- R6::R6Class(
    classname = 'UAX29SentenceVectorizer',
    inherit = UAX29Vectorizer_impl,
    public = list(
        transform = function(X, y = NULL, parallel = F, ...) {
            private$check_pointer()
            if(length(self$vocabulary) == 0) stop('vectorizer vocabulary is empty')
            txtlib:::sentence_vectorize_impl(private$vectorizer_pointer, X, parallel = parallel)
        }
    )
)

#' @title Vectorize texts using Unicode Standard Annex #29 text segmentation rules.
#' @description
#' @seealso https://unicode.org/reports/tr29/
#' @param vocabulary The vocabulary for this vectorizer (optional, can be set later using set_vocabulary method).
#' @param ignored_terms A character array of terms to ignore when processing texts.
#' @param case_sensitive_aliases A named character array that maps case sensitive terms to their replacements (e.g. c("US" = "U.S.")).
#' @param case_insensitive_aliases A named character array that maps case insensitive terms to their replacements (e.g. c("it's" = "it_is")).
#' @param casing_transformation Casing transformation function name: "lower" or NA.
#' @param ngrams_size Maximum size of generated ngrams.
#' @param min_term_length Minimum length a term should have to be considered a token.
#' @param stemming_language Language used when stemming terms. If NA, no stemming is done.
#' @param word_token_categories Only terms with at least one character belonging to these Unicode General Categories will be considered a token (defaults to all).
#' @param non_word_token_categories Terms with any of these General Categories will be skipped (empty list by default).
#' @param locale Unicode locale to use when parsing texts.
#' @export
UAX29Vectorizer <- function(vocabulary = as.character(c()),
                            ignored_terms = as.character(c()),
                            case_sensitive_aliases = as.character(c()),
                            case_insensitive_aliases = as.character(c()),
                            casing_transformation = NA,
                            ngrams_size = 1L,
                            min_term_length = 1L,
                            stemming_language = NA,
                            word_token_categories = NULL,
                            non_word_token_categories = NULL,
                            locale = NULL) {
    UAX29Vectorizer_impl$new(
        vocabulary = vocabulary,
        ignored_terms = ignored_terms,
        case_sensitive_aliases = case_sensitive_aliases,
        case_insensitive_aliases = case_insensitive_aliases,
        casing_transformation = casing_transformation,
        ngrams_size = ngrams_size,
        min_term_length = min_term_length,
        stemming_language = stemming_language,
        word_token_categories = word_token_categories,
        non_word_token_categories = non_word_token_categories,
        locale = locale
    )
}

#' @title Vectorize texts using Unicode Standard Annex #29 text segmentation rules into one or more sentence documents.
#' @description
#' @seealso https://unicode.org/reports/tr29/
#' @param vocabulary The vocabulary for this vectorizer (optional, can be set later using set_vocabulary method).
#' @param ignored_terms A character array of terms to ignore when processing texts.
#' @param case_sensitive_aliases A named character array that maps case sensitive terms to their replacements (e.g. c("US" = "U.S.")).
#' @param case_insensitive_aliases A named character array that maps case insensitive terms to their replacements (e.g. c("it's" = "it_is")).
#' @param casing_transformation Casing transformation function name: "lower" or NA.
#' @param ngrams_size Maximum size of generated ngrams.
#' @param min_term_length Minimum length a term should have to be considered a token.
#' @param stemming_language Language used when stemming terms. If NA, no stemming is done.
#' @param word_token_categories Only terms with at least one character belonging to these Unicode General Categories will be considered a token (defaults to all).
#' @param non_word_token_categories Terms with any of these General Categories will be skipped (empty list by default).
#' @param locale Unicode locale to use when parsing texts.
#' @export
UAX29SentenceVectorizer <- function(vocabulary = as.character(c()),
                                    ignored_terms = as.character(c()),
                                    case_sensitive_aliases = as.character(c()),
                                    case_insensitive_aliases = as.character(c()),
                                    casing_transformation = NA,
                                    ngrams_size = 1L,
                                    min_term_length = 1L,
                                    stemming_language = NA,
                                    word_token_categories = NULL,
                                    non_word_token_categories = NULL,
                                    locale = NULL) {
    UAX29SentenceVectorizer_impl$new(
        vocabulary = vocabulary,
        ignored_terms = ignored_terms,
        case_sensitive_aliases = case_sensitive_aliases,
        case_insensitive_aliases = case_insensitive_aliases,
        casing_transformation = casing_transformation,
        ngrams_size = ngrams_size,
        min_term_length = min_term_length,
        stemming_language = stemming_language,
        word_token_categories = word_token_categories,
        non_word_token_categories = non_word_token_categories,
        locale = locale
    )
}