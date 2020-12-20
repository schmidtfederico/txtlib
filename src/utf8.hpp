#ifndef _UTF8_R_FUNCTIONS_
#define _UTF8_R_FUNCTIONS_

#include "mutable_string_view.hpp"
#include <string>
#include <codecvt>
#include <locale>

namespace txtlib {

inline std::wstring utf8_to_ws(const std::string &utf8) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cnv;
    std::wstring ws = cnv.from_bytes(utf8);
    if(cnv.converted() < utf8.size())
        throw std::runtime_error("Incomplete UTF-8 conversion");
    return ws;
}

inline std::string ws_to_utf8(const mutable_wstring_view &ws) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cnv;
    std::string utf8 = cnv.to_bytes(ws.begin(), ws.end());
    if(cnv.converted() < ws.size())
        throw std::runtime_error("Incomplete UTF-8 conversion");
    return utf8;
}

}

#endif
