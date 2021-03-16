# Templates and Generic Programming

## typename vs class
`typename`: allows to use non-class built-in types as template type parameters

`class`: Only allows to use class types as template type parameters

## Non-type template parameters
A non-type parameter represents a value instead of a type.
```cpp
//class
template<typename T, size_t Size>
class array
{
/**/
};
//usage
auto arr = std::array<int, 5>{1,2,3,4,5};

//function
template <unsigned N, unsigned M>
void compare(const char (&p) [N], const char (&p2)[M]){/**/}

// usage
auto result = compare("hi", "buddy");
```
If the non-type argument is bound to an integral type, the argument should be constant expression. If it is bound to a reference or pointer, it must have a static lifetime. A pointer can be `nullptr` or zero-valued constant expression.


## Template Declerations
```cpp
template <typename T> int compare(const T&, const T&);
template <typename T> class Blob;
```
> It is always a good practice to provide declerations at the begining of the file before they are used.

## Template default arguments
If a template has default arguments, to use is with default arguments call with empty brackets.
```cpp
//function
template<typename T = int> void calc(const T& arg){/*Do something*/}
calc<>(4); // will call calc<int>(4)

//class
template<typename T = double> class myClass{};
myClass<int> obj1{};
myClass<> obj2{};// myClass<double>
```

## Template Argument Deduction

### Function
Template argument deduction takes place after the function template name lookup (which may involve argument-dependent lookup) and before template argument substitution (which may involve SFINAE) and overload resolution. 

**Value**
```cpp
template<typename T>void f(T){/**/}

int a[3];
f(a); // P = T, A = int[3], adjusted to int*: deduced T = int*

void b(int);
f(b); // P = T, A = void(int), adjusted to void(*)(int): deduced T = void(*)(int)

const int c = 13;
f(c); // P = T, A = const int, adjusted to int: deduced T = int

// Pass by value
// -------------
int i = 5;
f(i); // deduced type: int - arg type: int

const int ci = 6; // const dropped
f(ci); // deduced type: int - arg type: int

// Pass by pointer
// -------------
int* iptr = &i;
f(iptr); // deduced type: int* - arg type: int*

const int* ciptr = &i;
f(ciptr); // deduced type: const int* - arg type: const int*

int* const i_cptr = &i; //const dropped
f(i_cptr); // deduced type: int* - arg type: int*

const int* const ci_cptr = &i;//const dropped
f(ci_cptr); // deduced type: const int*  - arg type: const int*

// Pass by reference
// -------------
int& ir = i;
f(ir); // deduced type: int - arg type: int

const int& icr = i;//const dropped
f(icr); // deduced type: int - arg type: int

// Pass by array
//--------------
int a[3] = { 4, 5, 6 };
f(a); // deduced type: int* - arg type: int*

const int b[4] = { 6 , 7, 8, 9 };
f(b);// deduced type: const int* - arg type: const int*

```

**Const Value**
```cpp
template<typename T>void f(const T arg){/**/}

// Pass by value
// -------------
int i = 5;
f(i); // deduced type: int - arg type: const int

const int ci = 6; // const dropped
f(ci); // deduced type: const int - arg type: const int

// Pass by pointer
// -------------
int* iptr = &i;
f(iptr); // deduced type: int* - arg type: int* const

const int* ciptr = &i;
f(ciptr); // deduced type: const int* - arg type: const int* const

int* const i_cptr = &i; //nothing changed
f(i_cptr); // deduced type: int* const - arg type: int* const

const int* const ci_cptr = &i; //nothing changed
f(ci_cptr); // deduced type: const int* const - arg type: const int* const

// Pass by reference
// -------------
int& ir = i;//ref dropped
f(ir); // deduced type: int - arg type: const int

const int& icr = i; // ref dropped
f(icr); // deduced type: const int - arg type: const int

// Pass by array
//--------------
int a[3] = { 4, 5, 6 };
f(a); // deduced type: int* - arg type: int* const

const int b[4] = { 6 , 7, 8, 9 };
f(b);// deduced type: const int* - arg type: const int* const

```

