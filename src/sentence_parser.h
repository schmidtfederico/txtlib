#ifndef _SENTENCE_PARSER_HPP_
#define _SENTENCE_PARSER_HPP_

#define PARSER_DEBUG 0
#define PARSER_PROFILE 0

#include <string>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <array>
#include "unicode.h"


#if PARSER_PROFILE
#include "gperftools/profiler.h"
#include "iprof.hpp"
#endif

// [[Rcpp::plugins(cpp11)]]

namespace txtlib {

    template <typename StringType>
        class IToken {
            public:
                StringType token;
                int start_position;
                int end_position;
                unsigned int uid;
                bool new_sentence = false;
                unsigned long token_mask = 0;

                IToken(StringType tok, int start, int end): token(tok), start_position(start), end_position(end), uid(0) {};
                IToken(StringType tok, int start, int end, unsigned int id): token(tok), start_position(start), end_position(end), uid(id) {};
                IToken() {};
        };

    template <class StringType = std::wstring>
        class UAX29Extractor {

            public:
                const StringType str;
                IToken<StringType> current_token;

                UAX29Extractor(const StringType &_str) : str(_str) {
                    // Read the first characters (if any).
                    this->init_read();
                }

                bool has_tokens() {
                    this->previous_token = this->current_token;
                    return this->get_token();
                }

            protected:
                IToken<StringType> previous_token;
                IToken<StringType> next_token;

                size_t current_position;
                size_t n_char_left;
                size_t input_length;

                const UnicodeData* previous_previous_char;
                const UnicodeData* previous_char;
                const UnicodeData* current_char;
                const UnicodeData* next_char;
                const UnicodeData* next_next_char;

                bool new_sentence_flag = false;

                // This array is used to store the last 4 distinct Unicode Sentence Break character classes.
                std::array<const UnicodeData*, 4> SB_classes;

            void init_read() {
                this->input_length = this->str.length();
                this->n_char_left = this->input_length;
                this->current_position = 0;

                this->previous_char = txtlib::unknown_char;
                this->current_char = txtlib::unknown_char;
                this->SB_classes = { {unknown_char, unknown_char, unknown_char, unknown_char} };

                switch(this->input_length) {
                    case 0:
                        break;
                    case 1:
                        this->next_char = find_character(this->str[0]); break;
                    default:
                        this->next_char = find_character(this->str[0]);
                        this->next_next_char = find_character(this->str[1]);
                        break;
                }
            }

            void push_character_class(const UnicodeData* new_class) {
#if PARSER_PROFILE
                IPROF_FUNC;
#endif

                // This implements the Close* and Sp* part of SB8-SB11.
                if(!(new_class->Close or new_class->Sp or new_class->Extend or new_class->Format) or
                       (new_class->Sp and !this->SB_classes[0]->Sp) or
                       (new_class->Close and !this->SB_classes[0]->Close)) {
                    this->SB_classes[3] = this->SB_classes[2];
                    this->SB_classes[2] = this->SB_classes[1];
                    this->SB_classes[1] = this->SB_classes[0];
                    this->SB_classes[0] = new_class;
                }
            }

            void read_char() {
#if PARSER_PROFILE
                IPROF_FUNC;
#endif

                this->n_char_left--;

                if(!(this->current_char->Extend or this->current_char->Format)) {
                    // Only replace previous chars if the new current character is not (Extend|Format).
                    this->previous_previous_char = this->previous_char;
                    this->previous_char = this->current_char;
                }
                this->current_char = this->next_char;
                this->next_char = this->next_next_char;

                this->push_character_class(this->current_char);

#if PARSER_DEBUG
                std::cout << std::endl;
                std::cout << this->current_position << std::flush;
                std::wcout << L": (code: " << (size_t) this->str[this->current_position] << L")" << std::endl;
                debug_unicode_data(this->current_char->General_Category);
#endif

                if(this->n_char_left > 1) {
                    this->next_next_char = find_character(this->str[this->current_position + 2]);
                } else {
                    this->next_next_char = txtlib::unknown_char;
                }

                this->current_position++;
            }

