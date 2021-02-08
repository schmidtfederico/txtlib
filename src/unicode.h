
#ifndef _UNICODE_DATA_
#define _UNICODE_DATA_

#define UNICODE_DEBUG 0

#include <iostream>
#include <map>

#if UNICODE_DEBUG
#include "iprof.hpp"
#endif

namespace txtlib {

    enum GeneralCategory {
        Cc = 1UL,
        Zs = 2UL,
        Po = 4UL,
        Sc = 8UL,
        Ps = 16UL,
        Pe = 32UL,
        Sm = 64UL,
        Pd = 128UL,
        Nd = 256UL,
        Lu = 512UL,
        Sk = 1024UL,
        Pc = 2048UL,
        Ll = 4096UL,
        So = 8192UL,
        Lo = 16384UL,
        Pi = 32768UL,
        Cf = 65536UL,
        No = 131072UL,
        Pf = 262144UL,
        Lt = 524288UL,
        Lm = 1048576UL,
        Mn = 2097152UL,
        Me = 4194304UL,
        Mc = 8388608UL,
        Nl = 16777216UL,
        Zl = 33554432UL,
        Zp = 67108864UL,
        Cs = 134217728UL,
        Co = 268435456UL,
        Unknown = 536870912UL
    };

    const static std::map< std::string, size_t > GeneralCategoryValues = {
        {"Cc", 1UL},
        {"Zs", 2UL},
        {"Po", 4UL},
        {"Sc", 8UL},
        {"Ps", 16UL},
        {"Pe", 32UL},
        {"Sm", 64UL},
        {"Pd", 128UL},
        {"Nd", 256UL},
        {"Lu", 512UL},
        {"Sk", 1024UL},
        {"Pc", 2048UL},
        {"Ll", 4096UL},
        {"So", 8192UL},
        {"Lo", 16384UL},
        {"Pi", 32768UL},
        {"Cf", 65536UL},
        {"No", 131072UL},
        {"Pf", 262144UL},
        {"Lt", 524288UL},
        {"Lm", 1048576UL},
        {"Mn", 2097152UL},
        {"Me", 4194304UL},
        {"Mc", 8388608UL},
        {"Nl", 16777216UL},
        {"Zl", 33554432UL},
        {"Zp", 67108864UL},
        {"Cs", 134217728UL},
        {"Co", 268435456UL},
        {"Unknown", 536870912UL}
    };

    struct UnicodeData {
        const GeneralCategory General_Category;
    };


    const static UnicodeData unique_character_groups[] = {
        { GeneralCategory::Cc },
        { GeneralCategory::Zs },
        { GeneralCategory::Po },
        { GeneralCategory::Sc },
        { GeneralCategory::Ps },
        { GeneralCategory::Pe },
        { GeneralCategory::Sm },
        { GeneralCategory::Pd },
        { GeneralCategory::Nd },
        { GeneralCategory::Lu },
        { GeneralCategory::Sk },
        { GeneralCategory::Pc },
        { GeneralCategory::Ll },
        { GeneralCategory::So },
        { GeneralCategory::Lo },
        { GeneralCategory::Pi },
        { GeneralCategory::Cf },
        { GeneralCategory::No },
        { GeneralCategory::Pf },
        { GeneralCategory::Lt },
        { GeneralCategory::Lm },
        { GeneralCategory::Mn },
        { GeneralCategory::Me },
        { GeneralCategory::Mc },
        { GeneralCategory::Nl },
        { GeneralCategory::Zl },
        { GeneralCategory::Zp },
        { GeneralCategory::Unknown },
        { GeneralCategory::Cs },
        { GeneralCategory::Co }
    };

    const static UnicodeData* ascii_chars[] = {&unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[1], &unique_character_groups[2], &unique_character_groups[2], &unique_character_groups[2], &unique_character_groups[3], &unique_character_groups[2], &unique_character_groups[2], &unique_character_groups[2], &unique_character_groups[4], &unique_character_groups[5], &unique_character_groups[2], &unique_character_groups[6], &unique_character_groups[2], &unique_character_groups[7], &unique_character_groups[2], &unique_character_groups[2], &unique_character_groups[8], &unique_character_groups[8], &unique_character_groups[8], &unique_character_groups[8], &unique_character_groups[8], &unique_character_groups[8], &unique_character_groups[8], &unique_character_groups[8], &unique_character_groups[8], &unique_character_groups[8], &unique_character_groups[2], &unique_character_groups[2], &unique_character_groups[6], &unique_character_groups[6], &unique_character_groups[6], &unique_character_groups[2], &unique_character_groups[2], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[4], &unique_character_groups[2], &unique_character_groups[5], &unique_character_groups[10], &unique_character_groups[11], &unique_character_groups[10], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[4], &unique_character_groups[6], &unique_character_groups[5], &unique_character_groups[6], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[0], &unique_character_groups[1], &unique_character_groups[2], &unique_character_groups[3], &unique_character_groups[3], &unique_character_groups[3], &unique_character_groups[3], &unique_character_groups[13], &unique_character_groups[2], &unique_character_groups[10], &unique_character_groups[13], &unique_character_groups[14], &unique_character_groups[15], &unique_character_groups[6], &unique_character_groups[16], &unique_character_groups[13], &unique_character_groups[10], &unique_character_groups[13], &unique_character_groups[6], &unique_character_groups[17], &unique_character_groups[17], &unique_character_groups[10], &unique_character_groups[12], &unique_character_groups[2], &unique_character_groups[2], &unique_character_groups[10], &unique_character_groups[17], &unique_character_groups[14], &unique_character_groups[18], &unique_character_groups[17], &unique_character_groups[17], &unique_character_groups[17], &unique_character_groups[2], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[6], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[9], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[6], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12], &unique_character_groups[12]};

    const static UnicodeData* unknown_char = &unique_character_groups[27];

    const UnicodeData* find_character(const wchar_t &code);

    // Casing functions.
    void preserve_case(wchar_t &code);
    void to_lowercase(wchar_t &code);

    void debug_unicode_data(const UnicodeData *unicode_data);
}

#endif