**Reference**:
```cpp
// argument must be an lvalue
template<typename T>void f(T& arg){/**/}

// Pass by value
// -------------
int i = 5;
f(i); // deduced type: int - arg type: int &

const int ci = 6;
f(ci); // deduced type: const int - arg type: const int &

// Pass by pointer
// -------------
int* iptr = &i;
f(iptr); // deduced type: int* - arg type: int* &

const int* ciptr = &i;
f(ciptr); // deduced type: const int* - arg type: const int* &

int* const i_cptr = &i;
f(i_cptr); // deduced type: int* const - arg type: int* const &

const int* const ci_cptr = &i;
f(ci_cptr); // deduced type: const int* const  - arg type: const int* const &

// Pass by reference
// -------------
int& ir = i;
f(ir); // deduced type: int - arg type: int &

const int& icr = i;
f(icr); // deduced type: const int - arg type: const int &

// Pass by array
//--------------
int a[3] = { 4, 5, 6 };
f(a); // deduced type: int[3] - arg type: int[3] &

const int b[4] = { 6 , 7, 8, 9 };
f(b);// deduced type: const int[4] - arg type: const int[4] &

// Errors
f(4); // must be an lvalue
```

**Const Reference**:
```cpp
// Any kind of argument can be passed
// const && non-const
// rvalues
// temp objects
// literals
template<typename T>void f(const T &){/**/}

// Pass by value
// -------------
int i = 5;
f(i); // deduced type: int - arg type: const int &

const int ci = 6;
f(ci); // deduced type: int - arg type: const int &

// Pass by pointer
// -------------
int* iptr = &i;
f(iptr); // deduced type: int* - arg type: int* const &

const int* ciptr = &i;
f(ciptr); // deduced type: const int* - arg type: const int* const &

int* const i_cptr = &i;
f(i_cptr); // deduced type: int* const - arg type: int* const &

const int* const ci_cptr = &i;
f(ci_cptr); // deduced type: const int* const  - arg type: const int* const &

// Pass by reference
// -------------
int& ir = i;
f(ir); // deduced type: int - arg type: const int &

const int& icr = i;
f(icr); // deduced type: const int - arg type: const int &

// Pass by array
//--------------
int a[3] = { 4, 5, 6 };
f(a); // deduced type: int[3] - arg type: const int[3] &

const int b[4] = { 6 , 7, 8, 9 };
f(b);// deduced type: const int[4] - arg type: const int[4] &

// Pass rvalue
//------------
int cval = 6;
f(std::move(cval)); // deduced type: int - arg type: const int &

```

## Template Compilation & Initialization

1. Compiler checks the template itself for any syntax error.
2. This stage is triggered when compiler sees a usage of a template: Checks against if the number of provided arguments are matched with template definition.
3. Then compiler instatiates the template with provided type arguments and generates a version of the template (function or class) for given type arguments. Writes it to obj file.
4. If there is any error occurs, it is reported during linkage. 


The compiler does not generate code for the template until it is being used. Compiler can still detect syntax error. If that function template or class template is being used for different types, compiler generates different codes for each usage.

> Each institation of a class or function template, generates an independent and different code for each usage (as long as different type arguments provided.) and each generated code has no relationship with each other.

This rule is the same for the class template members. A code is generated for members of class templates if they are used. 

```cpp
// a.h
template<typename T>
void myFunc(const T& arg) { /*do something */}

// b.cpp
#include"a.h"
//Somewhere in the source code
auto myInt = 5;
myFunc<int>(myInt);

//c.cpp
#include "a.h"
// somewhere in the source code
MyClass obj{};
myFunc<MyClass>(obj);
```

Compiler will generate two different function code for these two usage:
```cpp
void myFunc(const int & arg){/**/}
void myFunc(const MyClass & arg){/**/}
```

## Class Templates

### Template type aliases

```cpp
template<typename T> using twin = pair<T,T>;
template<typename T> using partNo = pair<T, unsigned>;
// usage
twin<int> win_loss;
twin<std::string> authors;

partNo<Vehicle> cars;
```

### Types as class members
The scope `::` operator normally resolves for a class member. If the class member is a type, `typename` keyword should be used.

```cpp
template<typename T, std::size_t Size>
class iterator
{
    public:
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;

};

template<typename T>
typename iterator::reference operator[](std::size_t index){/**/}

template<typename T>
typename iterator::const_reference operator[](std::size_t index) const{/**/}
```

