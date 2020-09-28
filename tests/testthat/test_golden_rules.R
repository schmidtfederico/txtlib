context('Test Golden Rules (English)')

# (\d+.*$)\n+(.*$)\n=> \[(.*)\]
# test_that("($1)", {\n    testthat::expect_equal(\n        txtlib::split_sentences("$2"),\n        c($3)\n    )\n})

test_that("(1.) Simple period to end sentence", {
    testthat::expect_equal(
        txtlib::split_sentences("Hello World. My name is Jonas."),
        c("Hello World. ", "My name is Jonas.")
    )
})

test_that("(2.) Question mark to end sentence", {
    testthat::expect_equal(
        txtlib::split_sentences("What is your name? My name is Jonas."),
        c("What is your name? ", "My name is Jonas.")
    )
})

test_that("(3.) Exclamation point to end sentence", {
    testthat::expect_equal(
        txtlib::split_sentences("There it is! I found it."),
        c("There it is! ", "I found it.")
    )
})

test_that("(4.) One letter upper case abbreviations", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("My name is Jonas E. Smith."),
        c("My name is Jonas E. Smith.")
    )
})


test_that("(5.) One letter lower case abbreviations", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("Please turn to p. 55."),
        c("Please turn to p. 55.")
    )
})


test_that("(6.) Two letter lower case abbreviations in the middle of a sentence", {
    testthat::expect_equal(
        txtlib::split_sentences("Were Jane and co. at the party?"),
        c("Were Jane and co. at the party?")
    )
})


test_that("(7.) Two letter upper case abbreviations in the middle of a sentence", {
    testthat::expect_equal(
        txtlib::split_sentences("They closed the deal with Pitt, Briggs & Co. at noon."),
        c("They closed the deal with Pitt, Briggs & Co. at noon.")
    )
})

test_that("(8.) Two letter lower case abbreviations at the end of a sentence", {
    testthat::expect_equal(
        txtlib::split_sentences("Let's ask Jane and co. They should know."),
        c("Let's ask Jane and co. ", "They should know.")
    )
})


test_that("(9.) Two letter upper case abbreviations at the end of a sentence", {
    testthat::expect_equal(
        txtlib::split_sentences("They closed the deal with Pitt, Briggs & Co. It closed yesterday."),
        c("They closed the deal with Pitt, Briggs & Co. ", "It closed yesterday.")
    )
})

test_that("(10.) Two letter (prepositive) abbreviations", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("I can see Mt. Fuji from here."),
        c("I can see Mt. Fuji from here.")
    )
})

test_that("(11.) Two letter (prepositive & postpositive) abbreviations", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("St. Michael's Church is on 5th st. near the light."),
        c("St. Michael's Church is on 5th st. near the light.")
    )
})

test_that("(12.) Possesive two letter abbreviations", {
    testthat::expect_equal(
        txtlib::split_sentences("That is JFK Jr.'s book."),
        c("That is JFK Jr.'s book.")
    )
})

test_that("(13.) Multi-period abbreviations in the middle of a sentence", {
    testthat::expect_equal(
        txtlib::split_sentences("I visited the U.S.A. last year."),
        c("I visited the U.S.A. last year.")
    )
})

test_that("(14.) Multi-period abbreviations at the end of a sentence", {
    testthat::expect_equal(
        txtlib::split_sentences("I live in the E.U. How about you?"),
        c("I live in the E.U. ", "How about you?")
    )
})

test_that("(15.) U.S. as sentence boundary", {
    testthat::expect_equal(
        txtlib::split_sentences("I live in the U.S. How about you?"),
        c("I live in the U.S. ", "How about you?")
    )
})

test_that("(16.) U.S. as non sentence boundary with next word capitalized", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("I work for the U.S. Government in Virginia."),
        c("I work for the U.S. Government in Virginia.")
    )
})

test_that("(17.) U.S. as non sentence boundary", {
    testthat::expect_equal(
        txtlib::split_sentences("I have lived in the U.S. for 20 years."),
        c("I have lived in the U.S. for 20 years.")
    )
})

