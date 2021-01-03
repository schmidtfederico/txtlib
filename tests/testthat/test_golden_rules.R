context('Test Golden Rules (English)')

golden_rules_en <- list(
    list("Simple period to end sentence", "Hello World. My name is Jonas.", list("Hello World. ", "My name is Jonas.")),
    list("Question mark to end sentence", "What is your name? My name is Jonas.", list("What is your name? ", "My name is Jonas.")),
    list("Exclamation point to end sentence", "There it is! I found it.", list("There it is! ", "I found it.")),
    list("One letter upper case abbreviations", "My name is Jonas E. Smith.", list("My name is Jonas E. Smith.")),
    list("One letter lower case abbreviations", "Please turn to p. 55.", list("Please turn to p. 55.")),
    list("Two letter lower case abbreviations in the middle of a sentence", "Were Jane and co. at the party?", list("Were Jane and co. at the party?")),
    list("Two letter upper case abbreviations in the middle of a sentence", "They closed the deal with Pitt, Briggs & Co. at noon.", list("They closed the deal with Pitt, Briggs & Co. at noon.")),
    list("Two letter lower case abbreviations at the end of a sentence", "Let's ask Jane and co. They should know.", list("Let's ask Jane and co. ", "They should know.")),
    list("Two letter upper case abbreviations at the end of a sentence", "They closed the deal with Pitt, Briggs & Co. It closed yesterday.", list("They closed the deal with Pitt, Briggs & Co. ", "It closed yesterday.")),
    list("Two letter (prepositive) abbreviations", "I can see Mt. Fuji from here.", list("I can see Mt. Fuji from here.")),
    list("Two letter (prepositive & postpositive) abbreviations", "St. Michael's Church is on 5th st. near the light.", list("St. Michael's Church is on 5th st. near the light.")),
    list("Possesive two letter abbreviations", "That is JFK Jr.'s book.", list("That is JFK Jr.'s book.")),
    list("Multi-period abbreviations in the middle of a sentence", "I visited the U.S.A. last year.", list("I visited the U.S.A. last year.")),
    list("Multi-period abbreviations at the end of a sentence", "I live in the E.U. How about you?", list("I live in the E.U. ", "How about you?")),
    list("U.S. as sentence boundary", "I live in the U.S. How about you?", list("I live in the U.S. ", "How about you?")),
    list("U.S. as non sentence boundary with next word capitalized", "I work for the U.S. Government in Virginia.", list("I work for the U.S. Government in Virginia.")),
    list("U.S. as non sentence boundary", "I have lived in the U.S. for 20 years.", list("I have lived in the U.S. for 20 years.")),
    list("A.M. / P.M. as non sentence boundary and sentence boundary", "At 5 a.m. Mr. Smith went to the bank. He left the bank at 6 P.M. Mr. Smith then went to the store.", list("At 5 a.m. Mr. Smith went to the bank. ", "He left the bank at 6 P.M. ", "Mr. Smith then went to the store.")),
    list("Number as non sentence boundary", "She has $100.00 in her bag.", list("She has $100.00 in her bag.")),
    list("Number as sentence boundary", "She has $100.00. It is in her bag.", list("She has $100.00. ", "It is in her bag.")),
    list("Parenthetical inside sentence", "He teaches science (He previously worked for 5 years as an engineer.) at the local University.", list("He teaches science (He previously worked for 5 years as an engineer.) at the local University.")),
    list("Email addresses", "Her email is Jane.Doe@example.com. I sent her an email.", list("Her email is Jane.Doe@example.com. ", "I sent her an email.")),
    list("Web addresses", "The site is: https://www.example.50.com/new-site/awesome_content.html. Please check it out.", list("The site is: https://www.example.50.com/new-site/awesome_content.html. ", "Please check it out.")),
    list("Single quotations inside sentence", "She turned to him, 'This is great.' she said.", list("She turned to him, 'This is great.' she said.")),
    list("Double quotations inside sentence", "She turned to him, \"This is great.\" she said.", list("She turned to him, \"This is great.\" she said.")),
    list("Double quotations at the end of a sentence", "She turned to him, \"This is great.\" She held the book out to show him.", list("She turned to him, \"This is great.\" ", "She held the book out to show him.")),
    list("Double punctuation (exclamation point)", "Hello!! Long time no see.", list("Hello!! ", "Long time no see.")),
    list("Double punctuation (question mark)", "Hello?? Who is there?", list("Hello?? ", "Who is there?")),
    list("Double punctuation (exclamation point / question mark)", "Hello!? Is that you?", list("Hello!? ", "Is that you?")),
    list("Double punctuation (question mark / exclamation point)", "Hello?! Is that you?", list("Hello?! ", "Is that you?")),
    list("List (period followed by parens and no period to end item)", "1.) The first item 2.) The second item", list("1.) The first item ", "2.) The second item")),
    list("List (period followed by parens and period to end item)", "1.) The first item. 2.) The second item.", list("1.) The first item. ", "2.) The second item.")),
    list("List (parens and no period to end item)", "1) The first item 2) The second item", list("1) The first item ", "2) The second item")),
    list("List (parens and period to end item)", "1) The first item. 2) The second item.", list("1) The first item. ", "2) The second item.")),
    list("List (period to mark list and no period to end item)", "1. The first item 2. The second item", list("1. The first item ", "2. The second item")),
    list("List (period to mark list and period to end item)", "1. The first item. 2. The second item.", list("1. The first item. ", "2. The second item.")),
    list("List with bullet", "• 9. The first item • 10. The second item", list("• 9. The first item ", "• 10. The second item")),
    list("List with hypthen", "⁃9. The first item ⁃10. The second item", list("⁃9. The first item ", "⁃10. The second item")),
    list("Alphabetical list", "a. The first item b. The second item c. The third list item", list("a. The first item", "b. The second item", "c. The third list item")),
    list("Errant newline in the middle of a sentence (PDF)", "This is a sentence\ncut off in the middle because pdf.", list("This is a sentence\ncut off in the middle because pdf.")),
    list("Errant newline in the middle of a sentence", "It was a cold \nnight in the city.", list("It was a cold night in the city.")),
    list("Lower case list separated by newline", "features\ncontact manager\nevents, activities\n", list("features", "contact manager", "events, activities")),
    list("Geo Coordinates", "You can find it at N°. 1026.253.553. That is where the treasure is.", list("You can find it at N°. 1026.253.553. ", "That is where the treasure is.")),
    list("Named entities with an exclamation point", "She works at Yahoo! in the accounting department.", list("She works at Yahoo! in the accounting department.")),
    list("I as a sentence boundary and I as an abbreviation", "We make a good team, you and I. Did you see Albert I. Jones yesterday?", list("We make a good team, you and I. ", "Did you see Albert I. Jones yesterday?")),
    list("Ellipsis at end of quotation", "Thoreau argues that by simplifying one’s life, “the laws of the universe will appear less complex. . . .”", list("Thoreau argues that by simplifying one’s life, “the laws of the universe will appear less complex. . . .”")),
    list("Ellipsis with square brackets", "\"Bohr [...] used the analogy of parallel stairways [...]\" (Smith 55).", list("\"Bohr [...] used the analogy of parallel stairways [...]\" (Smith 55).")),
    list("Ellipsis as sentence boundary (standard ellipsis rules)", "If words are left off at the end of a sentence, and that is all that is omitted, indicate the omission with ellipsis marks (preceded and followed by a space) and then indicate the end of the sentence with a period . . . . Next sentence.", list("If words are left off at the end of a sentence, and that is all that is omitted, indicate the omission with ellipsis marks (preceded and followed by a space) and then indicate the end of the sentence with a period . . . . ", "Next sentence.")),
    list("Ellipsis as sentence boundary (non-standard ellipsis rules)", "I never meant that.... She left the store.", list("I never meant that.... ", "She left the store.")),
    list("Ellipsis as non sentence boundary", "I wasn’t really ... well, what I mean...see . . . what I'm saying, the thing is . . . I didn’t mean it.", list("I wasn’t really ... well, what I mean...see . . . what I'm saying, the thing is . . . I didn’t mean it.")),
    list("4-dot ellipsis", "One further habit which was somewhat weakened . . . was that of combining words into self-interpreting compounds. . . . The practice was not abandoned. . . .", list("One further habit which was somewhat weakened . . . was that of combining words into self-interpreting compounds. ", ". . . The practice was not abandoned. . . ."))
)

golden_rules_en <- lapply(golden_rules_en, function(x) data.frame(rule_name = x[[1]], rule_text = x[[2]], expected_result = I(x[3])))
golden_rules <- do.call(rbind, golden_rules_en)

test_golden_rules <- function(split_function) {
    splitted <- lapply(golden_rules$rule_text, split_function)

    sapply(1:nrow(golden_rules), function(rule_index) {
        expected <- unlist(golden_rules[rule_index, ]$expected_result)
        actual <- unlist(splitted[[rule_index]])
        length(expected) == length(actual) && all(trimws(expected) == trimws(actual))
    })
}


test_that("UAX #29 passes the Golden Rules (EN) that it should pass", {
    uax_29_expected_failures <- c(4, 5, 10, 11, 16, 18, 31:33, 35:41, 43:45, 47, 50, 51)

    expected_result <- rep(TRUE, nrow(golden_rules))
    expected_result[uax_29_expected_failures] <- FALSE

    testthat::expect_equal(
        test_golden_rules(txtlib::split_sentences),
        expected_result
    )
})
