# Techniques

## Partial template specialization
Partial template specialization can only occur for class templates. Function templates are out of context.

```cpp
template<typename T, typename U>
class C {/* generic impl*/};

template<>
class C<ModalDialog, Controller>
{/* fully specialized impl*/};

template<typename T>
class C<T, Controller>
{/* Partially specialized for Controller type*/};

template<typename ButtonArgs>
class C<Button<ButtonArgs>, Controller>
{/*specialized for all instantiations of Button and Controller*/};
```

## Local Classes
Local class can be defined in a function definition. That class belongs the function scope and can not be accessed from outside. 

```cpp
void func
{
    class Local
    {
        // member variables and functions
    };
}
```

Local classes can not define static member functions & variables and can not access non-static local variables.
However, they can access template parameters of the functions.
```cpp
class Interface
{
public:
    virtual void func() = 0;
};

template<typename T, typename U>
Interface* make_adapter(const T& obj, const U& arg)
{
    class Local : public Interface
    {
    public:
        Local(const T& obj, const U& arg)
            : obj_(obj), arg_(arg){}
        
        virtual void func() { obj_.call(arg_); }
    private:
        T obj_;
        U arg_;
    };
}
```

## Mapping integral constants to types
```cpp
namespace bc
{
    template<typename T, T v>
    struct integral_constant
    {
        static constexpr T value = v;
        typedef T                   value_type;
        typedef integral_constant   type;
        constexpr operator value_type() const noexcept { return value; }
    };

    typedef integral_constant<bool, true>   true_type;
    typedef integral_constant<bool, false>  false_type;
    // enable_if
    template<bool, typename T = void>
    struct enable_if{};

    template<typename T>
    struct enable_if<true, T>
    {
        typedef T   type;
    };
}
```