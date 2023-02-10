# C++ Design Guidelines

### Linkage


### Utility functions
> Avoid free functions (except operator functions) at file scope in .h files; avoid free functions with external linkage (including operator functions) in .cpp files.
1. Free functions with user defined type arguments:
2. Classes that holds utility functions as static member functions

### Enumerations, typedefs
- Prefer `scoped enum` to `unscoped enum`
- If `unscoped enum` is have to be used, define within class to keep the global namespace cleaner.
```cpp
class MyClass
{
    public:
        enum Color {BLACK, RED, GREEN}

};
```

### Inline functions
- Member or free inline functions should be defined in header files.

### Macros
- Avoid preprocessor macros in header files except include guards

## Class Design
- const methods should not return non-const references or pointers
```cpp
class MyClass
{
    public:
        B& GetB() const; // BAD
        const B& GetB() const; // GOOD: Non-mutable
        B& GetB(); // OK: Mutable
};
```

## Inheritance
- Prefer `protected` methods rather than `protected` fields: It is as bad as `public` since it is giving access to its internals to its child class.


## Casting
- Prefer modern style C++ casting over C style casting
- `dynamic_cast<T>(arg)` will not work if Copy Ctor is deleted or private for type of `arg`. (E0330, C2248) 


## Implicit type conversions
- Implicit type conversion never happens on the implicit parameter, `this`, for member functions so consider `non-member` overloading if implicit type conversion is desired. [Meyers - Effective C++, Item 24]
```cpp
class MyClass
{
    public:
    MyClass(int){/*...*/}
    const MyClass operator*(const MyClass &rhs){/*...*/};

};

auto obj =  MyClass{};
auto result = obj * 2; // WORKS
auto result2 = 2 * obj; //ERROR - no implicit type conversion if not

```

- Implicit type conversion functions (also ctors) are **never** considered during template argument deduction.
