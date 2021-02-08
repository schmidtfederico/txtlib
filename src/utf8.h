#ifndef _UTF8_R_FUNCTIONS_
#define _UTF8_R_FUNCTIONS_

#include <Rcpp.h>
#include "mutable_string_view.h"
#include <string>
#include <codecvt>
#include <locale>
#include <unicode/utf.h>

namespace txtlib {

template< class wstring_t = std::wstring >
inline wstring_t to_ws_icu(const char* str, size_t length) {
    using char_t = typename wstring_t::value_type;

    wstring_t w(length, 0);

    UChar32 u32_char = 0;
    R_len_t bytes_processed = 0;
    R_len_t i = 0;
    while (bytes_processed < length) {
        U8_NEXT(str, bytes_processed, length, u32_char);
        w[i] = static_cast< char_t >(u32_char);
        i++;
    }

    w.resize(i);

    return w;
}

template< class wstring_t = std::wstring >
inline wstring_t to_ws(const char* str, size_t length) {
    using char_t = typename wstring_t::value_type;

    std::wstring_convert< std::codecvt_utf8< char_t >, char_t > converter;
    wstring_t converted = converter.from_bytes(str, str + length);

    if(converter.converted() < length)
        throw std::runtime_error("Incomplete UTF-8 conversion");
    return converted;
}


inline std::wstring utf8_to_ws(const char* c, size_t len) {
    return to_ws_icu(c, len);
}

inline std::wstring utf8_to_ws(const std::string &utf8) {
    return to_ws_icu(utf8.c_str(), utf8.size());
}

inline std::wstring utf8_to_ws(const Rcpp::String& s) {
    char* str = (char *)(s.get_cstring());
    size_t str_len = LENGTH(s.get_sexp());
    return to_ws_icu(str, str_len);
}


template< class wstring_t >
inline std::string to_utf8(wstring_t ws) {
    using char_t = typename wstring_t::value_type;
    std::wstring_convert< std::codecvt_utf8< char_t >, char_t > converter;
    std::string utf8 = converter.to_bytes(ws.data(), ws.data() + ws.size());
    if(converter.converted() < ws.size())
        throw std::runtime_error("Incomplete UTF-8 conversion");
    return utf8;
}

inline std::string ws_to_utf8(const mutable_wstring_view &ws) {
    return to_utf8(ws);
}

inline std::string ws_to_utf8(const std::wstring &ws) {
    return to_utf8(ws);
}

}

#endif
