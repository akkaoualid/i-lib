#ifndef U638392002938382772829203948393738_Z6373930030383838398_X7383920309393939
#define U638392002938382772829203948393738_Z6373930030383838398_X7383920309393939
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <ilib/utility.hpp>
#include <iterator>
#include <type_traits>

namespace ilib {
template <class T, std::size_t N> struct static_vector {
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;

    constexpr static_vector() noexcept {}

    template <class Iter> constexpr static_vector(Iter begin, Iter end) noexcept {
        for (; begin != end; ++begin)
            emplace_back(*begin);
    }
    constexpr static_vector(std::initializer_list<T> list) {
        ILIB_ASSERT(list.size() < N, "initializer_list size is too big");
        for (auto begin = list.begin(); begin != list.end(); ++begin)
            emplace_back(*begin);
    }

    constexpr static_vector(const static_vector& other) noexcept : static_vector(other.cbegin(), other.cend()) {}
    constexpr static_vector(static_vector&& other) noexcept {
        for (auto&& elm : other)
            emplace_back(std::move(elm));
    }

    constexpr iterator begin() noexcept { return data(); }
    constexpr iterator end() noexcept { return data() + m_size; }
    constexpr const_iterator cbegin() const noexcept { return data(); }
    constexpr const_iterator cend() const noexcept { return data() + m_size; }

    constexpr size_type size() const noexcept { return m_size; }
    constexpr size_type capacity() const noexcept { return N; }
    constexpr size_type max_size() const noexcept { return N; }
    constexpr void clear() noexcept {
        for (; m_size;)
            pop_back();
    }

    constexpr pointer data() noexcept { return std::launder(reinterpret_cast<T*>(m_data)); }
    constexpr const_pointer data() const noexcept { return std::launder(reinterpret_cast<const T*>(m_data)); }

    constexpr static_vector& operator=(const static_vector& other) noexcept {
        if (this == std::addressof(other))
            return *this;
        std::size_t s = std::min(size(), other.size());
        for (std::size_t i = 0; i < s; ++i)
            data()[i] = other[i];

        if (size() < other.size())
            for (std::size_t i = size(); i < other.size(); ++i)
                emplace_back(other[i]);
        else
            for (std::size_t i = other.size(); i < size(); ++i)
                pop_back();
        return *this;
    }
    constexpr static_vector& operator=(static_vector&& other) noexcept {
        if (this == std::addressof(other))
            return *this;
        std::size_t s = std::min(size(), other.size());
        for (std::size_t i = 0; i < s; ++i)
            data()[i] = std::move(other[i]);

        if (size() < other.size())
            for (std::size_t i = size(); i < other.size(); ++i)
                emplace_back(std::move(other[i]));
        else
            for (std::size_t i = other.size(); i < size(); ++i)
                pop_back();
        return *this;
    }

    constexpr T& operator[](size_type idx) { return *(data() + idx); }
    constexpr const T& operator[](size_type idx) const { return *(data() + idx); }

    T& at(size_type idx) {
        if (idx >= size())
            throw std::out_of_range("ilib::static_vector::at: invalid range");
        return *(data() + idx);
    }
    const T& at(size_type idx) const {
        if (idx >= size())
            throw std::out_of_range("ilib::static_vector::at: invalid range ");
        return *(data() + idx);
    }

    template <class... Args> void emplace_back(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>) {
        ILIB_ASSERT(size() < capacity(), "ilib::static_vector::emplace_back: maximum capacity exceeded");
        ::new (static_cast<void*>(std::addressof(m_data[m_size]))) T(std::forward<Args>(args)...);
        ++m_size;
    }
    constexpr void push_back(const T& value) noexcept(std::is_nothrow_constructible_v<T, T>) { emplace_back(value); }
    constexpr void pop_back() noexcept(std::is_nothrow_destructible_v<T>) {
        --m_size;
        std::destroy_at(std::addressof(m_data[m_size]));
    }

    constexpr const_reference front() const noexcept { return *data(); }
    constexpr reference front() noexcept { return *data(); }
    constexpr const_reference back() const noexcept { return *(data() + m_size); }
    constexpr reference back() noexcept { return *(data() + m_size); }

    friend void swap(static_vector& left, static_vector& right) noexcept {
        using std::swap;
        swap(left.m_size, right.m_size);
        swap(left.m_data, right.m_data);
    }

    ~static_vector() { clear(); }

  private:
    alignas(T) std::byte m_data[sizeof(T) * N];
    size_type m_size = 0;
};

} // namespace ilib
#endif
