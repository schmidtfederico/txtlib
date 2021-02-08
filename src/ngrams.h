#ifndef _NGRAMS_
#define _NGRAMS_

#include <boost/functional/hash.hpp>

#include "mutable_string_view.h"
#include "vectorizers.h"

namespace txtlib {

class NGramView {
public:
    std::vector< mutable_wstring_view > tokens;

    NGramView(const std::vector< mutable_wstring_view > &ngram_tokens) : tokens(ngram_tokens) {};

    NGramView(std::vector< mutable_wstring_view >::iterator begin,
              std::vector< mutable_wstring_view >::iterator end) : tokens(begin, end) {};


    NGramView(std::vector< mutable_wstring_view >::reverse_iterator begin,
              std::vector< mutable_wstring_view >::reverse_iterator end) : tokens(begin, end) {};

    size_t size() const { return this->tokens.size(); }

    size_t hash() const {
        size_t seed = 0;

        for(const auto &t : this->tokens)
            this->hash_combine(seed, t);

        return seed;
    }

    std::wstring as_string() const {
        if(this->size() == 0) return L"";

        std::wstring out(this->tokens[0].data(), this->tokens[0].size());

        for(size_t idx = 1; idx < this->size(); idx++) {
            out += L'_';
            out.append(this->tokens[idx].data(), this->tokens[idx].size());
        }

        return out;
    }
private:
    void hash_combine(size_t &seed, const mutable_wstring_view &v) const {
        // From Boost: https://docs.huihoo.com/boost/1-33-1/doc/html/hash_combine.html
        seed ^= v.hash() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
};


template < class document_vector_t >
class NGramsGenerator {
public:
    virtual ~NGramsGenerator() {};

    NGramsGenerator(UAX29Vectorizer *_vectorizer) : vectorizer(_vectorizer) {
        this->ngrams_size = _vectorizer->ngrams_size;
        this->tokens_buffer.reserve(this->ngrams_size);
    };

    virtual void create_ngrams(const mutable_wstring_view &current_token, document_vector_t& document) {
        // If the buffer has ngram_size elements, delete the first (oldest) token.
        if(tokens_buffer.size() == ngrams_size) tokens_buffer.erase(tokens_buffer.begin());
        tokens_buffer.push_back(current_token);

        if(tokens_buffer.size() > 1) {
            for(size_t token_idx = tokens_buffer.size() - 1; token_idx > 0; token_idx--) {
                NGramView ngram(tokens_buffer.begin() + (token_idx - 1), tokens_buffer.end());
                vectorizer->put_token(ngram, document);
            }
        }
    }

    virtual void reset() {
        this->tokens_buffer.clear();
    }

protected:
    UAX29Vectorizer *vectorizer;
    size_t ngrams_size;
    std::vector< mutable_wstring_view > tokens_buffer;
};


template < class document_vector_t >
class NoOpNGramsGenerator : public NGramsGenerator< document_vector_t > {
public:
    using NGramsGenerator< document_vector_t >::NGramsGenerator;

    void create_ngrams(const mutable_wstring_view &current_token, document_vector_t& document) {}
    void reset() {}
};


template < class document_vector_t >
class BigramsGenerator : public NGramsGenerator< document_vector_t > {
public:
    BigramsGenerator(UAX29Vectorizer *_vectorizer) : NGramsGenerator< document_vector_t >::NGramsGenerator(_vectorizer) {
        this->tokens_buffer = std::vector< mutable_wstring_view >(2);
    };

    void create_ngrams(const mutable_wstring_view &current_token, document_vector_t& document) {
        this->tokens_buffer[1] = this->tokens_buffer[0];
        this->tokens_buffer[0] = current_token;

        if(this->tokens_buffer[1].length() > 0) {
            NGramView ngram(this->tokens_buffer.rbegin(), this->tokens_buffer.rend());
            this->vectorizer->put_token(ngram, document);
        }
    }

    void reset() {
        this->tokens_buffer[0].clear();
        this->tokens_buffer[1].clear();
    }
};

}

bool operator==(txtlib::NGramView __lhs, txtlib::NGramView __rhs) noexcept {
    if (__lhs.size() != __rhs.size()) return false;

    for(size_t token_idx = 0; token_idx < __lhs.size(); token_idx++)
        if(__lhs.tokens[token_idx].compare(__rhs.tokens[token_idx]) != 0)
            return false;

        return true;
}

namespace std {
    template<>
    struct std::hash< txtlib::NGramView > : public unary_function< txtlib::NGramView, size_t > {
        size_t operator()(const txtlib::NGramView& ngram) const noexcept {
            return ngram.hash();
        };
    };
}
#endif