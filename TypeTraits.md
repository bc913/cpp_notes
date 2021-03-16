# Type Traits
## Introduction
Generic programming is a technique to generalize certain components of a software system by decreasing the dependency to the type information. This also brings an advantage on code resuablity and maintainance.

However, sometimes, the implementation details might differ for specific types within the same algorithm/class. Also, some types can be better handled in terms of efficiency. Therefore, more information is required for those types.

This is where **Traits** come into the picture. The *Traits* are mechanisms that can carry or transform specific informations about types. This is a compile-time mechanism which eliminates all run-time complexities and error handlings.

To get this mechanism, **#include <type_traits>**.

## How does it work?
Basically, it is achieved by partial or full specialization of a generic struct which is designed to carry a specific information about types. **Type Traits** are used in two ways:
-   Carrying type information
-   Changing type information

Before jumping into the details, the definition of *integral_constant* is crucial. This trait is mostly used as base class for the value-based type traits [1].

```cpp
template <typename T, T val>
struct integral_constant
{
    using value_type = T;
    using type = integral_constant;
    static constexpr T value = val;
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;
```
> Each compiler vendors have different implementations for type traits. The approach presented here is only one of them, NOT the only one.

### Carrying type information
Type traits classes can provide an answer for the following questions or more:
- is a type void?
- is type A same of type B?
- is a type pointer or reference?
- is a type floating point?

etc. It is always better to explain through the code.

```cpp
// None of the types are void
template<typename T>
struct is_void : public false_type{};
// except the void itself
// full specialization of is_void<T>
template<>
struct is_void<void> : public true_type{};
```

```cpp
// Two given types are always different
template <typename T, typename U>
struct is_same : public false_type{};
// Except the given types are the same
template<typename T>
struct is_same<T, T> : public true_type{};
```

```cpp
// std::is_pointer
template<typename T>
struct is_pointer : public false_type{};

template<typename T>
struct is_pointer<T*> : public true_type{};

// std::is_reference
template<typename T>
struct is_reference : public false_type{};
template<typename T>
using is_reference_v = typename is_reference<T>::value;

template<typename T>
struct is_reference<T&> : public true_type{};
template<typename T>
using is_reference_v = typename is_reference<T&>::value;

template<typename T>
struct is_reference<T&&> : public true_type{};
template<typename T>
using is_reference_v = typename is_reference<T&&>::value;
```

```cpp
template<typename T>
using is_float = typename is_same<T, float>;
// OR
template<typename T>
struct is_float
{
    static constexpr bool value = is_same<T, float>::value;
};
// OR
template <typename T>
struct is_float : public false_type{};
template<>
struct is_float<float> : public true_type{};
template<>
struct is_float<double> : public true_type{};
template<>
struct is_float<long double> : public true_type{};
```
#### How can they be used?
*Type Traits* mechanism provides great flexibility during an algorithm/data structure design when some type specialization is required for efficiency. The very typical example for 
the usage is **Optimized Copy** in the STL [1]. In general, default implementation of the copy algorithm works well for the most of the types. However, some types can be copied in a more efficient way using *memcopy* and this puts some requirements on the types. These requirements can only be checked with **Type Traits**.

A simple example can be found at [3]. A bit more complex example is discussed at [2].


### Changing type information (Type transformation)
**Type Traits** can also be used to transform the type by changing its information. The standard library provides several of them.
- Removing const qualifier from a type
- Add/remove reference
- Add lvalue reference

```cpp
// Remove const from a given type
template<typename T>
struct remove_const {using type = T;};
template<typename T>
struct remove_const<const T> {using type = T;};
```

```cpp
template<typename T>
struct remove_reference {using type = T;};
template<typename T>
struct remove_reference<T&> {using type = T;};
template<typename T>
struct remove_reference<T&&> {using type = T;};
```

```cpp
// This implementation is not the best but still works
template<typename...>
struct void_t{};
// void& and void&& is not allowed so leave as is
template<typename T, typename = void>
struct add_lvalue_reference {using type = T;};
// for all the types other then void (SFINAE)
template<typename T>
struct add_lvalue_reference<T, void_t<T&> > {using type = T&;};
```

**Each class that performs type transformation holds a alias (or typedef) member *type* which yields to the result of the transformation.**

## Resources
1.  [Boost libraries](https://www.boost.org/doc/libs/1_63_0/libs/type_traits/doc/html/index.html)
2. [C++ Type Traits - Dr.Dobbs](https://www.drdobbs.com/cpp/c-type-traits/184404270?pgno=1)
3. [A quick primer on type traits in modern C++](https://www.internalpointers.com/post/quick-primer-type-traits-modern-cpp)
4. [How to Implement std::declval, add_lvalue_reference, add_rvalue_reference, std::void_t](https://www.youtube.com/watch?v=hDKftGHTJk8)
5. [An introduction to C++ Traits](https://accu.org/journals/overload/9/43/frogley_442/)
6. [C++ short stories: type traits, concepts, and type constraints](https://www.jakubkonka.com/2017/09/02/type-traits-cpp.html)
7. [Simplify your type traits with C++14 variable templates](https://baptiste-wicht.com/posts/2017/08/simplify-your-type-traits-with-c%2B%2B14-variable-templates.html)
8. [Using C++ Trait Classes for Scientific Computing](http://www.cs.rpi.edu/~musser/design/blitz/traits.html)
9. [Effective Modern C++ - Scott Meyers]


