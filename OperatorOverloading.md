# Operator Overloading
User-defined typed objects can be operand of an operator using `Operator Overloading` mechanism. Following operators can be overloaded.

`+ - * / % ^ & | ~ ! = < > += -= *= /= %= ^= &= |= << >> >>= <<= == != <= >= <=> (since C++20) && || ++ -- , ->* -> ( ) [ ]`

### Rules:
- Operator overload functions can be defined as `member` or `non-member` with some exceptions.
- If it is defined as `member` function, the first operand is implicitly bound to this pointer.
- Do not overload `&&` , `||` and `,` operators because they are losing proper order of evaluation and/or short circuit behavior when overloaded.
- `[]`, `->`, `()` and `=` operator overloads should be `member` functions.

## Arithmetic and arithmetic assignment operator
Arithmetic operator overloads `operator@()` do not change the state of the operands so they have to return an object with updated state by value. Arithmetic assignment operator overloads operate on and change the state of the operands so the it should return reference.

**Good Practice:** Define `operator@()` as `non-member` in terms of `operator@=()` (`non-member` is preffered) [Item27, C++ Coding Standards]. (@ = `+`, `-`, `*` or `/`)

```cpp
// increment
// member
array_iterator& array_iterator::operator+=(const ptrdiff_t offset)
{
    // Test state if required
    ptr_ += offset;
    return *this;
}

// non-member
// pass-by-value provides optimization opportunity to the compiler
array_iterator operator+(const ptrdiff_t offset, array_iterator rhs)
{
    return rhs += offset;
}

array_iterator operator+(array_iterator lhs, const ptrdiff_t offset)
{
    return lhs += offset;
}

// decrement
// member
array_iterator& array_iterator::operator-=(const ptrdiff_t offset)
{
    // Test state
    return *this += -offset;
}

// non-member
array_iterator operator-(array_iterator lhs, const ptrdiff_t offset)
{
    return lhs -= offset;
}
```

## Increment/decrement operators
`++` and `--` operator can be in prefix and postfix forms.
Since they change the value of the operand object, prefer to define them as `member` functions. Overloads for postfix operators should have one argument of type `int`.

**Good Practice:** Define postfix operator overloads in terms of their prefix counterparts.

```cpp
//prefix
array_iterator& operator++()
{
    // Test the state if required
    ++ptr_;
    return *this;
}

array_iterator& operator--()
{
    // Test the state if required
    --ptr_;
    return *this;
}

```

```cpp
// postfix
// they can be non-member
array_iterator array_iterator::operator++(int)
{
    auto temp(*this);
    operator++();
    return temp;
}

array_iterator array_iterator::operator--(int)
{
    auto temp(*this);
    operator--();
    return temp;
}
```

## Array subscript operator
It provides read/write access so can be defined in two versions (const and non-const). If `T` is value type, const version can return by value. Should be defined as `member`.

```cpp
template <typename T>
struct MyVec
{
public:
    T& operator[](size_t index){ return vec_[index]; }
    const T& operator[](size_t index) const { return vec_[index];}
private:
    std::vector vec_;
};

```
## Arrow and dereferencing operators
These are typically used by pointer-like classes and iterators. Const and non-const versions should be provided. Try to implement one in terms of the other.

Arrow operator should return to either a pointer to class or an object of a class with its operator->() overloaded. [C++ PRimer, sec. 14.7]
```cpp
template<typename T>
class MyPtr
{
public:
    T* operator->() { return ptr_; }
    const T* operator->() const { return ptr_;}

    T& operator*() { return *operator->();}
    const T& operator*() const { return *operator->();}
private:
    T* ptr_;
};
```

## Comparison operators
No side effect should be recorded. (The values of the operands should not be changed during the operation.)

**Good Practice:** define as many comparison operator overload function as possible in terms of another comparison operator overload function.
- This provides better coupling within the class and easier maintanence.

