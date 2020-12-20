context('Test splitting sentences')


test_that("Empty text returns an empty array", {
    testthat::expect_equal(length(txtlib::split_sentences('')), 0)
})

test_that("Spaces around sentence terminators belong to the first sentence", {
    splitted <- txtlib::split_sentences('A sentence.   Another sentence.')

    testthat::expect_equal(splitted, c('A sentence.   ', 'Another sentence.'))
})

test_that("Emails don't cause sentence splits", {
    splitted <- txtlib::split_sentences('me@somedomain.com. Not really!!')

    testthat::expect_equal(splitted, c('me@somedomain.com. ', 'Not really!!'))
})

test_that("Numbers don't cause sentence splits", {
    splitted <- txtlib::split_sentences("It'll cost $5,000.00 dollars to repair. For real...")

    testthat::expect_equal(splitted, c("It'll cost $5,000.00 dollars to repair. ", 'For real...'))
})

test_that("Failing test", {
    testthat::fail()
})