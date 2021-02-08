#ifndef _MUTABLE_STRING_VIEW_
#define _MUTABLE_STRING_VIEW_

#include <string>
#include <iterator>
#include <algorithm>
#include <utility>
#include <vector>
#include <ostream>

#include "murmur2.h"

#include <boost/range.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/functional/hash.hpp>


template< class T, class Traits = std::char_traits<T> >
class base_mutable_string_view
{

public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;

    typedef T& reference;
    typedef T const& const_reference;

    typedef const_pointer const_iterator;
    typedef pointer iterator;

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    typedef size_t size_type;

    typedef typename std::iterator_traits<iterator>::difference_type difference_type;
    typedef Traits traits_type;

    static const size_type npos = static_cast<size_type>(-1);

private:
    pointer base_start;
    pointer base_end;

    pointer _start;
    pointer _end;

    size_type _len;

public:

    base_mutable_string_view() : base_start(nullptr), base_end(nullptr), _start(nullptr), _end(nullptr), _len(0) {}

    base_mutable_string_view(pointer data_in, size_type size_in) : base_start(data_in), base_end(data_in + size_in), _start(data_in), _end(data_in + size_in) {
        if(size_in > 0 and data_in == nullptr) throw std::runtime_error("Invalid data_in parameter (nullptr)");
        this->_len = size_in;
    }

    base_mutable_string_view(pointer _start_ptr, pointer _end_ptr) : base_start(_start_ptr), base_end(_end_ptr), _start(_start_ptr), _end(_end_ptr) {
        if(_start_ptr > _end_ptr) throw std::runtime_error("Invalid start and end pointer (start_ptr > end_ptr)");
        this->_len = _end_ptr - _start_ptr;
    }

    base_mutable_string_view(pointer _start_ptr) : base_start(_start_ptr), _start(_start_ptr) {
        size_type len = Traits::length(_start_ptr);
        this->base_end = this->_start + len;
        this->_end = this->base_end;
        this->_len = len;
    }

    base_mutable_string_view operator=(const_pointer new_value) {
        size_type len = Traits::length(new_value);
        if(len > this->max_size())
            throw std::runtime_error("Tried to mutate a string view to be larger that the original string");
        std::copy(new_value, new_value + len, this->_start);
        this->_end = this->_start + len;
        this->_len = len;
        return *this;
    }

    base_mutable_string_view operator+=(const T& a_char) {
        if(this->_len + 1 > this->max_size())
            throw std::runtime_error("Tried to mutate a string view to be larger than the original string by appending to it");
        *(this->_end + 1) = a_char;
        this->_end = this->_end + 1;
        this->_len = this->_end - this->_start;
        return *this;
    }

    // Iterators.
    iterator begin() const { return iterator( _start ); }
    const_iterator cbegin() const { return { this->begin() }; }

    iterator end() const { return iterator( _end ); }
    const_iterator cend() const { return { this->end() }; }

    reverse_iterator rbegin() const { return reverse_iterator(this->end()); }
    const_reverse_iterator crbegin() const { return const_reverse_iterator(this->cend()); }

    reverse_iterator rend() const { return reverse_iterator(this->begin()); }
    const_reverse_iterator crend() const { return const_reverse_iterator(this->cbegin()); }


    // Element access.
    reference operator[](const size_type& index) const { return this->at(index); }

    reference at(const size_type& index) const {
        if(index >= this->_len) throw std::runtime_error("Invalid index");
        return this->_start[index];
    }

    reference front() const { return this->_start[0]; }
    reference back() const { return this->_start[this->_len - 1]; }
    pointer data() const { return this->_start; }

    // Allow mutations.
    reference operator()(const size_type& index) const { return at(index); }

    // Capacity.
    size_type size() const { return this->_len; }
    size_type length() const { return this->_len; }

    size_type max_size() const { return this->base_end - this->base_start; }

    bool empty() const { return this->_len == 0; }

    // Modifiers.
    void remove_prefix(size_type n) {
        if(n > this->_len) throw std::runtime_error("Tried to remove more chars from prefix than string size");
        this->_start += n;
        this->_len -= n;
    }