This can be defined either `member` or `non-member` functions. Since `non-member` versions do not contain `this` implicitly, two arguments should be provided.

```cpp
inline bool operator<(const array_iterator & rhs) const
{
    return ptr_ < rhs.ptr_;
}

// define others in terms of operatoir <
inline bool operator>(const array_iterator & rhs) const
{
    return rhs < *this;
}

inline bool operator<=(const array_iterator& rhs) const
{
    return !(rhs < *this);
}

inline bool operator>=(const array_iterator& rhs) const
{
    return !(*this < rhs);
}
```

## Equality operators
This can be implemented as either `member` or `non-member`. Usually, `!=` overload is implemented in terms of `==`.

```cpp
inline bool operator==(const array_iterator & rhs) const { return ptr_ == rhs.ptr_;}
inline bool operator!=(const array_iterator& rhs) const { return !(*this == rhs);}
```

## Stream operators
Since the first argument is global stream object, this overloads should be implemented as `non-member` functions.

**Good Practice:** Provide a convenience `member` function (instead of `friend` decleration )to be used by the overload especially when private non-static data members need to be handled in the stream logic.

**Good Practice:** Output stream operator overload do minimal formatting. Do not provide new line in this overload since the user would not expect a new line during a stream.

```cpp
template<typename T>
std::ostream & operator<<(std::ostream & os, const myClass<T> & obj)
{
    return os << obj.to_string();
} 

template<typename T>
std::istream & operator>>(std::istream & is, myClass<T>& obj)
{
    // read the input from the stream if necessary
    int d;
    is >> d;
    // call a setter function on obj
    obj.set_something(d);

    return is;
}
```
## Function Call operators
Should be defined as `member` function.
```cpp

class MyClass
{
    public:
        double operator()(double x, double y) { return x * y;}
};
```

## Conversion operator overload
This is used for conversion from the class type to some other type. It should be defined as `member` function and it is usually const. No return type decleration is required but it has to return to the desired type.

```cpp

operator <type_to_convert_to> () const;

```
**Rule**: Conversion to an array or function type or void is not allowed. It can not take any argument.
## Misc
### Good Practices
- Prefer defining operator overload functions as `non-member`. If it has need to access private non-static members, define them as `member` functions.
- Do not prefer using `friend` approach for operator overloading functions such as other functions. That still breaks encapsulation.
- If any type conversion is applied (or desired) to the both operands of an operand, define the operator overload as `non-member` function. [Meyers - Effective C++, Item 24 & C++ Coding Standards, Item27]
- One case justifies the usage of `friend` for template classes. [Meyers - Effective C++, Item 46]. If a class template offers functions related to that template that support implicit type conversions for all types, i.e. `operator*()`,
    1. Declare them as non-member
    2. also define them within class as `friend`.
    3. Let that definition call a helper function

```cpp
// myclass.h
template<typename T>
class myClass
{
public:
    /**/
    friend const myClass<T> operator*(const myClass<T>& lhs, const myClass<T>& rhs)
    {
        return doMultiply(lhs, rhs);
    }
};

const myClass<T> operator*(const myClass<T>& lhs, const myClass<T>& rhs);
const myClass<T> doMultiply(const myClass<T>& lhs, const myClass<T>& rhs)
{
    // Do stuff and return
}

```

### Overloading enums
`Enum`s can also be overloaded with `non-member` functions.
```cpp
enum Days { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday};

Days & operator++(Days &d)
{
    if(d == Sunday)
        return Monday;
    int temp = d;
    return d = Days(++temp);
}

std::ostream & operator<<(std::ostream & os, const Days &d)
{
    switch(r)
    {
        case Monday : return os << "Monday";
        case Tuesday : return os << "Tuesday";
        case Wednesday : return os << "Wednesday";
        case Thursday : return os << "Thursday";
        case Friday : return os << "Friday";
        case Saturday : return os << "Saturday";
        case Sunday : return os << "Sunday";
    }
}
```
