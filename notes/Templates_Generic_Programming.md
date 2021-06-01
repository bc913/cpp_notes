# Templates and Generic Programming

[typename vs class](#typename-vs-class)

[Non-type template parameters](#non-type-template-parameters)

[Template Declerations](#template-declerations)

[Template Default Arguments](#template-default-arguments)

[Template Argument Deduction](#template-argument-deduction)

[Template Compilation & Initialization](#template-compilation--initialization)

[Class Templates](#class-templates)

[Variadic Templates](#variadic-templates)

[Template Specialization](#template-specialization)

[Overloading vs. Template Specialization](#overloading-vs-template-specialization)

[Best Practices](#best-practices)

[Additional Resources](#additional-resources)

## typename vs class
`typename`: allows to use non-class built-in types as template type parameters

`class`: Only allows to use class types as template type parameters

## Non-type template parameters
A non-type parameter represents a value instead of a type.
```cpp
//class
template<typename T, size_t Size>
class array{/**/};

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

## Template Default Arguments
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
template<typename T>
void f(T){/**/}

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
template<typename T>
void f(const T arg){/**/}

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
template<typename T>
void f(const T &){/**/}

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


### Function pointers
```cpp
template<typename T>
int comp(const T&, const T&);

// legal
int(*func_ptr)(const int&, const int&) = comp; //OK

void func(int(*)(const std::string&, const std::string&));
void func(int(*)(const int&, const int&));

// ambiguity
func(compare); // ERROR
// clean
func(compare<int>); // LEGAL
func(compare<std::string>); // LEGAL
```

### References
#### L-Value
- Non-const
```cpp
template<typename T>
void func(T&);
// 
int i = 2;
const int ci = 3;
// Rule: Only l-values can be passed(const or non-const)
func(i); // deduced type is int
func(ci); //deduced type is const int
func(4); // ERROR: it is an r-value
```

- Const
```cpp
template<typename T>
void func(const T&);
// 
int i = 2;
const int ci = 3;
// Rule: Any kind of argument (const or non-const) (an object, a temp object,or a literal) can be passed
func(i); // deduced type for T: int
func(ci);// deduced type for T: int
func(4); // const& par can bound to an r-value: T is int
```
#### R-Value - Universal reference
The `T&&` in template function has two meanings:
1. r-value reference: It can bound to r-value references
2. universal reference: `T&&` can be r-value or l-value reference. They can bind to:
    - r-value refs
    - l-value refs
    - const or non-const objects
    - volatile or non-volatile objects
    - Even to objects both const and volatile.

```cpp
template<typename T>
void func(T&& param);

Widget w;
func(w); // l-value is passed, param's type: Widget&
func(std::move(w)) // r-value is passed. param's  type: Widget&&
func(Widget()); // temp(r-value) is passed so param's type: Widget&&
func(42); //temp(r-value) so param's type: int&& and deduced type for T: int
```

#### Reference Collapsing
Actually, the `universal reference` idiom sits on top of the `reference collapsing` mechanism. If a reference to a reference is created `indirectly` (via type alias or template parameter) then proper `reference collapsing` occurs.

> `T& &`, `T& &&` and `T&& &` collapse to `T&`.
> `T&& &&` collapses to `T&`.

This yields to the fact that if an l-value reference is passed to the template argument `T&&`, reference collapsing occurs and it is bound to an l-value reference at the end.

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
One of the major difference from function templates is that class templates can not deduce the type parameter from given arguments so `the type information should be provided within the brackets <> for class templates during instantiation`.

### Member functions
Class template member functions can be defined inside or outside the class. The member functions defined inside are `inline` by default. The template parameter list is not required to be specified for member functions defined inside but not so for outside member functions.
```cpp
template<typename T>
class pq
{
public:
    void pop()// marked as inline by default
    {
        std::pop_heap(c.begin(), c.end(), comp);
        c.pop_back();
    }
};

// Outside
template<typename T>
void pq::push(const T& val)
{
    c.push_back(val);
    std::push_heap(c.begin(), c.end(), comp);
}
```
> By default, a member function of an instantiated class template is only instantiated when that member function is used.

Within the class scope, the compiler can deduce the class template's argument type so no need to specify it.
```cpp
template<typename T>
class pq
{
public:
    pq& operator=(const pq& rhs) {/**/}
    // no need to specify the return or argument type as pq<T>
};
```

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
// Old style (typedef)
template<typename T, typename Container = std::vector<T>, typename Compare = std::less<T>>
class priority_queue
{
public:
    typedef Container                           container_type;
    typedef Compare                             value_compare;
    typedef typename container_type::value_type    value_type;
    typedef typename container_type::reference     reference;
    typedef typename container_type::size_type     size_type;
    static_assert((std::is_same<T, value_type>::value), "");
};

// New style (using)
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
It is always good practice to forward declare the template function or class which is going to be the friend of a given template class.

#### Same type
```cpp
// fwd decls are required
template<typename T> class pq_ptr;
template<typename T> class pq; // required for operator()== overload
template<typename T>
bool operator==(const pq<T>&, const pq<T>&);

// template class definition
template<typename T>
class pq
{
public:
    // class pq grants access to the verion of
    // pq_ptr and the operator== function instantiated with the same type
    friend class pq_ptr<T>;
    friend bool operator==<T>(const pq<T>&, const pq<T>&);
};

// Usage
pq<int> pi; //pq_ptr<int> and operator==<int> are friends with pq<int>
```
#### Specific Type
- Example 1:
```cpp
//fwd decl for template class
template<typename T> class Pal; 

// non-template class
class A
{
    friend class Pal<A>; // friendship to specific instantiation of class Pal
};

```
- Example 2:
```cpp
// template class
template<typename T>
class B
{
    friend class Pal3;
    // friendship to Pal3
    // no prior fwd decl required since it is a non-template class
};
```

#### General
When general friendship is in place, no prior forward decleration is required.
- Example 1:
```cpp
// non-template class
class A
{
    template<typename T>
    friend class Pal2;
    // friendship to all instantiations of Pal2
};
```

- Example 2:
```cpp
// template class definition
template<typename T>
class B
{
    template<typename X>
    friend class Pal2;
    // friendship to all instantiations of Pal2
    // all instances of Pal2 are friends with all instances of B
};
```
> To allow all instantiations as friends, the friend's template parameter(s) should differ from the template class which grants the friendship.

#### Own Type
```cpp
template<typename T>
class C
{
    friend T;
    // class C grants access to the type T
    // T can be user defined or built-in type
};
```

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
- C++ Primer, Stanley B. Lippman, Josee Lajoie and Barbara E. Moo.