#ifndef H7393030300_D7393930020200_R739202030030393
#define H7393030300_D7393930020200_R739202030030393
#include <cstdio>
#include <iterator>
#include <type_traits>
/*
 * required aliases for containers
 */
#define ILIB_TEMPLATE(...) template <__VA_ARGS__>

#define ILIB_CONTAINER_BEGIN(name, T)            \
    struct name {                                \
        using value_type = std::remove_cv_t<T>;  \
        using reference = value_type&;           \
        using const_reference = const reference; \
        using size_type = unsigned long;         \
        using difference_type = std::ptrdiff_t;

#define ILIB_ALLOCATOR(allocator)                                            \
    using allocator_type = allocator;                                        \
    using pointer = typename std::allocator_trairs<allocator_type>::pointer; \
    using const_pointer =                                                    \
        typename std::allocator_trairs<allocator_type>::const_pointer;

#define ILIB_ITERATOR(IT)                                     \
    using iterator = IT;                                      \
    using const_iterator = const IT;                          \
    using reverse_iterator = std::reverse_iterator<iterator>; \
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

#define ILIB_CONTAINER_END() \
    }                        \
    ;
#endif
