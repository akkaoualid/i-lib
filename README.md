<p align="center">
   <img width=128 height=128 src="./logo.png">
</p>

# i-lib
A general utilities library made for fun (still WIP)

![c++](https://img.shields.io/badge/-C++20-00599C?style=flat-square&logo=c%2B%2B)
[![documentation](https://img.shields.io/badge/-documentation-00599C?style=flat-square])](https://dammi-i.github.io/i-lib)

# TODO:
- [ ] ilib::merge
- [ ] ilib::tuple_iterator
- [ ] ilib::static_string
- [ ] ilib::static_map
- [ ] ilib::static_stack
- [ ] ilib::static_queue
- [ ] ilib::pretty_range
- [ ] ilib::range, `ilib::range(_, 10);`, `ilib::range(10, _);`
- [ ] overload `[]` operator to support slice i.e `container[{3,5}]`, `container[{_, 60}]`, `container[{1, _}]`
- [ ] ilib::slice_range
- [ ] ilib::async_generator
- [ ] ilib::coroutine, supporting all of coroutine types, task, generator, awaitable
- [ ] make ilib::generator suitable with [P2168]
- [ ] fix ilib::task, ilib::generator
- [ ] ilib::units::radian, ilib::units::degrees
- [ ] math functions
- [ ] ilib::pretty
- [ ] change ilib::Result to ilib::result and not a bloated copy paste of rust
- [ ] change ilib::Option to ilib::optional just like a C++ interface
- [ ] support for async operations on ilib::result and ilib::optional
- [ ] add ilib::json
- [ ] functional:
   - [ ] ilib::compose
   - [ ] ilib::compose_with
   - [ ] ilib::functor
   - [ ] support for binary operators such as `|` for compose, `>>` call the function and takes its result then apply to right function,

- [ ] ilib::numbers for constant numbers such as pi, tau, etc
- [ ] ilib::format
