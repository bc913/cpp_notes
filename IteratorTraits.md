# Iterator Traits
## Introduction
**Iterator Traits** provides compile-time information about itself and the type of the object that it points to. This mechanism provides flexibility and compile-time security for generic programming and/or template metaprogramming.

This is really benefical for the development of an algorithm which operates on iterators. Different implementations can be applied to different types of iterators and this is possible with **Iterator Traits**. Current *STL* implementations work based on this mechanism.

## How it works?
`Iterator Traits` work hand in hand with `iterator`s and requires a specific API from the subjected iterator to gain and provide type information. This API for the iterator is NOT a language or syntax requirement but useful for the traits.

```cpp
template<typename T, typename Category, typename Dist = std::ptrdiff_t, typename Ptr = T*, typename Ref = T&>
struct iterator
{
    using value_type        = T;
    using iterator_category = Category;
    using difference_type   = Dist;
    using pointer           = Ptr;
    using reference         = Ref;
};
```
As of C++17, following categories are avaliable:
```cpp
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag : public input_iterator_tag{};
struct bidirectional_iterator_tag : public forward_iterator_tag{};
struct random_access_iterator_tag :  public bidirectional_iterator_tag{};
```

These tags are the most critical pieces for algorithms in order to carry information for efficient algorithms based on their types.

```cpp
// Implementations might vary from compiler to compiler
template<typename Iter>
struct iterator_traits
{
    using value_type        = typename Iter::value_type;
    using iterator_category = typename Iter::iterator_category;
    using difference_type   = typename Iter::difference_type;
    using pointer           = typename Iter::pointer;
    using reference         = typename Iter::reference;
};

// Specialization for pointers
template<typename T>
struct iterator_traits<T*>
{
    using value_type        = T;
    using iterator_category = std::random_access_iterator_tag;
    using difference_type   = ptrdiff_t;
    using pointer           = T*;
    using reference         = T&;
};
// Specialization for const pointers
template<typename T>
struct iterator_traits<const T*>
{
    using value_type        = T;
    using iterator_category = std::random_access_iterator_tag;
    using difference_type   = ptrdiff_t;
    using pointer           = const T*;
    using reference         = const T&;
};

```
## How do we use?
Using `iterator_traits` bring the flexibility of switching/choosing the default/efficient implementation of an algorithm in compile-time.

### Switch the default implementation
```cpp
namespace impl
{
    template<typename FwdIter, typename Cmp>
    void bubble_sort(FwdIter beg, FwdIter end, Cmp comp)
    {
        for(FwdIter i = beg; i != end; ++i)
            for(FwdIter j = beg; j < i; ++j)
                if(!comp(*j, *i))
                    std::iter_swap(i, j);
    }
}

template<typename FwdIter>
void bubble_sort(FwdIter beg, FwdIter end)
{
    impl::bubble_sort(beg, end, std::less<typename std::iterator_traits<FwdIter>::value_type>());
}

template <typename FwdIter, typename Cmp>
void bubble_sort(FwdIter beg, FwdIter end, Cmp comp)
{
    impl::bubble_sort(beg, end, comp);
}
```

### Choose the efficient implementation
```cpp
namespace impl
{
    template<typename RanIter>
    void func(RanIter beg, RanIter end, std::random_access_iterator_tag)
    {

    }

    template<typename FwdIter>
    void func(FwdIter beg, FwdIter end, std::forward_iterator_tag)
    {

    }
}

template<typename Iter>
void func(Iter beg, Iter end)
{
    impl::func(beg, end, typename std::iterator_traits<Iter>::iterator_category);
}
```

## Resources
[An introduction to Iterator Traits](https://www.codeproject.com/Articles/36530/An-Introduction-to-Iterator-Traits)

[std::iterator is deprecated: Why, What It Was, and What to Use Instead](https://www.fluentcpp.com/2018/05/08/std-iterator-deprecated/)

[code.woboq.org](https://code.woboq.org/gcc/libstdc++-v3/include/bits/stl_iterator_base_types.h.html)

[boost.org](https://www.boost.org/sgi/stl/iterator_traits.html)

[cppreference.com](https://en.cppreference.com/w/)

[cplusplus.com](http://cplusplus.com/)



