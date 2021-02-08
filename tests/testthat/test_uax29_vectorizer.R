context("Test UAX29Vectorizer")

test_that("UAX29Vectorizer can be initialized", {
    v <- UAX29Vectorizer()
    testthat::expect_s3_class(v, 'UAX29Vectorizer')
})

test_that("UAX29Vectorizer is initialized to valid defaults", {
    v <- UAX29Vectorizer()

    # Base Vectorizer properties.
    testthat::expect_length(v$vocabulary, 0)
    testthat::expect_length(v$ignored_terms, 0)
    testthat::expect_equal(v$ngrams_size, 1)
    testthat::expect_equal(v$min_term_length, 1)
    testthat::expect_length(v$case_insensitive_aliases, 0)
    testthat::expect_length(v$case_sensitive_aliases, 0)
    testthat::expect_equal(v$stemming_language, "")
    testthat::expect_equal(v$casing_transformation, "")

    # UAX29 specific properties.
    testthat::expect_equal(v$word_token_categories, txtlib:::unicode_general_categories())
    testthat::expect_length(v$non_word_token_categories, 0)
    testthat::expect_equal(v$locale, "")
})

test_that("Setting vocabulary before and after init is equivalent", {
    test_vocabulary <- c('a', 'short', 'sentence')
    test_sentence <- 'A short test sentence.'

    v <- UAX29Vectorizer(vocabulary = test_vocabulary)
    v2 <- UAX29Vectorizer()

    testthat::expect_error(v2$transform(test_sentence))  # Untrained vectorizer.

    v2$set_vocabulary(test_vocabulary)
    testthat::expect_equal(v$transform(test_sentence), v2$transform(test_sentence))
})

test_that("Processing empty strings returns valid data structures", {
    test_vocabulary <- c('a', 'short', 'sentence')

    v <- UAX29Vectorizer(vocabulary = test_vocabulary)

    testthat::expect_length(v$tokenize(''), 1)
    testthat::expect_length(v$tokenize(c('', '')), 2)

    testthat::expect_equal(dim(v$transform('')), c(1, 3))
    testthat::expect_equal(dim(v$transform(c('', ''))), c(2, 3))
    testthat::expect_equal(Matrix::rowSums(v$transform(c('', ''))), c(0, 0))

    testthat::expect_length(v$tokenize(c('', NA, '')), 3)  # NA string is treated as an empty string.

    testthat::expect_equal(dim(v$transform(c('', NA))), c(2, 3))
    testthat::expect_equal(Matrix::rowSums(v$transform(c('', NA))), c(0, 0))
})

test_that("Ignored terms and term smaller than the min term length are skipped", {
    test_vocabulary <- c('A', 'short', 'test', 'sentence')
    ignored_terms <- c('test')

    test_sentence <- 'A short test sentence.'

    v <- UAX29Vectorizer(vocabulary = test_vocabulary, ignored_terms = ignored_terms, min_term_length = 1)

    testthat::expect_equal(v$tokenize(test_sentence)[[1]], c('A', ' ', 'short', ' ', ' ', 'sentence', '.'))

    v <- UAX29Vectorizer(vocabulary = test_vocabulary, min_term_length = 2)
    testthat::expect_equal(v$tokenize(test_sentence)[[1]], c('short', 'test', 'sentence'))
})

test_that("Casing transformations can be applied", {
    test_vocabulary <- c('A', 'short', 'test', 'sentence')
    test_sentence <- 'A SHORT test sentence.'

    v <- UAX29Vectorizer(
        vocabulary = test_vocabulary,
        casing_transformation = 'lower'
    )

    testthat::expect_equal(v$tokenize(test_sentence)[[1]], c('a', ' ', 'short', ' ', 'test', ' ', 'sentence', '.'))
})

test_that("Aliases are applied correctly", {
    test_vocabulary <- c('A', 'short', 'test', 'sentence')
    test_sentence <- 'A short test sentence.'

    v <- UAX29Vectorizer(
        vocabulary = test_vocabulary,
        case_insensitive_aliases = c('short' = 'long', 'a' = 'b'),
        case_sensitive_aliases = c('A' = 'The')
    )

    testthat::expect_equal(v$tokenize(test_sentence)[[1]], c('The', ' ', 'long', ' ', 'test', ' ', 'sentence', '.'))
})

test_that("Tokens can be selected or ignored using Unicode General Categories", {
    test_vocabulary <- c('A', 'short', 'test', 'sentence')
    test_sentence <- 'A short test sentence.'

    v <- UAX29Vectorizer(
        vocabulary = test_vocabulary,
        word_token_categories = c('L', 'Z'),  # Select only letters or spaces.
        non_word_token_categories = c('Z')  # Exclude tokens containing spaces.
    )

    expect_equal(v$tokenize(test_sentence)[[1]], c('A', 'short', 'test', 'sentence'))
})

test_that("Ngrams can be generated", {
    test_vocabulary <- c('A', 'short', 'test', 'sentence')
    test_sentence <- 'A short test sentence.'

    v <- UAX29Vectorizer(
        vocabulary = test_vocabulary,
        ignored_terms = c('A'),
        word_token_categories = 'L',  # Only select letter tokens.
        ngrams_size = 2
    )

    expect_equal(v$tokenize(test_sentence)[[1]], c('short', 'test', 'short_test', 'sentence', 'test_sentence'))
})

test_that("Ngrams never include tokens from different sentences", {
    test_vocabulary <- c('A', 'short', 'test', 'sentence')
    test_sentence <- 'A short sentence. Another one.'

    v <- UAX29Vectorizer(
        vocabulary = test_vocabulary,
        word_token_categories = 'L',
        ngrams_size = 3
    )

    expect_equal(v$tokenize(test_sentence)[[1]], c('A', 'short', 'A_short', 'sentence', 'short_sentence', 'A_short_sentence', 'Another', 'one', 'Another_one'))
})