    void remove_suffix(size_type n) {
        if(n > this->_len) throw std::runtime_error("Tried to remove more chars from suffix than string size");
        this->_end -= n;
        this->_len -= n;
    }

    void swap(base_mutable_string_view &other) {
        using std::swap;
        swap(_start, other._start);
        swap(_end , other._end);
        swap(_len , other._len);
    }

    base_mutable_string_view& erase(size_type index = 0, size_type count = npos) {
        size_type rm = std::min(count, this->_len - index);

        if(index > 0) {
            if((index + rm) < this->_len) throw std::runtime_error("Cannot remove characters in between. Only suffix & prefix trimming supported");
            this->remove_suffix(rm);
        } else {
            this->remove_prefix(rm);
        }

        return *this;
    }

    // Operations.

    void clear() {
        this->_end = this->_start;
        this->_len = 0;
    }

    int compare(const base_mutable_string_view &_other) const noexcept {
        size_type compare_length = std::min(this->_len, _other.size());
        int result = Traits::compare(this->_start, _other._start, compare_length);
        if (result == 0) // first compare_length chars matched
            result = this->_len == _other._len ? 0 : (this->_len < _other._len ? -1 : 1 );
        return result;
    }

    int compare(const T* __s) const {
        return compare(base_mutable_string_view(__s));
    }

    // find
    size_type find(const T* s, size_type pos, size_type count) const noexcept {
        if (pos > this->_len) return npos;
        if (count < 1) return pos;

        const_iterator result = std::search(this->cbegin() + pos, this->cend(), s, s + count, Traits::eq);

        return (result == this->cend()) ? npos : std::distance(this->cbegin(), result);
    }

    size_type find(base_mutable_string_view s, size_type pos = 0) const noexcept {
        return this->find(s._start, pos, s.size());
    }

    size_type find(T ch, size_type pos = 0) const noexcept {
        return this->find(&ch, pos, 1);
    }

    size_type find(const T* s, size_type pos = 0) const noexcept {
        return this->find(s, pos, Traits::length(s));
    }


    // rfind
    size_type rfind(const T* s, size_type pos, size_type count) const noexcept {
        if (this->_len < count) return npos;
        if (pos > this->_len - count) pos = this->_len - count;
        if (count == 0) return pos;

        for (const T* c = this->_end; ; --c) {
            if (Traits::compare(c, s, count) == 0) return c - this->_start;
            if (c == this->_start) return npos;
        }
    }

    size_type rfind(base_mutable_string_view s, size_type pos = npos) const noexcept {
        return this->rfind(s._start, pos, s.size());
    }

    size_type rfind(T ch, size_type pos = npos) const noexcept {
        return this->rfind(&ch, pos, 1);
    }

    size_type rfind(const T* s, size_type pos = npos) const noexcept {
        return this->rfind(s, pos, Traits::length(s));
    }


    // find_first_of
    size_type find_first_of(const T* s, size_type pos, size_type count) const noexcept {
        if (pos >= this->_len || count == 0) return npos;

        const_iterator result = std::find_first_of(this->cbegin() + pos, this->cend(), s, s + count, Traits::eq);
        return (result == this->cend()) ? npos : std::distance(this->cbegin(), result);
    }

    size_type find_first_of(base_mutable_string_view s, size_type pos = 0) const noexcept {
        return this->find_first_of(s._start, pos, s.size());
    }

    size_type find_first_of(T ch, size_type pos = 0) const noexcept {
        return this->find_first_of(&ch, pos, 1);
    }

    size_type find_first_of(const T* s, size_type pos = 0) const noexcept {
        return this->find_first_of(s, pos, Traits::length(s));
    }


    // find_last_of
    size_type find_last_of(const T* s, size_type pos, size_type count) const noexcept {
        if (count == 0) return npos;

        if (pos >= this->_len) {
            pos = 0;
        } else {
            pos = this->_len - (pos + 1);
        }

        const_reverse_iterator result = std::find_first_of(this->crbegin() + pos, this->crend(), s, s + count, Traits::eq);

        return (result == this->crend()) ? npos : (this->_len - std::distance(this->crbegin(), result) - 1);
    }

    size_type find_last_of(base_mutable_string_view s, size_type pos = npos) const noexcept {
        return this->find_last_of(s._start, pos, s.size());
    }

