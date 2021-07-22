context("Test splitting words")

test_that("Multiple white spaces are kept together", {
    # Only after ICU 62, which bumped Unicode version to 11, whitespaces are together.
    testthat::skip_if(txtlib:::icu_info()$VERSION_MAJOR < 62)
    tokenizer <- UAX29Tokenizer()
    testthat::expect_equal(tokenizer$transform('a  b')[[1]], c('a', '  ', 'b'))
    testthat::expect_equal(tokenizer$transform('a \t b')[[1]], c('a', ' ', '\t', ' ', 'b'))
})

test_that("Format characters are handled as expected when breaking words", {
    tokenizer <- UAX29Tokenizer()
    testthat::expect_equal(tokenizer$transform('a \u200B b')[[1]], c('a', ' ', '\u200B', ' ', 'b'))
    testthat::expect_equal(tokenizer$transform('a b\u200B')[[1]], c('a', ' ', 'b', '\u200B'))
    testthat::expect_equal(tokenizer$transform('A. \u200BC. \u200B\u200B')[[1]], c('A', '.', ' ', '\u200B', 'C', '.', ' ', '\u200B', '\u200B'))
})

