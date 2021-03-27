#ifndef F8484939_JFJDKE_FUEUEIJRDN_PEOR
#define F8484939_JFJDKE_FUEUEIJRDN_PEOR
#include <iterator>
#include <memory>
#include <utility>
namespace ilib {
namespace traits {
template <class>
class iterator;
}
namespace details {
template <class T>
struct enumerator_iter {
    T iter;
    std::size_t count{1};
    using value_type = typename std::iterator_traits<T>::value_type;
    using difference_type = typename std::iterator_traits<T>::difference_type;
    using pointer = typename std::iterator_traits<T>::pointer;
    using reference = typename std::iterator_traits<T>::reference;
    using iterator_category =
        typename std::iterator_traits<T>::iterator_category;

    enumerator_iter(const T& it) noexcept : iter(it) {}
    constexpr auto operator*() const
        -> std::pair<std::size_t, reference> requires requires {
        *iter;
    }
    { return {count, *iter}; }
    constexpr auto operator->() const -> pointer requires requires {
        iter.operator->();
    }
    { return std::to_address(iter); }
    constexpr auto operator++() -> enumerator_iter& requires requires {
        ++iter;
    }
    {
        ++count;
        ++iter;
        return *this;
    }
    constexpr auto operator++(int) -> enumerator_iter requires requires {
        iter++;
    }
    {
        auto temp = *this;
        count++;
        iter++;
        return temp;
    }
    constexpr auto operator+=(difference_type n)
        -> enumerator_iter& requires requires {
        iter += n;
    }
    {
        count += n;
        iter += n;
    }
    constexpr auto operator--() -> enumerator_iter& requires requires {
        --iter;
    }
    {
        if (count == 0)
            count = 0;
        else
            --count;
        --iter;
        return *this;
    }
    constexpr auto operator--(int) -> enumerator_iter requires requires {
        iter--;
    }
    {
        auto temp = *this;
        if (count == 0)
            count = 0;
        else
            count--;
        iter--;
        return temp;
    }
    constexpr auto operator-=(difference_type n)
        -> enumerator_iter& requires requires {
        iter -= n;
    }
    {
        if (n >= count)
            count = 0;
        else
            count -= n;
        iter -= n;
    }
    constexpr bool operator!=(const enumerator_iter& rhs) const noexcept
        requires requires {
        iter != iter;
    }
    { return (iter != rhs.iter); }
    constexpr bool operator==(const enumerator_iter& rhs) const noexcept
        requires requires {
        iter == iter;
    }
    { return (iter == rhs.iter); }
    constexpr auto friend operator+(const enumerator_iter& lhs,
                                    difference_type n) -> enumerator_iter
        requires requires {
        iter + n;
    }
    {
        auto it = lhs;
        lhs.count + n;
        lhs.iter + n;
        return it;
    }

    constexpr auto friend operator-(const enumerator_iter& lhs,
                                    difference_type n) -> enumerator_iter
        requires requires {
        iter - n;
    }
    {
        auto it = lhs;
        if (n > lhs.count)
            lhs.count = 0;
        else
            lhs.count - n;
        lhs.iter - n;
        return it;
    }
    constexpr auto friend operator+(difference_type n,
                                    const enumerator_iter& rhs)
        -> enumerator_iter requires requires {
        n + iter;
    }
    { return rhs + n; }

    constexpr auto friend operator-(difference_type n,
                                    const enumerator_iter& rhs)
        -> enumerator_iter requires requires {
        n - iter;
    }
    { return rhs - n; }
};
}  // namespace details

template <class It>
class enumerator {
    It m_Beg;
    It m_End;

   public:
    enumerator(It beg, It end) : m_Beg(beg), m_End(end) {}
    auto begin() -> details::enumerator_iter<It> {
        return details::enumerator_iter<It>(m_Beg);
    }
    auto end() -> details::enumerator_iter<It> {
        return details::enumerator_iter<It>(m_End);
    }
};
template <class T>
enumerator(T b, T e) -> enumerator<T>;
}  // namespace ilib

#endif