### Static members of class templates

```cpp
template<typename T>
class smart_ptr
{
public:
    static std::size_t use_count() { return refCount_;} 
private:
    static std::size_t refCount_;
};

template<typename T>
std::size_t smart_ptr<T>::refCount_ = 0;

```

### Friendships

## Variadic Templates
### Function
Variadic functions are often recursive.
```cpp
template<typename T>
std::ostream& print(std::ostream& os, const T& t)
{
    return os << t;
}

template<typename T, typename ...Args>
std::ostream& print(std::ostream& os, const T& t, const Args&... rest)
{
    os << t << ", ";
    return print(os, rest...);
}
```

How to call nonvariadic template function with variadic args
```cpp
template<typename T>
std::string debug_rep(const T& t)
{
    std::ostringstream ret;
    ret << t;
    return ret.str();
}

template<typename ...Args>
std::ostream& errorMsg(std::ostream& os, const Args&... rest)
{
    return print(os, debug_rep(rest)...);
}

// debug_rep(rest) --> Call debug_rep on each element
```


#### Pack expansion
```cpp
template <typename ...UArgs>
void f(UArgs... args){/**/}

template <typename ...TArgs>
void g(TArgs... targs)
{
    // i.e g(int, double, const char*)
    f(targs...); // expands to f(int, double, const char *)
    f(&targs...); // expands to f(int*, double* , const char **)
}

```
#### sizeof... Operator
```cpp
template<typename ...Args>
void display(Args ...args)
{
    std::cout << "Number of type params: " << sizeof ...(Args) << "\n";
    std::cout << "Number of function parameters " << sizeof ...(args) << "\n";

}
```

### Class:
Variadic class templates can be used to model tuple through empty base optimization. [Link](https://eli.thegreenplace.net/2014/variadic-templates-in-c/#id8)


## Template Specialization:
### Function
```cpp
template <typename T>
void f(T &arg){/**/}

//specialization for int
template<>
void f<int>(int & arg){/**/}
// or
template<>
void f<>(int &arg){/**/}
// or
template<>
void f(int &arg){/**/}
```

#### Rules:
- If the primary template has a exception specification that isn't noexcept(false), the explicit specializations must have a compatible exception specification.

### Class: TODO
Typical examples are std::hash and std::array with zero elements.

#### Partial Specialization:
Only applicaple to class templates.
### Guideline:
- Templates and their specializations should be declered in the same header file.
- Declerations for all the templates with a given name should appear first, followed by any specializations of those templates.

## Overloading vs. Template Specialization
Function template specializations does NOT participate to `overload resolution`. There are some rules to consider:

- Non-template functions are the first class citizens so they are the ones to be picked by the compiler during overload resolution. Compiler has to pick the best match with arguments and return types.
- If there is no `first class citizen` available, then the function base templates are the `second class citizen`s.
    * `Most specialized` version of the function base template has higher priority then the `less specialized` or its `base template`.

### Guideline:
Follow Herb Sutter's [advice](<http://www.gotw.ca/publications/mill17.htm>): 
- **Moral 1**: If you want to customize a function base template and want that customization to participate in overload resolution (or, to always be used in the case of exact match), make it a plain old function, not a specialization. And, if you do provide overloads, avoid also providing specializations.
- **Moral #2**: If you're writing a function base template, prefer to write it as a single function template that should never be specialized or overloaded, and then implement the function template entirely as a simple handoff to a class template containing a static function with the same signature. Everyone can specialize that -- both fully and partially, and without affecting the results of overload resolution.

```cpp
template<class T> 
struct FImpl;

template<class T> 
void f( T t ) { FImpl<T>::f( t ); } // users, don't touch this!

template<class T> 
struct FImpl 
{ 
  static void f( T t ); // users, go ahead and specialize this 
};
```

## Best Practices:
- Template programs should try to minimize the number of requirements on template argument types

## Additional Resources
- [CppCon 2016: Arthur O'Dwyer “Template Normal Programming (part 1 of 2)”](<https://www.youtube.com/watch?v=vwrXHznaYLA&feature=youtu.be>)
- [Back to Basics: Function and Class Templates - Dan Saks - CppCon 2019](<https://www.youtube.com/watch?v=LMP_sxOaz6g>)