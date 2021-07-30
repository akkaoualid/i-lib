#ifndef U63738930377_QyO7389399_63738399
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <ostream>
#include <type_traits>

namespace ilib {

namespace helpers {

template <class, class = void> struct is_input_iterator { constexpr static bool value = false; };

template <class T>
struct is_input_iterator<T, std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<T>::iterator_category,
                                                                   std::input_iterator_tag>>> {
    constexpr static bool value = true;
};

template <class T> constexpr bool is_input_iterator_v = is_input_iterator<T>::value;
} // namespace helpers

template <class C> struct char_traits;

template <> struct char_traits<char> {
    using char_type = char;
    using int_type = int;

    static constexpr void assign(char_type& dest, const char_type& src) noexcept { dest = src; }
    static constexpr char_type* assign(char_type* dest, std::size_t count, char_type src) noexcept {
        std::fill_n(dest, count, src);
        return dest;
    }

    static constexpr bool eq(char_type c1, char_type c2) noexcept { return c1 == c2; }
    static constexpr bool eq_int_type(int_type c1, int_type c2) noexcept { return c1 == c2; }
    static constexpr bool lt(char_type c1, char_type c2) noexcept { return c1 < c2; }

    static char_type* move(char_type* dest, const char_type* src, std::size_t count) noexcept {
        return static_cast<char_type*>(std::memmove(dest, src, count));
    }
    static constexpr char_type* copy(char_type* dest, const char_type* src, std::size_t count) noexcept {
        std::copy(src, src + count, dest);
        return dest;
    }
    static int_type compare(const char_type* s1, const char_type* s2, std::size_t count) noexcept {
        return std::memcmp(s1, s2, count);
    }

    static std::size_t length(const char_type* cs) noexcept { return std::strlen(cs); }

    static constexpr char_type to_char_type(int_type i) noexcept { return static_cast<char_type>(i); }
    static constexpr char_type to_int_type(char_type i) noexcept { return static_cast<int_type>(i); }

    static constexpr const char_type* find(const char_type* src, std::size_t count, char_type c) noexcept {
        for (std::size_t i = 0; i < count; ++i)
            if (src[i] == c)
                return src + i;
        return nullptr;
    }

    static constexpr int_type eof() noexcept { return EOF; }
    static constexpr bool not_eof(int_type i) noexcept { return i == EOF; }
};

template <class Ch, std::size_t N, class Traits = ilib::char_traits<Ch>> struct basic_static_string {

    using value_type = Ch;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using traits_type = Traits;

    using reference = value_type&;
    using const_reference = const value_type&;

    using pointer = value_type*;
    using const_pointer = const value_type*;

    using iterator = Ch*;
    using const_iterator = const Ch*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using iterator_category = std::random_access_iterator_tag;

    constexpr static size_type npos = std::numeric_limits<size_type>::max();

    basic_static_string(std::initializer_list<Ch> init) { assign(init); }

    template <class Iter, std::enable_if_t<helpers::is_input_iterator_v<Iter>>>
    constexpr basic_static_string(Iter first, Iter last) {
        assign(first, last);
    }

    basic_static_string(const_pointer str, size_type count) { assign(str, count); }

    basic_static_string(const_pointer str) { assign(str); }

    constexpr basic_static_string(size_type count, value_type ch) { assign(count, ch); }

    constexpr basic_static_string(const basic_static_string& str) { assign(str); }

    constexpr size_type size() const noexcept { return m_size; }
    constexpr size_type max_size() const noexcept { return N; }
    constexpr size_type capacity() const noexcept { return N; }

    constexpr iterator begin() noexcept { return data(); }
    constexpr iterator end() noexcept { return data() + size(); }
    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator{data() + size()}; }
    constexpr reverse_iterator rend() noexcept { return reverse_iterator{data()}; }

