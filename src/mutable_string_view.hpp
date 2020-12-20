#ifndef _MUTABLE_STRING_VIEW_
#define _MUTABLE_STRING_VIEW_

#include <string>
#include <iterator>
#include <algorithm>
#include <utility>
#include <vector>
#include <ostream>


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

private:
    pointer base_start;
    pointer base_end;

    pointer _start;
    pointer _end;

public:

    base_mutable_string_view() : base_start(nullptr), base_end(nullptr), _start(nullptr), _end(nullptr) {}

    base_mutable_string_view(pointer data_in, size_type size_in) : base_start(data_in), base_end(data_in + size_in), _start(data_in), _end(data_in + size_in) {
        if(size_in > 0 and data_in == nullptr) throw std::runtime_error("Invalid data_in parameter (nullptr)");
    }

    base_mutable_string_view(pointer _start_ptr, pointer _end_ptr) : base_start(_start_ptr), base_end(_end_ptr), _start(_start_ptr), _end(_end_ptr) {
        if(_start_ptr > _end_ptr) throw std::runtime_error("Invalid start and end pointer (start_ptr > end_ptr)");
    }

    base_mutable_string_view(pointer _start_ptr) : base_start(_start_ptr), _start(_start_ptr) {
        size_type len = Traits::length(_start_ptr);
        this->base_end = this->_start + len;
        this->_end = this->base_end;
    }

    base_mutable_string_view operator=(const_pointer new_value) {
        size_type len = Traits::length(new_value);
        if(len > this->max_size())
            throw std::runtime_error("Tried to mutate a string view to be larger that the original string");
        std::copy(new_value, new_value + len, this->_start);
        this->_end = this->_start + len;
        return *this;
    }

    base_mutable_string_view operator+=(const T& a_char) {
        if(this->size() + 1 > this->max_size())
            throw std::runtime_error("Tried to mutate a string view to be larger than the original string by appending to it");
        *(this->_end + 1) = a_char;
        this->_end = this->_end + 1;
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
    reference operator[](size_type index) const { return this->at(index); }

    reference at(size_type index) const {
        if(index >= this->size()) throw std::runtime_error("Invalid index");
        return this->_start[index];
    }

    reference front() const { return this->_start[0]; }
    reference back() const { return this->_start[this->size() - 1]; }
    pointer data() const { return this->_start; }

    // Allow mutations.
    reference operator()(size_type index) const { return at(index); }

    // Capacity.
    size_type size() const { return this->_end - this->_start; }
    size_type length() const { return this->size(); }

    size_type max_size() const { return this->base_end - this->base_start; }

    bool empty() const { return this->size() == 0; }

    // Modifiers.
    void remove_prefix(size_type n) {
        if(n > this->size()) throw std::runtime_error("Tried to remove more chars from prefix than string size");
        this->_start += n;
    }

    void remove_suffix(size_type n) {
        if(n > this->size()) throw std::runtime_error("Tried to remove more chars from suffix than string size");
        this->_end -= n;
    }

    void swap(base_mutable_string_view &other) {
        using std::swap;
        swap(_start, other._start);
        swap(_end , other._end);
    }

    base_mutable_string_view& erase(size_type index = 0, size_type count = npos) {
        size_type rm = std::min(count, this->size() - index);

        if(index > 0) {
            if((index + rm) < this->size()) throw std::runtime_error("Cannot remove characters in between. Only suffix & prefix trimming supported");
            this->remove_suffix(rm);
        } else {
            this->remove_prefix(rm);
        }

        return *this;
    }

    // Operations.

    void clear() {
        this->_end = this->_start;
    }

    int compare(const base_mutable_string_view &_other) const noexcept {
        size_type compare_length = std::min(this->size(), _other.size());
        int result = Traits::compare(data(), _other.data(), compare_length);
        if ( result == 0 ) // first compare_length chars matched
            result = this->size() == _other.size() ? 0 : ( this->size() < _other.size() ? -1 : 1 );
        return result;
    }

    int compare(const T* __s) const {
        return compare(base_mutable_string_view(__s));
    }

    // find
    size_type find(const base_mutable_string_view &s, size_type pos = 0) const noexcept {
        _LIBCPP_ASSERT(s.size() == 0 || s.data() != nullptr, "mutable_string_view::find(): recieved nullptr");
        return std::__str_find<value_type, size_type, traits_type, npos>(data(), size(), s.data(), pos, s.size());
    }

    size_type find(T ch, size_type pos = 0) const noexcept {
        return std::__str_find<value_type, size_type, traits_type, npos>(data(), size(), ch, pos);
    }

    size_type find(const T* s, size_type pos, size_type count) const {
        _LIBCPP_ASSERT(count == 0 || s != nullptr, "mutable_string_view::find(): recieved nullptr");
        return std::__str_find<value_type, size_type, traits_type, npos>(data(), size(), s, pos, count);
    }

    size_type find(const T* s, size_type pos = 0) const {
        _LIBCPP_ASSERT(s != nullptr, "mutable_string_view::find(): recieved nullptr");
        return std::__str_find<value_type, size_type, traits_type, npos>(data(), size(), s, pos, traits_type::length(s));
    }

    // rfind
    size_type rfind(const base_mutable_string_view &s, size_type pos = npos) const noexcept {
        _LIBCPP_ASSERT(s.size() == 0 || s.data() != nullptr, "mutable_string_view::find(): recieved nullptr");
        return std::__str_rfind<value_type, size_type, traits_type, npos>(data(), size(), s.data(), pos, s.size());
    }

    size_type rfind(T ch, size_type pos = npos) const noexcept {
        return std::__str_rfind<value_type, size_type, traits_type, npos>(data(), size(), ch, pos);
    }

    size_type rfind(const T* s, size_type pos, size_type count) const {
        _LIBCPP_ASSERT(count == 0 || s != nullptr, "mutable_string_view::rfind(): recieved nullptr");
        return std::__str_rfind<value_type, size_type, traits_type, npos>(data(), size(), s, pos, count);
    }

    size_type rfind(const T* s, size_type pos=npos) const {
        _LIBCPP_ASSERT(s != nullptr, "mutable_string_view::rfind(): recieved nullptr");
        return std::__str_rfind<value_type, size_type, traits_type, npos>(data(), size(), s, pos, traits_type::length(s));
    }

    // find_first_of
    size_type find_first_of(const base_mutable_string_view &s, size_type pos = 0) const noexcept {
        _LIBCPP_ASSERT(s.size() == 0 || s.data() != nullptr, "mutable_string_view::find_first_of(): recieved nullptr");
        return std::__str_find_first_of<value_type, size_type, traits_type, npos>(data(), size(), s.data(), pos, s.size());
    }

    size_type find_first_of(T ch, size_type pos = 0) const noexcept { return find(ch, pos); }

    size_type find_first_of(const T* s, size_type pos, size_type count) const {
        _LIBCPP_ASSERT(count == 0 || s != nullptr, "mutable_string_view::find_first_of(): recieved nullptr");
        return std::__str_find_first_of<value_type, size_type, traits_type, npos>(data(), size(), s, pos, count);
    }

    size_type find_first_of(const T* s, size_type pos=0) const {
        _LIBCPP_ASSERT(s != nullptr, "mutable_string_view::find_first_of(): recieved nullptr");
        return std::__str_find_first_of<value_type, size_type, traits_type, npos>(data(), size(), s, pos, traits_type::length(s));
    }

    // find_last_of
    size_type find_last_of(const base_mutable_string_view &s, size_type pos=npos) const noexcept {
        _LIBCPP_ASSERT(s.size() == 0 || s.data() != nullptr, "mutable_string_view::find_last_of(): recieved nullptr");
        return std::__str_find_last_of<value_type, size_type, traits_type, npos>(data(), size(), s.data(), pos, s.size());
    }

    size_type find_last_of(T ch, size_type pos = npos) const noexcept { return rfind(ch, pos); }

    size_type find_last_of(const T* s, size_type pos, size_type count) const {
        _LIBCPP_ASSERT(count == 0 || s != nullptr, "mutable_string_view::find_last_of(): recieved nullptr");
        return std::__str_find_last_of<value_type, size_type, traits_type, npos>(data(), size(), s, pos, count);
    }

    size_type find_last_of(const T* s, size_type pos=npos) const {
        _LIBCPP_ASSERT(s != nullptr, "mutable_string_view::find_last_of(): recieved nullptr");
        return std::__str_find_last_of<value_type, size_type, traits_type, npos>(data(), size(), s, pos, traits_type::length(s));
    }

    // find_first_not_of
    size_type find_first_not_of(const base_mutable_string_view &s, size_type pos=0) const noexcept {
        _LIBCPP_ASSERT(s.size() == 0 || s.data() != nullptr, "mutable_string_view::find_first_not_of(): recieved nullptr");
        return std::__str_find_first_not_of<value_type, size_type, traits_type, npos>(data(), size(), s.data(), pos, s.size());
    }

    size_type find_first_not_of(T ch, size_type pos=0) const noexcept {
        return std::__str_find_first_not_of<value_type, size_type, traits_type, npos>(data(), size(), ch, pos);
    }

    size_type find_first_not_of(const T* s, size_type pos, size_type count) const {
        _LIBCPP_ASSERT(count == 0 || s != nullptr, "mutable_string_view::find_first_not_of(): recieved nullptr");
        return std::__str_find_first_not_of<value_type, size_type, traits_type, npos>(data(), size(), s, pos, count);
    }

    size_type find_first_not_of(const T* s, size_type pos=0) const {
        _LIBCPP_ASSERT(s != nullptr, "mutable_string_view::find_first_not_of(): recieved nullptr");
        return std::__str_find_first_not_of<value_type, size_type, traits_type, npos>(data(), size(), s, pos, traits_type::length(s));
    }

    // find_last_not_of
    size_type find_last_not_of(const base_mutable_string_view &s, size_type pos=npos) const noexcept {
        _LIBCPP_ASSERT(s.size() == 0 || s.data() != nullptr, "mutable_string_view::find_last_not_of(): recieved nullptr");
        return std::__str_find_last_not_of<value_type, size_type, traits_type, npos>(data(), size(), s.data(), pos, s.size());
    }

    size_type find_last_not_of(T ch, size_type pos=npos) const noexcept {
        return std::__str_find_last_not_of<value_type, size_type, traits_type, npos>(data(), size(), ch, pos);
    }

    size_type find_last_not_of(const T* s, size_type pos, size_type count) const {
        _LIBCPP_ASSERT(count == 0 || s != nullptr, "mutable_string_view::find_last_not_of(): recieved nullptr");
        return std::__str_find_last_not_of<value_type, size_type, traits_type, npos>(data(), size(), s, pos, count);
    }

    size_type find_last_not_of(const T* s, size_type pos=npos) const {
        _LIBCPP_ASSERT(s != nullptr, "mutable_string_view::find_last_not_of(): recieved nullptr");
        return std::__str_find_last_not_of<value_type, size_type, traits_type, npos>(data(), size(), s, pos, traits_type::length(s));
    }

    static constexpr size_type npos = std::basic_string_view<T>::npos;
};