test_that("(18.) A.M. / P.M. as non sentence boundary and sentence boundary", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("At 5 a.m. Mr. Smith went to the bank. He left the bank at 6 P.M. Mr. Smith then went to the store."),
        c("At 5 a.m. Mr. Smith went to the bank. ", "He left the bank at 6 P.M. ", "Mr. Smith then went to the store.")
    )
})

test_that("(19.) Number as non sentence boundary", {
    testthat::expect_equal(
        txtlib::split_sentences("She has $100.00 in her bag."),
        c("She has $100.00 in her bag.")
    )
})

test_that("(20.) Number as sentence boundary", {
    testthat::expect_equal(
        txtlib::split_sentences("She has $100.00. It is in her bag."),
        c("She has $100.00. ", "It is in her bag.")
    )
})


test_that("(21.) Parenthetical inside sentence", {
    testthat::expect_equal(
        txtlib::split_sentences("He teaches science (He previously worked for 5 years as an engineer.) at the local University."),
        c("He teaches science (He previously worked for 5 years as an engineer.) at the local University.")
    )
})

test_that("(22.) Email addresses", {
    testthat::expect_equal(
        txtlib::split_sentences("Her email is Jane.Doe@example.com. I sent her an email."),
        c("Her email is Jane.Doe@example.com. ", "I sent her an email.")
    )
})

test_that("(23.) Web addresses", {
    testthat::expect_equal(
        txtlib::split_sentences("The site is: https://www.example.50.com/new-site/awesome_content.html. Please check it out."),
        c("The site is: https://www.example.50.com/new-site/awesome_content.html. ", "Please check it out.")
    )
})

test_that("(24.) Single quotations inside sentence", {
    testthat::expect_equal(
        txtlib::split_sentences("She turned to him, 'This is great.' she said."),
        c("She turned to him, 'This is great.' she said.")
    )
})

test_that("(25.) Double quotations inside sentence", {
    testthat::expect_equal(
        txtlib::split_sentences("She turned to him, \"This is great.\" she said."),
        c("She turned to him, \"This is great.\" she said.")
    )
})

test_that("(26.) Double quotations at the end of a sentence", {
    testthat::expect_equal(
        txtlib::split_sentences("She turned to him, \"This is great.\" She held the book out to show him."),
        c("She turned to him, \"This is great.\" ", "She held the book out to show him.")
    )
})

test_that("(27.) Double punctuation (exclamation point)", {
    testthat::expect_equal(
        txtlib::split_sentences("Hello!! Long time no see."),
        c("Hello!! ", "Long time no see.")
    )
})

test_that("(28.) Double punctuation (question mark)", {
    testthat::expect_equal(
        txtlib::split_sentences("Hello?? Who is there?"),
        c("Hello?? ", "Who is there?")
    )
})

test_that("(29.) Double punctuation (exclamation point / question mark)", {
    testthat::expect_equal(
        txtlib::split_sentences("Hello!? Is that you?"),
        c("Hello!? ", "Is that you?")
    )
})


test_that("(30.) Double punctuation (question mark / exclamation point)", {
    testthat::expect_equal(
        txtlib::split_sentences("Hello?! Is that you?"),
        c("Hello?! ", "Is that you?")
    )
})


test_that("(31.) List (period followed by parens and no period to end item)", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("1.) The first item 2.) The second item"),
        c("1.) The first item ", "2.) The second item")
    )
})

test_that("(32.) List (period followed by parens and period to end item)", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("1.) The first item. 2.) The second item."),
        c("1.) The first item. ", "2.) The second item.")
    )
})

test_that("(33.) List (parens and no period to end item)", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("1) The first item 2) The second item"),
        c("1) The first item ", "2) The second item")
    )
})

test_that("(34.) List (parens and period to end item)", {
    testthat::expect_equal(
        txtlib::split_sentences("1) The first item. 2) The second item."),
        c("1) The first item. ", "2) The second item.")
    )
})

test_that("(35.) List (period to mark list and no period to end item)", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("1. The first item 2. The second item"),
        c("1. The first item ", "2. The second item")
    )
})

test_that("(36.) List (period to mark list and period to end item)", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("1. The first item. 2. The second item."),
        c("1. The first item. ", "2. The second item.")
    )
})

