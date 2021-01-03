// #include "sentence_vectorization.hpp"

#include <RcppParallel.h>
#include <string>
#include <algorithm>
#include <iterator>
#include <map>

#include "utf8.h"
#include "mutable_string_view.h"
#include "sentence_parser.h"

using namespace std;
using namespace txtlib;


#if PARSER_PROFILE
#include "iprof.hpp"
#include "gperftools/profiler.h"

void start_profiling() {
    ProfilerStart("parser.log");
}

void stop_profiling() {
    ProfilerStop();
}

#endif

// [[Rcpp::export(name = ".iprof_results")]]]
void iprof_results() {
#if PARSER_PROFILE
    InternalProfiler::aggregateEntries();
    std::cout << "\nWHAT: AVG_TIME (TOTAL_TIME / TIMES_EXECUTED)\nAll times in micro seconds\n" << InternalProfiler::stats << endl;
#endif
}


//' Splits a text into one or more sentences.
//'
//' @param text A text.
//' @return An array of sentences.
// [[Rcpp::export]]
std::vector<std::vector<std::wstring>> split_sentences(const std::vector<std::string> &texts) {
    std::vector<std::vector<std::wstring>> out_sentences(texts.size());

    for(size_t i = 0; i < texts.size(); ++i) {
        const std::string text = texts[i];

        if(text.length() == 0) continue;

        std::vector<std::wstring> sentences;

        std::wstring wide_string = txtlib::utf8_to_ws(text);

        mutable_wstring_view text_view(&wide_string[0], wide_string.length());
        txtlib::UAX29Extractor<mutable_wstring_view> parser(text_view);

        mutable_wstring_view current_token;

        size_t sentence_start = 0;

        while(parser.has_tokens()) {
            if(parser.current_token.new_sentence) {
                sentences.push_back(wide_string.substr(sentence_start, parser.current_token.start_position - sentence_start));
                sentence_start = parser.current_token.start_position;
            }
        }

        sentences.push_back(wide_string.substr(sentence_start));

        out_sentences[i] = sentences;
    }


    return out_sentences;
}

//' Splits a text into one or more words.
//'
//' @param text A text.
//' @return An array of words.
// [[Rcpp::export]]
std::vector<std::vector<std::wstring>> split_words(const std::vector<std::string> &texts, const unsigned long word_mask = 2147483647, const unsigned long non_word_mask = 0, const bool lowercase = false) {
#if PARSER_PROFILE
    IPROF_FUNC;
#endif

    std::vector<std::vector<std::wstring>> out_tokens(texts.size());

    for(size_t i = 0; i < texts.size(); ++i) {
        const std::string text = texts[i];

        if(text.length() == 0) continue;

        std::vector<std::wstring> words;

        std::wstring wide_string = txtlib::utf8_to_ws(text);

        mutable_wstring_view text_view(&wide_string[0], wide_string.length());
        txtlib::UAX29Extractor<mutable_wstring_view> parser(text_view);

        while(parser.has_tokens()) {
            if(parser.current_token.token_mask & word_mask and !(parser.current_token.token_mask & non_word_mask)) {
                mutable_wstring_view current_token = parser.current_token.token;

                if(lowercase) std::for_each(current_token.begin(), current_token.end(), txtlib::to_lowercase);

                words.push_back(wide_string.substr(parser.current_token.start_position, parser.current_token.end_position - parser.current_token.start_position));
            }
        }

        out_tokens[i] = words;
    }

    return out_tokens;
}