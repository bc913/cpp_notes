# Internal vs. External Linkage

## Terminology
**Translation Unit**: The preprocessing of a source file - preprocessing the `#include` directives - results in a definition called `translation unit` so it basically is a preprocessed source file. Each preprocessed source file is a translation unit.

Each translation unit communicates with each other or other parts of the program through available(exposed) identifiers with external linkage.

Based on the relationships between the translation units, the compiler generates the corresponding object files.

[**One definition rule**](https://en.cppreference.com/w/cpp/language/definition)

## Internal Linkage
An identifier with `internal linkage` can only be seen and used within a single translation unit and is not accessible from other translation units. Internal linkage can be achieved with `static` storage class specifier. One exception to this rule is having a free function with static variable defined within.

- Free Variables:
```cpp
// .h or .cpp
static int global_x; // default initialized
static int global_x{2};

const int global_b = 3;
constexpr int global_c = 4;
```

- Free functions
```cpp
// Method 1
// .h
// can be declared under a namespace
static void func();

// .cpp
void func() {/**/}

// Method 2 - Unnamed namespace
// .h
namespace{
    void func();
}

// .cpp
namespace{
    void func() {/**/}
}
```

- Free Functions with static variable defined within
```cpp
// .h
// can be declared under a namespace
Printer& singleton();

// .cpp
Printer& singleton()
{
    static Printer p;
    return p;
}
```

## External Linkage
An identifier with `external linkage` can be seen and used by other translation units. `External linkage` can be achieved by identifying the name `extern` storage class specifier. Usually used to define global variable.

- Free variables:
```cpp
// .h or .cpp
int global_var1{3};
extern const int global_var2{4};
extern constexpr int global_var3{6};

// Fwd decl in any client
extern int global_var1;
extern const int global_var2;
extern constexpr int global_var3;
```

- Free functions: Functions have external linkage by default;
```cpp
// .h 
// can be in a namespace
void func();

// .cpp
void func(){/**/}
```

## Rules & Idioms
- `dllexport` and `dllimport` can **NOT** be applied on identifiers with internal linkage. Attempting to honor those identifiers with `dllimport/dllexport` will result in compile-time error.

- If the developer's purpose is to achieve singleton with only and only one instance using `free function with static variable defined within` method, it is better **NOT** to identify that method with `inline` keyword. The reason is simple:
> If the default linkage of inline functions are **internal** (might be dependent on the compiler), it might create more than one copy of this function's and its local static variable's object code which means you'd have multiple singletons.

- When multiple translation unit consumes the same identifier with `internal linkage`, each translation unit will have its 
own copy.

```cpp
// utils.h in a dll component
namespace bc
{
    static int global_var = 999;
    void SOME_API func();
}

// utils.cpp in the same dll component
namespace bc
{
    void func() { global_var = 123; std::cout << global_var << "\n"; }
}

// Client (might be an other lib or executable) component
// other.cpp
#include <utils.h>

int main()
{
    bc::func();                             // Prints 123
    std::cout << bc::global_var << "\n";    // Prints 999
    bc::global_var = 500;
    std::cout << bc::global_var << "\n";    // Prints 500
    return 0;    
}

```

## References
- [Microsoft Docs - Translation units and linkage](https://docs.microsoft.com/en-us/cpp/cpp/program-and-linkage-cpp?view=msvc-160)
- [Iso Std C++](http://www.open-std.org/jtc1/sc22/WG14/www/docs/n1256.pdf)
- [More Effective C++ - Scott Meyers - Addison Wesley]
- [learncpp.com](https://www.learncpp.com)