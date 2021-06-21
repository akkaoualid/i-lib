#ifndef F8484939_JFJDKE_FUEUEIJRDN_PEOR
#define F8484939_JFJDKE_FUEUEIJRDN_PEOR
#include <iterator>
#include <memory>
#include <tuple>
#include <utility>
namespace ilib {
namespace details {
template <class T>
struct enumerator_iter {
    T iter;
    std::size_t count{1};
    using value_type = typename std::iterator_traits<T>::value_type;
    using difference_type = typename std::iterator_traits<T>::difference_type;
    using pointer = typename std::iterator_traits<T>::pointer;
    using reference = typename std::iterator_traits<T>::reference;
    using iterator_category = typename std::iterator_traits<T>::iterator_category;

    enumerator_iter(const T& it) noexcept : iter(it) {}
    constexpr auto operator*() const -> std::pair<std::size_t, reference>
    requires requires {
        *iter;
    }
    { return {count, *iter}; }
    constexpr auto operator->() const -> pointer {
        return std::to_address(iter);
    }
    constexpr auto operator++() -> enumerator_iter& {
        ++count;
        ++iter;
        return *this;
    }
    constexpr auto operator++(int) -> enumerator_iter {
        auto temp = *this;
        count++;
        iter++;
        return temp;
    }
    constexpr auto operator+=(difference_type n) -> enumerator_iter& {
        count += n;
        iter += n;
    }
    constexpr auto operator--() -> enumerator_iter& {
        if (count == 0)
            count = 0;
        else
            --count;
        --iter;
        return *this;
    }
    constexpr auto operator--(int) -> enumerator_iter {
        auto temp = *this;
        if (count == 0)
            count = 0;
        else
            count--;
        iter--;
        return temp;
    }
    constexpr auto operator-=(difference_type n) -> enumerator_iter& requires requires {
        iter -= n;
    }
    {
        if (n >= count)
            count = 0;
        else
            count -= n;
        iter -= n;
    }
    constexpr bool operator!=(const enumerator_iter& rhs) const noexcept requires requires {
        iter != iter;
    }
    { return (iter != rhs.iter); }
    constexpr bool operator==(const enumerator_iter& rhs) const noexcept requires requires {
        iter == iter;
    }
    { return (iter == rhs.iter); }
    constexpr auto friend operator+(const enumerator_iter& lhs, difference_type n)
        -> enumerator_iter requires requires {
        iter + n;
    }
    {
        auto it = lhs;
        lhs.count + n;
        lhs.iter + n;
        return it;
    }

    constexpr auto friend operator-(const enumerator_iter& lhs, difference_type n)
        -> enumerator_iter requires requires {
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
    constexpr auto friend operator+(difference_type n, const enumerator_iter& rhs)
        -> enumerator_iter requires requires {
        n + iter;
    }
    { return rhs + n; }

    constexpr auto friend operator-(difference_type n, const enumerator_iter& rhs)
        -> enumerator_iter requires requires {
        n - iter;
    }
    { return rhs - n; }
};
template <class... A>
struct zip_iterator {
    using value_type = std::tuple<typename std::iterator_traits<std::remove_cvref_t<A>>::value_type...>;

    zip_iterator(A... args) : iters{std::forward<A>(args)...} {}

    constexpr value_type operator*() noexcept {
        return std::apply(
            [](auto&&... args) constexpr { return value_type(*args...); }, iters);
    }

    constexpr zip_iterator& operator++() noexcept {
        std::apply(
            [](auto&... args) constexpr { ((args += 1), ...); }, iters);
        return *this;
    }

    constexpr zip_iterator operator++(int) noexcept {
        auto t = *this;
        ++*this;
        return t;
    }

    constexpr const zip_iterator& operator--() noexcept {
        std::apply(
            [](auto&... args) constexpr { ((args -= 1), ...); }, iters);
        return *this;
    }

    constexpr zip_iterator operator--(int) noexcept {
        auto t = *this;
        --*this;
        return t;
    }

    constexpr bool operator==(const zip_iterator& other) noexcept {
        return tpl_equals(iters, other.iters, std::make_index_sequence<sizeof...(A)>());
    }

    constexpr bool operator!=(const zip_iterator& other) noexcept {
        return !(*this == other);
    }

   private:
    template <std::size_t... Idx>
    constexpr bool tpl_equals(std::tuple<A...> left, std::tuple<A...> right, std::index_sequence<Idx...>) noexcept {
        return (... || (std::get<Idx>(left) == std::get<Idx>(right)));
    }
    std::tuple<A...> iters;
};
template <class... A>
zip_iterator(A...) -> zip_iterator<A...>;
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
template <class... It>
struct zipper {
    constexpr zipper(It... args) noexcept : iters{std::forward<It>(args)...} {}

    constexpr auto begin() noexcept {
        return std::apply([](auto&&... args) { return ::ilib::details::zip_iterator(std::begin(args)...); }, iters);
    }

    constexpr auto end() noexcept {
        return std::apply([](auto&&... args) { return ::ilib::details::zip_iterator(std::end(args)...); }, iters);
    }

   private:
    std::tuple<It...> iters;
};
template <class T>
enumerator(T, T) -> enumerator<T>;

template <class... A>
zipper(A...) -> zipper<A...>;

template <class Range>
auto enumerate(Range rng) {
    using std::begin, std::end;
    return enumerator(begin(rng), end(rng));
}
template <class... It>
constexpr zipper<It...> zip(It... it) noexcept {
    return zipper<It...>(std::forward<It>(it)...);
}
}  // namespace ilib
#endif
