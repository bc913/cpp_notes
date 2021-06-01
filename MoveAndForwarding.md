# RValue and Universal references & Move and Forwarding

## Universal reference
When an RValue reference is used in template definition, that is called `universal reference`. LValue or RValue reference can bound to it
```cpp
template<typename T>
void func(T&& param); // universal reference

Widget&& var1 = Widget(); // rvalue reference
auto&& var2 = var1; // universal reference
```
> If there is `T&&` without type deduction, it is r-value reference. 

`const` and `volatile` qualifiers disqualify a reference from being universal.

```cpp
template<typename T>
void func(const T&& param); // rvalue reference
```
### Type deduction for universal references
If l-value is passed, T is deduced to be a reference. If an r-value is passed, T is deduced to be non reference.
```cpp
template<typename T>
void func(T&& param);

Widget w();
func(w); // l-value reference. T = Widget &
Widget widgetFactory(); // returning an rvalue
func(widgetFactory())// r-value reference. T = Widget
```

## std::move does not move anything. It is an unconditional cast
`std::move` performs unconditional cast to r-value reference.
```cpp
// C++11
template<typename T>
typename std::remove_reference<T>::type&&
move(T&& param)
{
    using ReturnType = typename std::remove_reference<T>::type&&;
    return static_cast<ReturnType>(param);
}

//C++14
template<typename T>
decltype(auto) move(T&& param)
{
    using ReturnType = remove_reference_t<T>&&;
    return static_cast<ReturnType>(param);
}

```

Passing a `const` or `volatile` object to `std::move` disqualifies it from casting to r-value reference and copy operation is carried out.
> If you want to avoid copying during move, do NOT pass an object to std::move with `const` or `volatile` qualifier.

## std::forward does not forward anything. It is a conditional cast.
`std::forward` performs conditional cast to r-value reference. It only casts to r-value reference if the argument is bound to an rr-value.
```cpp
// C++11
template<typename T>
T&& forward(typename std::remove_reference<T>::type& param)
{
    return static_cast<T&&>(param);
}

template<typename T>
T&& forward(typename std::remove_reference<T>::type&& param)
{
    static_assert(std::is_lvalue_reference<T>(param), "bad forward call");
    return static_cast<T&&>(param);
}

// C++14
template<typename T>
T&& forward(typename std::remove_reference_t<T>& param)
{
    return static_cast<T&&>(param);
}

template<typename T>
T&& forward(typename std::remove_reference_t<T>&& param)
{
    static_assert(std::is_lvalue_reference<T>(param), "bad forward call");
    return static_cast<T&&>(param);
}

```


## Reference collapsing
Rererence to reference in C++ is forbidden.
```cpp
int a;
auto& & ra = a; //ERROR
```
If a reference to a reference arises in a context where this is permitted (i.e. template initialization), the references collapse to single-reference according to the following rule:
> If either reference is an l-value reference, the result is l-value reference. Otherwise(i.e. both are r-value reference) the result is r-value reference.

```cpp
// Reference collapsing rule
& + &   -> &
& + &&  -> &
&& + &  -> &
&& + && -> &&

template<typename T>
void func(T&& param);

Widget w{};
func(w);

// template initialization
void func(Widget& && param);
// Reference collapsing occurs according to the rules
void func(Widget & param);
```

Reference collapsing occurs in four contexts:
* template initialization
* `auto` type generation
```cpp
// l value
Widget w;
auto && w1 = w;
// This call transforms into
Widget & && w1 = w;
// with reference collapsing
Widget& w1 = w;

// rvalue
auto && w2 = widgetFactory();
// Transforms to
Widget&& w2 = widgetFactor();
```
* creation and use of `typedef`s and alias decleration
```cpp
template<typename T>
class Widget
{
public:
    typedef T&& RvalueRef;

};

Windget<int &> w;
// Transforms the typdef to
typedef int& && RvalueRef;
// with reference collapsing
typedef int& RValueRef
// It is not an rvalue anymore so naming is not so good.
```
* decltype


## Guidelines:
- Try to provide setter two setter functions (1 for lvalue and 1 for rvalue) for a member if move operation is supported for that type:
```cpp
void SetName(const std::string & str);
void SetName(std::string && str);
```
- Or a single set method with value type can also be applicable. Allows compiler optimization.
```cpp
void SetName(std::string str);
```

