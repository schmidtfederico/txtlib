#ifndef _STEMMING_
#define _STEMMING_

#include "mutable_string_view.h"

#include "o_stemmer/stemming/stemming.h"
#include "o_stemmer/stemming/danish_stem.h"
#include "o_stemmer/stemming/dutch_stem.h"
#include "o_stemmer/stemming/english_stem.h"
#include "o_stemmer/stemming/finnish_stem.h"
#include "o_stemmer/stemming/french_stem.h"
#include "o_stemmer/stemming/german_stem.h"
#include "o_stemmer/stemming/italian_stem.h"
#include "o_stemmer/stemming/norwegian_stem.h"
#include "o_stemmer/stemming/portuguese_stem.h"
#include "o_stemmer/stemming/russian_stem.h"
#include "o_stemmer/stemming/spanish_stem.h"
#include "o_stemmer/stemming/swedish_stem.h"

namespace txtlib {

    class Stemmer {
    public:
        virtual ~Stemmer() {};
        virtual void stem(mutable_wstring_view& token) {}
    };

    template < class stemmer_t >
    class OStemmer : public Stemmer {
    private:
        stemmer_t s;
    public:
        void stem(mutable_wstring_view& token) {
            this->s(token);
        }
    };

    inline Stemmer* create_stemmer_pointer(const std::string &stem_language) {
        if(stem_language.length() > 0) {
            if(stem_language == "danish") return new OStemmer< stemming::danish_stem< mutable_wstring_view > >();
            // if(stem_language == "dutch") return new OStemmer< stemming::dutch_stem< mutable_wstring_view > >();
            if(stem_language == "english") return new OStemmer< stemming::english_stem< mutable_wstring_view > >();
            // if(stem_language == "finnish") return new OStemmer< stemming::finnish_stem< mutable_wstring_view > >();
            // if(stem_language == "french") return new OStemmer< stemming::french_stem< mutable_wstring_view > >();
            // if(stem_language == "german") return new OStemmer< stemming::german_stem< mutable_wstring_view > >();
            // if(stem_language == "italian") return new OStemmer< stemming::italian_stem< mutable_wstring_view > >();
            if(stem_language == "norwegian") return new OStemmer< stemming::norwegian_stem< mutable_wstring_view > >();
            // if(stem_language == "portuguese") return new OStemmer< stemming::portuguese_stem< mutable_wstring_view > >();
            // if(stem_language == "russian") return new OStemmer< stemming::russian_stem< mutable_wstring_view > >();
            if(stem_language == "spanish") return new OStemmer< stemming::spanish_stem< mutable_wstring_view > >();
            if(stem_language == "swedish") return new OStemmer< stemming::swedish_stem< mutable_wstring_view > >();
            else throw std::runtime_error("Unsupported stemming language: " + stem_language);
        } else {
            // No-op stemmer.
            return new Stemmer();
        }
    }
}

#endif