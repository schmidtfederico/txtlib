context('Test splitting sentences')

test_that("Empty text returns an empty array", {
    testthat::expect_equal(length(txtlib::split_sentences('')[[1]]), 0)
})

test_that("Spaces around sentence terminators belong to the first sentence", {
    splitted <- txtlib::split_sentences('A sentence.   Another sentence.')[[1]]

    testthat::expect_equal(splitted, c('A sentence.   ', 'Another sentence.'))
})

test_that("Emails don't cause sentence splits", {
    splitted <- txtlib::split_sentences('me@somedomain.com. Not really!!')[[1]]

    testthat::expect_equal(splitted, c('me@somedomain.com. ', 'Not really!!'))
})

test_that("Numbers don't cause sentence splits", {
    splitted <- txtlib::split_sentences("It'll cost $5,000.00 dollars to repair. For real...")[[1]]

    testthat::expect_equal(splitted, c("It'll cost $5,000.00 dollars to repair. ", 'For real...'))
})

test_that("Control chars between spaces don't break the split rule", {
    testthat::expect_equal(txtlib::split_sentences('A. \u200B C')[[1]], c('A. \u200B ', 'C'))
    testthat::expect_equal(txtlib::split_sentences('A. C\u200B ')[[1]], c('A. ', 'C\u200B '))
    testthat::expect_equal(txtlib::split_sentences('A. \u200BC. \u200B\u200B')[[1]], c('A. \u200B', 'C. \u200B\u200B'))
})