// operator ==
template<class T, class Traits>
bool operator==(const base_mutable_string_view<T, Traits> &__lhs, const base_mutable_string_view<T, Traits> &__rhs) noexcept {
    if ( __lhs.size() != __rhs.size()) return false;
    return __lhs.compare(__rhs) == 0;
}

template<class T, class Traits>
bool operator==(const base_mutable_string_view<T, Traits> &__lhs, typename std::common_type<base_mutable_string_view<T, Traits> >::type __rhs) noexcept {
    if ( __lhs.size() != __rhs.size()) return false;
    return __lhs.compare(__rhs) == 0;
}

template<class T, class Traits> bool operator==(typename std::common_type<base_mutable_string_view<T, Traits> >::type __lhs, base_mutable_string_view<T, Traits> __rhs) noexcept {
    if ( __lhs.size() != __rhs.size()) return false;
    return __lhs.compare(__rhs) == 0;
}


// Stream IO
template<class T, class Traits> std::basic_ostream<T, Traits>&
operator<<(std::basic_ostream<T, Traits>& _stream, const base_mutable_string_view<T, Traits> &msv) {
    return std::__put_character_sequence(_stream, msv.data(), msv.size());
}

typedef base_mutable_string_view<char> mutable_string_view;
typedef base_mutable_string_view<wchar_t> mutable_wstring_view;


template<class T, class Traits>
struct std::hash<base_mutable_string_view<T, Traits>> : public unary_function<base_mutable_string_view<T, Traits>, size_t>
{
    size_t operator()(const base_mutable_string_view<T, Traits>& msv) const noexcept {
        return __do_string_hash(msv.data(), msv.data() + msv.size());
    };
};

#endif
