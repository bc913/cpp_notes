# Idioms

## (A possible) Generic implementation for flags

First make sure that the `underlying_type` of the enum is unsigned integer.

```cpp
enum class MyFlags : unsigned int {
    None = 0,
    Flag1 = 1 << 0,  // 0001
    Flag2 = 1 << 1,  // 0010
    Flag3 = 1 << 2,  // 0100
    Flag4 = 1 << 3   // 1000
};
```

Using `<type_traits>` might provide a solution to achieve a generic implementation for flag enums.

- Solution 1:

```cpp
#include <type_traits>

template<typename T, typename ULT = unsigned int>
struct is_flag_type : std::integral_constant<bool, (std::is_enum_v<T>) && (std::is_same_v<std::underlying_type_t<T>, ULT>)> {};

template<typename T, typename ULT = unsigned int, std::enable_if_t<is_flag_type<T,ULT>::value, bool> = true>
inline T operator|(T lhs, T rhs) {
    return static_cast<T>(static_cast<ULT>(lhs) | static_cast<ULT>(rhs));
}

template<typename T, typename ULT = unsigned int, std::enable_if_t<is_flag_type<T,ULT>::value, bool> = true>
inline T operator&(T lhs, T rhs) {
    return static_cast<T>(static_cast<ULT>(lhs) & static_cast<ULT>(rhs));
}

template<typename T, typename ULT = unsigned int, std::enable_if_t<is_flag_type<T,ULT>::value, bool> = true>
inline T operator~(T rhs) {
    return static_cast<T>(~static_cast<ULT>(rhs));
}

template<typename T, typename ULT = unsigned int, std::enable_if_t<is_flag_type<T,ULT>::value, bool> = true>
inline T& operator|=(T& lhs, T rhs) {
    lhs = lhs | rhs;
    return lhs;
}

template<typename T, typename ULT = unsigned int, std::enable_if_t<is_flag_type<T,ULT>::value, bool> = true>
inline T& operator&=(T& lhs, T rhs) {
    lhs = lhs & rhs;
    return lhs;
}

//=====================
template<typename T, typename ULT = unsigned int, std::enable_if_t<is_flag_type<T,ULT>::value, bool> = true>
inline T& SetFlag(T& lhs, T rhs)
{
    return lhs |= rhs;
}

template<typename T, typename ULT = unsigned int, std::enable_if_t<is_flag_type<T,ULT>::value, bool> = true>
inline T& UnSetFlag(T& lhs, T rhs)
{
    return lhs &= ~rhs;
}

template<typename T, typename ULT = unsigned int, std::enable_if_t<is_flag_type<T,ULT>::value, bool> = true>
inline bool IsFlagSet(T flag, T check)
{
    return (flag & check) == check;
}

```

- Solution 2 (A bit cleaner):

```cpp
template<typename T>
struct is_flag_type2 : std::integral_constant<bool, (std::is_enum_v<T>) && (std::is_unsigned_v<std::underlying_type_t<T>>)> {};


template<typename T, std::enable_if_t<is_flag_type2<T>::value, bool> = true>
inline T operator|(T lhs, T rhs) {
    return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) | static_cast<std::underlying_type_t<T>>(rhs));
}

template<typename T, std::enable_if_t<is_flag_type2<T>::value, bool> = true>
inline T operator&(T lhs, T rhs) {
    return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) & static_cast<std::underlying_type_t<T>>(rhs));
}

template<typename T, std::enable_if_t<is_flag_type2<T>::value, bool> = true>
inline T operator~(T rhs) {
    return static_cast<T>(~static_cast<std::underlying_type_t<T>>(rhs));
}

template<typename T, std::enable_if_t<is_flag_type2<T>::value, bool> = true>
inline T& operator|=(T& lhs, T rhs) {
    lhs = lhs | rhs;
    return lhs;
}

template<typename T, std::enable_if_t<is_flag_type2<T>::value, bool> = true>
inline T& operator&=(T& lhs, T rhs) {
    lhs = lhs & rhs;
    return lhs;
}
//=====================
template<typename T, std::enable_if_t<is_flag_type2<T>::value, bool> = true>
inline T& SetFlag(T& lhs, T rhs)
{
    return lhs |= rhs;
}

template<typename T, std::enable_if_t<is_flag_type2<T>::value, bool> = true>
inline T& UnSetFlag(T& lhs, T rhs)
{
    return lhs &= ~rhs;
}

template<typename T, std::enable_if_t<is_flag_type2<T>::value, bool> = true>
inline bool IsFlagSet(T flag, T check)
{
    return (flag & check) == check;
}
```

- Usage:

```cpp
MyFlags flags = MyFlags::None;
SetFlag<MyFlags>(flags, MyFlags::Flag1);
UnSetFlag<MyFlags>(flags, MyFlags::Flag1);
auto res = IsFlagSet<MyFlags>(flags, MyFlags::Flag2);
```
