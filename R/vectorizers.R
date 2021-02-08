
# TODO: create a Tokenizer class too.
Vectorizer <- R6::R6Class(
    classname = "Vectorizer",
    public = list(
        vocabulary = NULL,
        ignored_terms = as.character(c()),
        casing_transformation = NA,
        case_sensitive_aliases = as.character(c()),
        case_insensitive_aliases = as.character(c()),
        ngrams_size = 1,
        min_term_length = 1,
        stemming_language = NA,
        initialize = function(vocabulary = as.character(c()),
                              ignored_terms = as.character(c()),
                              case_sensitive_aliases = as.character(c()),
                              case_insensitive_aliases = as.character(c()),
                              casing_transformation = NA,
                              ngrams_size = 1L,
                              min_term_length = 1L,
                              stemming_language = NA) {
            if(!is.character(vocabulary)) stop('vocabulary should be of type character')
            self$vocabulary <- vocabulary

            if(!is.character(ignored_terms)) stop('ignored_terms should be of type character')
            self$ignored_terms <- ignored_terms

            if(!is.character(case_sensitive_aliases)) stop('case_sensitive_aliases should be of type character')
            if(length(case_sensitive_aliases) > 0) {
                terms <- unique(names(case_sensitive_aliases))
                replacements <- unique(names(case_sensitive_aliases))
                if(length(terms) != length(replacements)) stop('case_sensitive_aliases should be a named character array with unique names')

                self$case_sensitive_aliases <- case_sensitive_aliases
            }

            if(!is.character(case_insensitive_aliases)) stop('case_insensitive_aliases should be of type character')
            if(length(case_insensitive_aliases) > 0) {
                terms <- unique(names(case_insensitive_aliases))
                replacements <- unique(names(case_insensitive_aliases))
                if(length(terms) != length(replacements)) stop('case_insensitive_aliases should be a named character array with unique names')

                self$case_insensitive_aliases <- case_insensitive_aliases
            }

            if(is.na(casing_transformation) || is.null(casing_transformation)) casing_transformation <- ''
            if(!casing_transformation %in% c('lower', '')) stop('Supported casing transformations: lower or NA')
            self$casing_transformation <- casing_transformation

            if(is.na(ngrams_size) || is.null(ngrams_size)) ngrams_size <- 1
            if(is.numeric(ngrams_size)) ngrams_size <- as.integer(ngrams_size)
            if(!is.integer(ngrams_size) || ngrams_size < 1) stop('ngrams_size should be a integer >= 1')
            self$ngrams_size <- ngrams_size[1]

            if(is.na(min_term_length) || is.null(min_term_length)) min_term_length <- 1
            self$min_term_length <- min_term_length[1]

            if(is.na(stemming_language) || is.null(stemming_language)) stemming_language <- ''
            if(!is.character(stemming_language)) stop('stemming_language should be of type character')
            if(!stemming_language %in% c('english', 'spanish', '')) stop(sprintf('Unsupported stemming language "%s"', stemming_language))
            self$stemming_language <- stemming_language[1]

            private$config_updated()
        },
        tokenize = function(X) stop('Undefined'),
        tokenize_sentences = function(X) stop('Undefined'),
        fit = function(X, y = NULL, ...) stop('Undefined'),
        fit_transform = function(X, y = NULL, ...) {
            # Override method if "transform" can be efficiently implemented immediately after fit.
            self$fit(X, y)
            self$transform(X)
        },
        transform = function(X, y = NULL, ...) stop('Undefined'),
        set_vocabulary = function(vocabulary) {
            if(!is.character(vocabulary)) stop('vocabulary should be of type character')
            self$vocabulary <- vocabulary
            private$config_updated()
        }
    ),
    private = list(
        config_updated = function() stop('Undefined')
    )
)

UAX29Vectorizer_impl <- R6::R6Class(
    classname = 'UAX29Vectorizer',
    inherit = Vectorizer,
    public = list(
        locale = NA,
        word_token_categories = NULL,
        non_word_token_categories = NULL,
        initialize = function(word_token_categories = NULL,
                              non_word_token_categories = NULL,
                              locale = NULL,
                              ...) {
            if(is.na(word_token_categories) || is.null(word_token_categories)) word_token_categories <- txtlib:::unicode_general_categories()
            if(is.na(non_word_token_categories) || is.null(non_word_token_categories)) non_word_token_categories <- as.character(c())
            if(is.na(locale) || is.null(locale)) locale <- ''

            if(!is.character(word_token_categories)) stop('word_token_categories should be of type character')
            if(!is.character(non_word_token_categories)) stop('non_word_token_categories should be of type character')
            if(!is.character(locale)) stop('locale should be of type character')



            self$word_token_categories <- private$match_categories(word_token_categories)
            self$non_word_token_categories <- private$match_categories(non_word_token_categories)
            self$locale <- locale

            super$initialize(...)
        },
        tokenize = function(X, parallel = F) {
            private$check_pointer()
            txtlib:::tokenize_impl(private$vectorizer_pointer, X, parallel = parallel)
        },
        tokenize_sentences = function(X, parallel = F) {
            private$check_pointer()
            txtlib:::sentence_tokenize_impl(private$vectorizer_pointer, X, parallel = parallel)
        },
        transform = function(X, y = NULL, parallel = F, ...) {
            private$check_pointer()
            if(length(self$vocabulary) == 0) stop('vectorizer vocabulary is empty')
            txtlib:::vectorize_impl(private$vectorizer_pointer, X, parallel = parallel)
        }
    ),
    private = list(
        vectorizer_pointer = NULL,
        config_updated = function() {
            private$build_vectorizer_pointer()
        },
        check_pointer = function() {
            if(is.null(private$vectorizer_pointer)) stop("Untrained vectorizer")

            # Check if pointer needs to be rebuilt.
            if(grepl('<pointer: .*(nil|0x0>)', capture.output(private$vectorizer_pointer)))
                private$build_vectorizer_pointer()
        },
        build_vectorizer_pointer = function() {
            private$vectorizer_pointer <- txtlib:::create_uax29_vectorizer_pointer(
                vocabulary = self$vocabulary,
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