    size_type find_last_of(T ch, size_type pos = npos) const noexcept {
        return this->find_last_of(&ch, pos, 1);
    }

    size_type find_last_of(const T* s, size_type pos = npos) const noexcept {
        return this->find_last_of(s, pos, Traits::length(s));
    }

    // find_first_not_of
    size_type find_first_not_of(const T* s, size_type pos, size_type count) const noexcept {
        if (pos >= this->_len || count == 0) return npos;

        const_iterator result = this->find_not_of(this->cbegin() + pos, this->cend(), s, count);

        return (result == this->cend()) ? npos : std::distance(this->cbegin(), result);
    }

    size_type find_first_not_of(base_mutable_string_view s, size_type pos = 0) const noexcept {
        return this->find_first_not_of(s._start, pos, s.size());
    }

    size_type find_first_not_of(T ch, size_type pos = 0) const noexcept {
        return this->find_first_not_of(&ch, pos, 1);
    }

    size_type find_first_not_of(const T* s, size_type pos = 0) const noexcept {
        return this->find_first_not_of(s, pos, Traits::length(s));
    }

    //  find_last_not_of
    size_type find_last_not_of(const T* s, size_type pos, size_type count) const noexcept {
        if (pos >= this->_len) pos = this->_len - 1;
        if (count == 0) return pos;

        pos = this->_len - (pos + 1);
        const_reverse_iterator result = this->find_not_of(this->crbegin() + pos, this->crend(), s, count);
        return (result == this->crend()) ? npos : (this->_len - std::distance(this->crbegin(), result) - 1);
    }

    size_type find_last_not_of(base_mutable_string_view s, size_type pos = npos) const noexcept {
        return this->find_last_not_of(s._start, pos, s.size());
    }

    size_type find_last_not_of(T ch, size_type pos = npos) const noexcept {
        return this->find_last_not_of(&ch, pos, 1);
    }

    size_type find_last_not_of(const T* s, size_type pos = npos) const noexcept {
        return this->find_last_not_of(s, pos, Traits::length(s));
    }


    // Hashing
    size_type hash() const noexcept {
        return MurmurHash2(this->_start, (this->_end - this->_start) * sizeof(value_type), 0);
    }



private:
    template <typename Iterator>
    Iterator find_not_of(Iterator first, Iterator last, const T* s, size_type count) const noexcept {
        for(; first != last; ++first) {
            if(Traits::find(s, count, *first) == 0) return first;
        }
        return last;
    }

};


// operator ==
template<class T, class Traits>
bool operator==(const base_mutable_string_view<T, Traits> &__lhs, const base_mutable_string_view<T, Traits> &__rhs) noexcept {
    if (__lhs.size() != __rhs.size()) return false;
    return __lhs.compare(__rhs) == 0;
}

template<class T, class Traits>
bool operator==(const base_mutable_string_view<T, Traits> &__lhs, typename std::common_type<base_mutable_string_view<T, Traits> >::type __rhs) noexcept {
    if (__lhs.size() != __rhs.size()) return false;
    return __lhs.compare(__rhs) == 0;
}

template<class T, class Traits> bool operator==(typename std::common_type<base_mutable_string_view<T, Traits> >::type __lhs, base_mutable_string_view<T, Traits> __rhs) noexcept {
    if (__lhs.size() != __rhs.size()) return false;
    return __lhs.compare(__rhs) == 0;
}


// Stream IO
template<class T, class Traits> std::basic_ostream<T, Traits>&
operator<<(std::basic_ostream<T, Traits>& _stream, const base_mutable_string_view<T, Traits> &msv) {
    return __put_character_sequence(_stream, msv.data(), msv.size());
}

typedef base_mutable_string_view<char> mutable_string_view;
typedef base_mutable_string_view<wchar_t> mutable_wstring_view;

namespace std {

    template<class T, class Traits>
    struct hash<base_mutable_string_view<T, Traits>> : public unary_function<base_mutable_string_view<T, Traits>, size_t>
    {
        size_t operator()(const base_mutable_string_view<T, Traits>& msv) const noexcept {
            return msv.hash();
        };
    };

}
#endif
