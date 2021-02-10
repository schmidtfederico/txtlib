
UAX29Tokenizer_impl <- R6::R6Class(
    classname = 'UAX29Tokenizer',
    inherit = Tokenizer,
    public = list(
        locale = NA,
        word_token_categories = NULL,
        non_word_token_categories = NULL,
        initialize = function(word_token_categories = NULL,
                              non_word_token_categories = NULL,
                              locale = NULL,
                              ...) {
            if(anyNA(word_token_categories) || is.null(word_token_categories)) word_token_categories <- txtlib:::unicode_general_categories()
            if(anyNA(non_word_token_categories) || is.null(non_word_token_categories)) non_word_token_categories <- as.character(c())
            if(anyNA(locale) || is.null(locale)) locale <- ''

            if(!is.character(word_token_categories)) stop('word_token_categories should be of type character')
            if(!is.character(non_word_token_categories)) stop('non_word_token_categories should be of type character')
            if(!is.character(locale)) stop('locale should be of type character')

            self$word_token_categories <- private$match_categories(word_token_categories)
            self$non_word_token_categories <- private$match_categories(non_word_token_categories)
            self$locale <- locale

            super$initialize(...)
        },
        transform = function(X, y = NULL, parallel = F, ...) {
            private$check_pointer()
            txtlib:::tokenize_impl(private$vectorizer_pointer, X, parallel = parallel)
        }
    ),
    private = list(
        vectorizer_pointer = NULL,
        config_updated = function() {
            private$build_vectorizer_pointer()
        },
        check_pointer = function() {
            # Check if pointer needs to be rebuilt.
            if(is.null(private$vectorizer_pointer) || grepl('<pointer: .*(nil|0x0>)', capture.output(private$vectorizer_pointer)))
                private$build_vectorizer_pointer()
        },
        build_vectorizer_pointer = function() {
            vocabulary <- as.character(c())
            if('vocabulary' %in% ls(self)) vocabulary <- self$vocabulary

            private$vectorizer_pointer <- txtlib:::create_uax29_vectorizer_pointer(
                vocabulary = vocabulary,
                ignored_terms = self$ignored_terms,
                casing_transformation = self$casing_transformation,
                case_sensitive_aliases = self$case_sensitive_aliases,
                case_insensitive_aliases = self$case_insensitive_aliases,
                ngrams_size = self$ngrams_size,
                min_term_length = self$min_term_length,
                stemming_language = self$stemming_language,
                word_token_categories = self$word_token_categories,
                non_word_token_categories = self$non_word_token_categories,
                locale = self$locale
            )
        },
        match_categories = function(categories) {
            if(length(categories) == 0) return(categories)
            matches <- lapply(categories, function(x) grep(pattern = paste0(x, '.*'), txtlib:::unicode_general_categories(), value = T))
            empty_match <- sapply(matches, length) == 0
            if(any(empty_match)) stop('Invalid general categories: ', paste0(categories[which(empty_match)], collapse = ', '))
            unique(unlist(matches))
        }
    )
)

UAX29SentenceTokenizer_impl <- R6::R6Class(
    classname = 'UAX29SentenceTokenizer',
    inherit = UAX29Tokenizer_impl,
    public = list(
        transform = function(X, y = NULL, parallel = F, ...) {
            private$check_pointer()
            txtlib:::sentence_tokenize_impl(private$vectorizer_pointer, X, parallel = parallel)
        }
    )
)

#' @title Tokenize texts using Unicode Standard Annex #29 text segmentation rules.
#' @description
#' @seealso https://unicode.org/reports/tr29/
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
UAX29Tokenizer <- function(ignored_terms = as.character(c()),
                           case_sensitive_aliases = as.character(c()),
                           case_insensitive_aliases = as.character(c()),
                           casing_transformation = NA,
                           ngrams_size = 1L,
                           min_term_length = 1L,
                           stemming_language = NA,
                           word_token_categories = NULL,
                           non_word_token_categories = NULL,
                           locale = NULL) {
    UAX29Tokenizer_impl$new(
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

#' @title Tokenize texts using Unicode Standard Annex #29 text segmentation rules into lists of tokens for each sentence.
#' @description
#' @seealso https://unicode.org/reports/tr29/
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
UAX29SentenceTokenizer <- function(ignored_terms = as.character(c()),
                                   case_sensitive_aliases = as.character(c()),
                                   case_insensitive_aliases = as.character(c()),
                                   casing_transformation = NA,
                                   ngrams_size = 1L,
                                   min_term_length = 1L,
                                   stemming_language = NA,
                                   word_token_categories = NULL,
                                   non_word_token_categories = NULL,
                                   locale = NULL) {
    UAX29SentenceTokenizer_impl$new(
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