            bool is_sentence_break() {
#if PARSER_PROFILE
                IPROF_FUNC;
#endif

                const UnicodeData* next = this->next_char;

                // Do not break within CRLF.
                // SB3: CR × LF
                const bool sb3 = !(
                    this->current_char->CR and
                    next->LF
                );

                // Break after paragraph separators.
                // SB4: ParaSep ÷
                const bool sb4 = this->current_char->ParaSep;

                if(sb4 and sb3) return true;

                size_t current_pos = this->current_position;

                // SB5. X (Extend | Format)* → X
                if(next->Format or next->Extend) return false;

                // Do not break after full stop in certain contexts. [SB6-SB8a]
                // SB6. ATerm × Numeric
                const bool sb6 = !(
                    this->current_char->ATerm and
                    next->Numeric
                );

                // SB7. (Upper | Lower) ATerm × Upper
                const bool sb7 = !(
                    (this->previous_char->Upper or this->previous_char->Lower) and
                    this->current_char->ATerm and
                    next->Upper
                );


                // SB8. ATerm Close* Sp* × ( ¬(OLetter | Upper | Lower | ParaSep | SATerm) )* Lower
                const bool ATerm_Close_Sp = (this->SB_classes[2]->ATerm and this->SB_classes[1]->Close and this->SB_classes[0]->Sp) or
                    (this->SB_classes[1]->ATerm and (this->SB_classes[0]->Close or this->SB_classes[0]->Sp)) or
                    (this->SB_classes[0]->ATerm);

                bool sb8 = true;  // Allow by default.

                if(ATerm_Close_Sp) {
                    // We need to perform a lookahead to verify this condition.
                    size_t pos = current_pos;
                    const UnicodeData* lookahead = next;

#if PARSER_DEBUG
                    std::cout << "SB8 lookahead: [";
#endif
                    // (OLetter | Upper | Lower | ParaSep | SATerm)  ==  SB8_RHS1
                    while(pos < (this->input_length - 1) and !lookahead->SB8_RHS1) {
#if PARSER_DEBUG
                        std::cout << " " << this->str[pos];
#endif
                        pos++;
                        lookahead = find_character(this->str[pos]);
                    }

#if PARSER_DEBUG
                    std::cout << "]. Result: " << (lookahead->Lower) << std::endl;
#endif
                    sb8 = !lookahead->Lower; // Disallow breaking if rule matches.
                }

                // SB8a. SATerm Close* Sp* × (SContinue | SATerm)
                const bool SATerm_Close_Sp = (this->SB_classes[2]->SATerm and this->SB_classes[1]->Close and this->SB_classes[0]->Sp) or  // Both Close* and Sp* exist after SATerm.
                    (this->SB_classes[1]->SATerm and (this->SB_classes[0]->Close or this->SB_classes[0]->Sp)) or  // Either Close* or Sp* doesn't exist after SATerm.
                    this->SB_classes[0]->SATerm;  // Neither Close* or Sp* exist after SATerm.

                const bool sb8a = !(
                    SATerm_Close_Sp and
                    (next->SContinue or next->SATerm)
                );

                // SB9. SATerm Close* × (Close | Sp | ParaSep)
                const bool SATerm_Close = (this->SB_classes[1]->SATerm and this->SB_classes[0]->Close) or  // SATerm Close+
                    this->SB_classes[0]->SATerm;  // SATerm

#if PARSER_DEBUG
                std::cout << "ATerm_Close_Sp: " << ATerm_Close_Sp << ", SATerm_Close: " << SATerm_Close << ", next->Sp: " << next->Sp << std::endl;
#endif

                const bool sb9 = !(
                    SATerm_Close and
                    (next->Close or next->Sp or next->ParaSep)
                );

                // SB10. SATerm Close* Sp* × (Sp | ParaSep)
                const bool sb10 = !(
                    SATerm_Close_Sp and
                    (next->Sp or next->ParaSep)
                );

#if PARSER_DEBUG
                std::cout << "(" << (this->SB_classes[3]->SATerm ? "SATerm" : "") << ")  (" <<
                    (this->SB_classes[2]->SATerm ? "SATerm" : "") << ")  (" <<
                        (this->SB_classes[1]->Close ? "Close" : "") << (this->SB_classes[1]->SATerm ? "SATerm" : "") << (this->SB_classes[1]->Sp ? "Sp" : "") <<  ")  (" <<
                            (this->SB_classes[0]->Close ? "Close" : "") << (this->SB_classes[0]->Sp ? "Sp" : "") << (this->SB_classes[0]->SATerm ? "SATerm" : "") <<  ")" <<
                                std::endl;
#endif

                // SB11. SATerm Close* Sp* ParaSep? ÷
                const bool sb11 = (
                    SATerm_Close_Sp  // ParaSep doesn't exist after SATerm Close* Sp*.
                ) or (
                    ((this->SB_classes[3]->SATerm and this->SB_classes[2]->Close and this->SB_classes[1]->Sp) or
                    (this->SB_classes[2]->SATerm and (this->SB_classes[1]->Close or this->SB_classes[1]->Sp)) or
                    this->SB_classes[1]->SATerm) and
                    this->current_char->ParaSep
                );

#if PARSER_DEBUG
                std::cout << "sb3 (" << sb3 << "), sb4 (" << sb4 << "), sb6 (" << sb6 << ")" << ", sb7 (" << sb7 << "), sb8 (" << sb8 << "), sb8a (" << sb8a << "), sb9 (" << sb9 << ")" << ", sb10 (" << sb10 << "), sb11 (" << sb11 << ")" << std::endl;
                std::cout << "Sentence break? " << ((sb4 or sb11) and sb3 and sb6 and sb7 and sb8 and sb8a and sb9 and sb10) << std::endl;
#endif

                // SB4 and SB11 allow breaks and the rest of the rules deny sentence breaks.
                return (sb4 or sb11) and sb3 and sb6 and sb7 and sb8 and sb8a and sb9 and sb10;
            }