test_that("(37.) List with bullet", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("• 9. The first item • 10. The second item"),
        c("• 9. The first item ", "• 10. The second item")
    )
})

test_that("(38.) List with hypthen", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("⁃9. The first item ⁃10. The second item"),
        c("⁃9. The first item ", "⁃10. The second item")
    )
})

test_that("(39.) Alphabetical list", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("a. The first item b. The second item c. The third list item"),
        c("a. The first item", "b. The second item", "c. The third list item")
    )
})

test_that("(40.) Errant newline in the middle of a sentence (PDF)", {
    testthat::skip("Cleanup not performed")

    testthat::expect_equal(
        txtlib::split_sentences("This is a sentence\ncut off in the middle because pdf."),
        c("This is a sentence\ncut off in the middle because pdf.")
    )
})

test_that("(41.) Errant newline in the middle of a sentence", {
    testthat::skip("Cleanup not performed")

    testthat::expect_equal(
        txtlib::split_sentences("It was a cold \nnight in the city."),
        c("It was a cold night in the city.")
    )
})

test_that("(42.) Lower case list separated by newline", {
    testthat::expect_equal(
        trimws(txtlib::split_sentences("features\ncontact manager\nevents, activities\n")),
        c("features", "contact manager", "events, activities")
    )
})

test_that("(43.) Geo Coordinates", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("You can find it at N°. 1026.253.553. That is where the treasure is."),
        c("You can find it at N°. 1026.253.553. ", "That is where the treasure is.")
    )
})

test_that("(44.) Named entities with an exclamation point", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("She works at Yahoo! in the accounting department."),
        c("She works at Yahoo! in the accounting department.")
    )
})

test_that("(45.) I as a sentence boundary and I as an abbreviation", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("We make a good team, you and I. Did you see Albert I. Jones yesterday?"),
        c("We make a good team, you and I. ", "Did you see Albert I. Jones yesterday?")
    )
})

test_that("(46.) Ellipsis at end of quotation", {
    testthat::expect_equal(
        txtlib::split_sentences("Thoreau argues that by simplifying one’s life, “the laws of the universe will appear less complex. . . .”"),
        c("Thoreau argues that by simplifying one’s life, “the laws of the universe will appear less complex. . . .”")
    )
})

test_that("(47.) Ellipsis with square brackets", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("\"Bohr [...] used the analogy of parallel stairways [...]\" (Smith 55)."),
        c("\"Bohr [...] used the analogy of parallel stairways [...]\" (Smith 55).")
    )
})

test_that("(48.) Ellipsis as sentence boundary (standard ellipsis rules)", {
    testthat::expect_equal(
        txtlib::split_sentences("If words are left off at the end of a sentence, and that is all that is omitted, indicate the omission with ellipsis marks (preceded and followed by a space) and then indicate the end of the sentence with a period . . . . Next sentence."),
        c("If words are left off at the end of a sentence, and that is all that is omitted, indicate the omission with ellipsis marks (preceded and followed by a space) and then indicate the end of the sentence with a period . . . . ", "Next sentence.")
    )
})

test_that("(49.) Ellipsis as sentence boundary (non-standard ellipsis rules)", {
    testthat::expect_equal(
        txtlib::split_sentences("I never meant that.... She left the store."),
        c("I never meant that.... ", "She left the store.")
    )
})


test_that("(50.) Ellipsis as non sentence boundary", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("I wasn’t really ... well, what I mean...see . . . what I'm saying, the thing is . . . I didn’t mean it."),
        c("I wasn’t really ... well, what I mean...see . . . what I'm saying, the thing is . . . I didn’t mean it.")
    )
})

test_that("(51.) 4-dot ellipsis", {
    testthat::skip("UAX #29 limitation")

    testthat::expect_equal(
        txtlib::split_sentences("One further habit which was somewhat weakened . . . was that of combining words into self-interpreting compounds. . . . The practice was not abandoned. . . ."),
        c("One further habit which was somewhat weakened . . . was that of combining words into self-interpreting compounds. ", ". . . The practice was not abandoned. . . .")
    )
})
