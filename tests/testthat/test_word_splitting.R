context("Test splitting words")

test_that("Multiple white spaces are kept together", {
    testthat::expect_equal(txtlib::split_words('a  b')[[1]], c('a', '  ', 'b'))
    testthat::expect_equal(txtlib::split_words('a \t b')[[1]], c('a', ' ', '\t', ' ', 'b'))
})

test_that("Format characters are handled as expected when breaking words", {
    testthat::expect_equal(txtlib::split_words('a \u200B b')[[1]], c('a', ' ', '\u200B', ' ', 'b'))
    testthat::expect_equal(txtlib::split_words('a b\u200B')[[1]], c('a', ' ', 'b', '\u200B'))
    testthat::expect_equal(txtlib::split_words('A. \u200BC. \u200B\u200B')[[1]], c('A', '.', ' ', '\u200B', 'C', '.', ' ', '\u200B', '\u200B'))
})