    constexpr const_iterator cbegin() const noexcept { return data(); }
    constexpr const_iterator cend() const noexcept { return data() + size(); }
    constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator{data() + size()}; }
    constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator{data()}; }

    constexpr pointer data() noexcept { return buffer; }
    constexpr const_pointer data() const noexcept { return buffer; }

    constexpr const_pointer c_str() const noexcept { return buffer; }

    constexpr basic_static_string& assign(size_type count, value_type ch) {
        if (count >= max_size())
            throw std::length_error("count >= max_size");
        Traits::assign(buffer, count, ch);
        m_size = count;
        return add_term();
    }
    template <class Iter>
    constexpr std::enable_if_t<helpers::is_input_iterator_v<Iter>, basic_static_string<Ch, N>>& assign(Iter first,
                                                                                                       Iter last) {
        for (std::size_t i = 0; first != last; ++first, ++i) {
            if (i >= max_size()) {
                add_term();
                throw std::length_error("[first, last) >= max_size");
            }
            buffer[i] = *first;
            ++m_size;
        }
        return add_term();
    }
    basic_static_string& assign(std::initializer_list<Ch> init) { assign(init.begin(), init.end()); }

    constexpr basic_static_string& assign(const basic_static_string& str) {
        assign(str.cbegin(), str.cend());
        return add_term();
    }
    basic_static_string& assign(const_pointer str, size_type count) {
        if (count >= max_size())
            throw std::length_error("count >= max_size");
        Traits::copy(buffer, str, count);
        m_size = count;
        return add_term();
    }
    basic_static_string& assign(const_pointer str) {
        assign(str, Traits::length(str));
        return add_term();
    }

    constexpr reference operator[](size_type i) noexcept { return buffer[i]; }
    constexpr const_reference operator[](size_type i) const noexcept { return buffer[i]; }

    constexpr bool operator==(const_pointer other) const noexcept {
        for (size_type i = 0; i < size(); ++i)
            if (!Traits::eq(at(i), other[i]))
                return false;
        return true;
    }
    constexpr bool operator==(const basic_static_string& other) const noexcept { return (*this) == other.c_str(); }

    constexpr reference at(size_type i) {
        if (i > size())
            throw std::range_error("out of bounds access to static_string");
        return buffer[i];
    }
    constexpr const_reference at(size_type i) const {
        if (i > size())
            throw std::range_error("out of bounds access to static_string");
        return buffer[i];
    }

    constexpr reference front() noexcept { return buffer[0]; }
    constexpr reference back() noexcept { return buffer[m_size - 1]; }

    constexpr const_reference front() const noexcept { return buffer[0]; }
    constexpr const_reference back() const noexcept { return buffer[m_size - 1]; }

    constexpr bool empty() const noexcept { return m_size == 0; }
    constexpr void clear() noexcept {
        while (m_size)
            pop_back();
    }

    constexpr void push_back(value_type ch) {
        if (m_size >= N)
            throw std::length_error("push_back: size > capacity");
        buffer[++m_size] = ch;
    }
    constexpr void pop_back() noexcept {
        if (empty())
            return;
        buffer[--m_size] = '\0';
    }

    constexpr basic_static_string& insert(size_type pos, size_type count, value_type ch) {
        if (size() + count >= max_size())
            throw std::length_error("insert: (size + count) >= max_size");
        if (pos > size())
            throw std::length_error("insert: pos > size");

        if (size() == pos) {
            m_size += count;
            for (std::size_t i = count; i < m_size; ++i) {
                buffer[i] = ch;
            }
            return add_term();
        }

        std::copy(data() + pos, data() + pos + count, data() + pos + count);
        for (std::size_t i = pos; i < count; ++i)
            buffer[i] = ch;
        m_size += count;
        return add_term();
    }
    constexpr basic_static_string& insert(size_type pos, const_pointer str, size_type count) {
        if (size() + count >= max_size())
            throw std::length_error("insert: (size + count) >= max_size");
        if (pos > size())
            throw std::length_error("insert: pos > size");

        if (size() == pos) {
            std::copy_n(str, count, end());
            m_size += count;
            return add_term();
        }

        std::copy(data() + pos, data() + pos + count, data() + pos + count);
        std::copy_n(str, count, data() + pos);
        m_size += count;
        return add_term();
    }
    constexpr basic_static_string& insert(size_type pos, const_pointer str) {
        return insert(pos, str, Traits::length(str));
    }
    template <class Iter>
    std::enable_if_t<helpers::is_input_iterator_v<Iter>, basic_static_string<Ch, N>>& insert(size_type pos, Iter first,
                                                                                             Iter last) {
        const std::size_t iter_distance = std::distance(first, last);
        if (pos > size())
            throw std::length_error("insert: pos > size");
        if (size() == pos) {
            std::copy(first, last, end());
            m_size += iter_distance;
            return add_term();
        }

        std::copy(data() + pos, data() + pos + iter_distance, data() + pos + iter_distance);
        std::copy(first, last, data() + pos);
        m_size += iter_distance;
        return add_term();
    }
    constexpr basic_static_string& insert(size_type pos, const basic_static_string& other) {
        return insert(pos, other.c_str());
    }
    constexpr basic_static_string& insert(size_type pos, const basic_static_string& str, size_type first,
                                          size_type last = npos) {
        const size_type size_ = last == npos ? str.size() : last;
        return insert(pos, str.c_str() + first, str.c_str() + size_);
    }

    constexpr basic_static_string& replace(size_type pos, size_type count, const basic_static_string& str) {
        if (pos > size())
            throw std::length_error("replace: pos > size");

        for (size_type i = pos; i < count; ++i) {
            if (i > size()) {
                add_term();
                throw std::out_of_range("replace: count > size");
            }
            buffer[i] = str[i];
            ++m_size;
        }
        return add_term();
    }
    constexpr basic_static_string& replace(const_iterator first, const_iterator last, const basic_static_string& str) {
        if (first < begin() || last > end())
            throw std::length_error("replace: first < begin() || last > end()");
        for (std::size_t i = 0; first != last; ++first, ++i) {
            *first = str[i];
            ++m_size;
        }
        return add_term();
    }

    constexpr size_type find(const basic_static_string& sub, size_type start = 0) const noexcept {
        auto out = std::search(cbegin() + start, cend(), sub.cbegin(), sub.cend());
        if (out == cend())
            return npos;
        return std::distance(cbegin(), out);
    }
    constexpr size_type find(const_pointer sub, size_type start = 0) const noexcept {
        auto out = std::search(cbegin() + start, cend(), sub, sub + Traits::length(sub));
        if (out == cend())
            return npos;
        return std::distance(cbegin(), out);
    }
    constexpr size_type find(value_type ch, size_type start = 0) const noexcept {
        for (; start < size(); ++start)
            if (Traits::eq(buffer[start], ch))
                return start;
        return npos;
    }

    constexpr size_type rfind(const basic_static_string& sub, size_type start = 0) const noexcept {
        auto out = std::search(cbegin() + start, cend(), sub.cbegin(), sub.cend());
        if (out == cend())
            return npos;
        return size() - sub.size() - std::distance(cbegin(), out) - 1;
    }
    constexpr size_type rfind(const_pointer sub, size_type start = 0) const noexcept {
        auto out = std::search(cbegin() + start, cend(), sub, sub + Traits::length(sub));
        if (out == cend())
            return npos;
        return size() - Traits::length(sub) - std::distance(cbegin(), out) - 1;
    }
    constexpr size_type rfind(value_type ch, size_type start = 0) const noexcept {
        for (; start <= size(); ++start)
            if (Traits::eq(buffer[size() - start], ch))
                return start;
        return npos;
    }

    constexpr basic_static_string& append(size_type count, value_type ch) { return insert(size(), count, ch); }
    constexpr basic_static_string& append(const_pointer str) { return insert(size(), str); }
    constexpr basic_static_string& append(const basic_static_string& str) { return append(str.c_str()); }

  private:
    constexpr basic_static_string& add_term() noexcept {
        buffer[m_size] = value_type{};
        return *this;
    }

    value_type buffer[N] = {};
    size_type m_size = 0;
};
template <std::size_t N> using static_string = basic_static_string<char, N>;

template <std::size_t N> std::ostream& operator<<(std::ostream& os, const ilib::static_string<N>& str) {
    return os << str.c_str();
}
template <class Ch, std::size_t N>
basic_static_string(const Ch (&)[N]) -> basic_static_string<typename ilib::char_traits<Ch>::value_type, N>;

template <class Ch, std::size_t N, class Traits>
basic_static_string(const basic_static_string<Ch, N, Traits>&) -> basic_static_string<Ch, N, Traits>;

} // namespace ilib
#endif
