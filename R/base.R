
Transformer <- R6::R6Class(
    classname = "Transformer",
    public = list(
        fit = function(X, y = NULL, ...) stop('Undefined'),
        fit_transform = function(X, y = NULL, ...) {
            # Override method if "transform" can be efficiently implemented immediately after fit.
            self$fit(X, y)
            self$transform(X)
        },
        transform = function(X, y = NULL, ...) stop('Undefined')
    )
)



Tokenizer <- R6::R6Class(
    classname = 'Tokenizer',
    inherit = Transformer,
    public = list(
        ignored_terms = as.character(c()),
        casing_transformation = NA,
        case_sensitive_aliases = as.character(c()),
        case_insensitive_aliases = as.character(c()),
        ngrams_size = 1,
        min_term_length = 1,
        stemming_language = NA,
        initialize = function(ignored_terms = as.character(c()),
                              case_sensitive_aliases = as.character(c()),
                              case_insensitive_aliases = as.character(c()),
                              casing_transformation = NA,
                              ngrams_size = 1L,
                              min_term_length = 1L,
                              stemming_language = NA) {
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

            if(anyNA(casing_transformation) || is.null(casing_transformation)) casing_transformation <- ''
            if(!casing_transformation %in% c('lower', '')) stop('Supported casing_transformation values are: lower or NA')
            self$casing_transformation <- casing_transformation

            if(anyNA(ngrams_size) || is.null(ngrams_size)) ngrams_size <- 1
            if(is.numeric(ngrams_size)) ngrams_size <- as.integer(ngrams_size)
            if(!is.integer(ngrams_size) || ngrams_size < 1) stop('ngrams_size should be a integer >= 1')
            self$ngrams_size <- ngrams_size[1]

            if(anyNA(min_term_length) || is.null(min_term_length)) min_term_length <- 1
            self$min_term_length <- min_term_length[1]

            if(anyNA(stemming_language) || is.null(stemming_language)) stemming_language <- ''
            if(!is.character(stemming_language)) stop('stemming_language should be of type character')
            if(!stemming_language %in% c('english', 'spanish', '')) stop(sprintf('Unsupported stemming language "%s"', stemming_language))
            self$stemming_language <- stemming_language[1]

            private$config_updated()
        }
    ),
    private = list(
        config_updated = function() stop('Undefined')
    )
)


Vectorizer <- R6::R6Class(
    classname = "Vectorizer",
    inherit = Tokenizer,
    public = list(
        vocabulary = NULL,
        initialize = function(vocabulary = as.character(c()), ...) {
            if(!is.character(vocabulary)) stop('vocabulary should be of type character')
            self$vocabulary <- vocabulary

            super$initialize(...)
        },
        tokenize = function(X) stop('Undefined'),
        set_vocabulary = function(vocabulary) {
            if(!is.character(vocabulary)) stop('vocabulary should be of type character')
            self$vocabulary <- vocabulary
            private$config_updated()
        }
    )
)