            bool is_word_break() {
#if PARSER_PROFILE
                IPROF_FUNC;
#endif

                const UnicodeData* previous = this->previous_char;
                const UnicodeData* current = this->current_char;
                const UnicodeData* next = this->next_char;
                const UnicodeData* next2 = this->next_next_char;

                // WB3. Do not break within CRLF.
                const bool wb3 = !(current->CR and next->LF);

                // Otherwise break before and after Newlines (including CR and LF)
                // WB3a. (Newline | CR | LF) ÷
                const bool wb3a = (current->Newline or current->CR or current->LF);
                // WB3b. ÷ (Newline | CR | LF)
                const bool wb3b = (next->Newline or next->CR or next->LF);

                // Keep horizontal whitespace together.
                // WB3d. WSegSpace × WSegSpace
                const bool wb3d = !(
                    current->WSegSpace and
                    next->WSegSpace
                );

                // WB4. X (Extend | Format)* → X
                if(next->Extend or next->Format) {
                    size_t current_pos = this->current_position;

                    while ((next->Extend or next->Format) and current_pos < (this->input_length - 1)) {
#if PARSER_DEBUG
                        std::cout << "Next is Extend or Format at current_pos " << std::flush;
                        std::cout << current_pos << std::flush;
                        std::wcout << L": (" << (size_t) this->str[current_pos] << L")" << std::flush;
#endif
                        current_pos++;
                        next = find_character(this->str[current_pos]);
                    }

                    if(current_pos < (this->input_length - 1)) {
                        next2 = find_character(this->str[current_pos + 1]);
                    } else {
                        next2 = txtlib::unknown_char;;
                    }
                }

                // Do not break between most letters.
                // WB5. AHLetter × AHLetter
                const bool wb5 = !(
                    current->AHLetter and
                    next->AHLetter
                );

                // Do not break letters across certain punctuation.
                // WB6. AHLetter × (MidLetter | MidNumLetQ) AHLetter
                const bool wb6 = !(
                    current->AHLetter and
                    (next->MidLetter or next->MidNumLetQ) and
                    next2->AHLetter
                );

                // WB7. AHLetter (MidLetter | MidNumLetQ) × AHLetter
                const bool wb7 = !(
                    previous->AHLetter and
                    (current->MidLetter or current->MidNumLetQ) and
                    next->AHLetter
                );

                // WB7a. Hebrew_Letter × Single_Quote
                const bool wb7a = !(
                    current->Hebrew_Letter and
                    next->Single_Quote
                );

                // WB7b. Hebrew_Letter × Double_Quote Hebrew_Letter
                const bool wb7b = !(
                    current->Hebrew_Letter and
                    next->Double_Quote and
                    next2->Hebrew_Letter
                );

                // WB7c. Hebrew_Letter Double_Quote × Hebrew_Letter
                const bool wb7c = !(
                    previous->Hebrew_Letter and
                    current->Double_Quote and
                    next->Hebrew_Letter
                );

                // Do not break within sequences of digits, or digits adjacent to letters.
                // WB8. Numeric × Numeric
                // WB9. AHLetter × Numeric
                const bool wb8_9 = !(
                    (current->Numeric or current->AHLetter) and
                    next->Numeric
                );

                // WB10. Numeric × AHLetter
                const bool wb10 = !(
                    current->Numeric and
                    next->AHLetter
                );

                // WB11. Numeric (MidNum | MidNumLetQ) × Numeric
                const bool wb11 = !(
                    previous->Numeric and
                    (current->MidNum or current->MidNumLetQ) and
                    next->Numeric
                );

                // WB12. Numeric × (MidNum | MidNumLetQ) Numeric
                const bool wb12 = !(
                    current->Numeric and
                    (next->MidNum or next->MidNumLetQ) and
                    next2->Numeric
                );

                // WB13. Katakana × Katakana
                const bool wb13 = !(
                    current->Katakana and
                    next->Katakana
                );

                // WB13a. (AHLetter | Numeric | Katakana | ExtendNumLet) × ExtendNumLet
                const bool wb13a = !(
                    (current->AHLetter or current->Numeric or current->Katakana or current->ExtendNumLet) and
                    next->ExtendNumLet
                );

                // WB13b. ExtendNumLet × (AHLetter | Numeric | Katakana)
                const bool wb13b = !(
                    current->ExtendNumLet and
                    (next->AHLetter or next->Numeric or next->Katakana)
                );

                // WB13c. Regional_Indicator × Regional_Indicator
                const bool wb13c = !(
                    current->Regional_Indicator and
                    next->Regional_Indicator
                );

#if PARSER_DEBUG
                std::cout << "wb3 (" << wb3 << "), wb3a (" << wb3a << "), wb3b (" << wb3b << "), wb3d (" << wb3d << "), wb5 (" << wb5 << "), wb6 (" << wb6 << "), wb7 (" << wb7 << "), wb7a (" << wb7a << "), wb7b (" << wb7b << "), wb7c (" << wb7c << "), ";
                std::cout << "wb8_9 (" << wb8_9 << "), wb10 (" << wb10 << "), wb11 (" << wb11 << "), wb12 (" << wb12 << "), wb13 (" << wb13 << "), wb13a (" << wb13a << "), wb13b (" << wb13b << "), wb13c (" << wb13c << ")" << std::endl;
#endif

                return (wb3 and (wb3a or wb3b)) or (wb3d and wb5 and wb6 and wb7 and wb7a and wb7b and wb7c and wb8_9 and wb10 and wb11 and wb12 and wb13 and wb13a and wb13b and wb13c);
            }

            bool get_token() {
#if PARSER_PROFILE
                IPROF_FUNC;
#endif

                const size_t token_start_position = this->current_position;
                const bool new_sentence = this->new_sentence_flag;
                unsigned long token_mask = 0;

                this->new_sentence_flag = false;

                while(this->n_char_left > 0) {
                    read_char();

                    token_mask = token_mask | this->current_char->General_Category;
                    this->new_sentence_flag = this->new_sentence_flag or this->is_sentence_break();
                    const bool is_word_break = this->is_word_break();

#if PARSER_DEBUG
                    std::cout << "Word break? " << (is_word_break ? "yes" : "no") << std::endl;
#endif

                    if(is_word_break) {
                        size_t token_size = this->current_position - token_start_position;

                        StringType t(&this->str[token_start_position], token_size);
                        this->current_token = IToken<StringType>(t, token_start_position, this->current_position);

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
