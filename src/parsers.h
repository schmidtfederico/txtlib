#ifndef _SENTENCE_PARSER_HPP_
#define _SENTENCE_PARSER_HPP_

#define PARSER_PROFILE 0

#include <string>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <array>
#include "unicode.h"
#include <unicode/brkiter.h>
#include <unicode/unistr.h>


#if PARSER_PROFILE
#include "gperftools/profiler.h"
#include "iprof.hpp"
#endif

// [[Rcpp::plugins(cpp11)]]
using namespace icu;

namespace txtlib {

    template <typename string_t>
        class IToken {
            public:
                string_t token;
                int start_position;
                int end_position;
                unsigned int uid;
                bool new_sentence = false;
                unsigned long token_mask = 0;

                IToken(string_t tok, int start, int end): token(tok), start_position(start), end_position(end), uid(0) {};
                IToken(string_t tok, int start, int end, unsigned int id): token(tok), start_position(start), end_position(end), uid(id) {};
                IToken() {};
        };

    template <class string_t = std::wstring>
    class TokenParser {
        public:
            const string_t *str;
            IToken<string_t> current_token;

            TokenParser() {};
            TokenParser(const string_t &_str) { set_str(_str); };

            virtual ~TokenParser() {};

            void set_str(const string_t &_str) { this->str = &_str; };

            virtual bool has_tokens() = 0;
    };

    template <class string_t = std::wstring>
        class UAX29Parser : public TokenParser<string_t> {
            typedef TokenParser<string_t> super;
            const std::string locale_str;

            public:
                UAX29Parser(const std::string& locale = "") {
                    this->init_break_iterators();
                }

                UAX29Parser(const string_t &_str, const std::string& locale = "") : locale_str(locale) {
                    this->init_break_iterators();
                    this->set_str(_str);
                };

                ~UAX29Parser() {
                    delete this->word_breaks;
                    delete this->sentence_breaks;
                }

                void set_str(const string_t &_str) {
                    this->str = &_str;
                    this->input_length = this->str->length();
                    this->n_char_left = this->input_length;
                    this->current_position = 0;

                    static_assert(sizeof(std::wstring::value_type) == sizeof(UChar32), "");
                    static_assert(alignof(std::wstring::value_type) == alignof(UChar32), "");
                    this->u_str = UnicodeString::fromUTF32(reinterpret_cast<const UChar32 *>(this->str->data()), this->str->length());

                    this->word_breaks->setText(this->u_str);
                    this->sentence_breaks->setText(this->u_str);

                    this->next_word_break = this->word_breaks->next();
                    this->next_sentence_break = this->sentence_breaks->next();;

                    this->current_char = txtlib::unknown_char;
                }

            protected:
                size_t current_position = 0UL;
                size_t n_char_left = 0UL;
                size_t input_length = 0UL;
                bool new_sentence_flag = false;

                const UnicodeData* current_char;

                UErrorCode status = U_ZERO_ERROR;
                BreakIterator* word_breaks;
                BreakIterator* sentence_breaks;
                UnicodeString u_str;

                size_t next_word_break = 0UL;
                size_t next_sentence_break = 0UL;

            void init_break_iterators() {
                this->word_breaks = BreakIterator::createWordInstance(this->locale_str.c_str(), status);
                if(U_FAILURE(status)) throw std::invalid_argument("Failed to create word break iterator with locale: '" + this->locale_str + "'");

                this->sentence_breaks = BreakIterator::createSentenceInstance(this->locale_str.c_str(), status);
                if(U_FAILURE(status)) throw std::invalid_argument("Failed to create sentence break iterator with locale: '" + this->locale_str + "'");
            }

            void read_char() {
                this->current_char = find_character(this->str->data()[this->current_position]);
                this->current_position++;
            }

            bool is_sentence_break() {
                if(this->current_position < this->next_sentence_break) return false;
                this->next_sentence_break = this->sentence_breaks->next();

                return this->current_position > 0;
            }

            bool is_word_break() {
                if(this->current_position < this->next_word_break) return false;
                this->next_word_break = this->word_breaks->next();

                return this->current_position > 0;
            }

        public:
            bool has_tokens() {
                const size_t token_start_position = this->current_position;
                const bool new_sentence = this->new_sentence_flag;
                uint64_t token_mask = 0;

                this->new_sentence_flag = false;

                while(this->current_position < this->input_length) {
                    read_char();

                    token_mask = token_mask | this->current_char->General_Category;
                    this->new_sentence_flag = this->new_sentence_flag or this->is_sentence_break();
                    const bool is_word_break = this->is_word_break();

                    if(is_word_break) {
                        size_t token_size = this->current_position - token_start_position;

                        string_t t(&(*this->str)[token_start_position], token_size);
                        this->current_token = IToken< string_t >(t, token_start_position, this->current_position);

                        this->current_token.new_sentence = new_sentence;
                        this->current_token.token_mask = token_mask;

                        return true; // Word token found.
                    }
                }

                return false;
            }
        };
}

#endif
