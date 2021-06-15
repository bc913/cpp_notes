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
    // bool_constant
    template<bool Val>
    using bool_constant = integral_constant<bool, Val>;
    using true_type = bool_constant<true>;
    using false_type = bool_constant<false>;

    /*
    // typedef integral_constant<bool, true>   true_type;
    // typedef integral_constant<bool, false>  false_type;
    // struct true_t : public integral_constant<bool, true>{};
    // struct false_t : public integral_constant<bool, false>{};
    */

    // remove_cv
    template< class T > struct remove_cv                   { typedef T type; };
    template< class T > struct remove_cv<const T>          { typedef T type; };
    template< class T > struct remove_cv<volatile T>       { typedef T type; };
    template< class T > struct remove_cv<const volatile T> { typedef T type; };
    template< typename T>
    using remove_cv_t = typename remove_cv<T>::type;
    
    template< class T > struct remove_const                { typedef T type; };
    template< class T > struct remove_const<const T>       { typedef T type; };
    template<typename T>
    using remove_const_t = typename remove_const<T>::type;
    
    template< class T > struct remove_volatile             { typedef T type; };
    template< class T > struct remove_volatile<volatile T> { typedef T type; };
    template<typename T>
    using remove_volatile_t = typename remove_volatile<T>::type;

    // is_same
    template<typename T, typename U>
    struct is_same : public false_type{};

    template<typename T>
    struct is_same<T, T> : public true_type{}; //partial specialization

    template<typename T, typename U>
    inline constexpr bool is_same_v = is_same<T, U>::value;

    // is_nullptr
    template<typename T>
    struct is_null_pointer : is_same<std::nullptr_t, remove_cv_t<T>>{};

    template<typename T>
    inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;
   
    
    // enable_if
    template<bool, typename T = void>
    struct enable_if{};

    template<typename T>
    struct enable_if<true, T>
    {
        typedef T   type;
    };

    template<bool B, typename T = void>
    using enable_if_t = typename enable_if<B, T>::type;
}

template<typename T>
class Container
{
    // polymorphic operation
    void do_something(T* obj, std::true_type)
    {
        T* newObj = obj->Clone();
        /*...*/
    }

    // non-polymoprhic operation
    void do_something(T* obj, std::false_type)
    {
        T* newObj = new T(*obj);
        /*...*/
    }

public:
    void do_something(T* obj)
    {
        do_something(obj, typename std::is_polymorphic<T>::type());
    }
};

class A
{
public:
    virtual A* Clone() const { return new A(*this); }
};

class B : public A
{
public:
    A* Clone() const override { return new B(*this); }
};

int main()
{
    Container<int> c;
    int* obj = new int(5);

    c.do_something(obj);
    delete obj;

    Container<A> ca;
    B* b_obj = new B();
    ca.do_something(b_obj);

    return 0;
}
```

## Typee-to-type MApping
Since the partial specialization of functions is not allowed in C++, customization is only available through overloading. An operation can be implemented differently for different types. A light type transformation template can be used for this purpose.

```cpp
template<typename T>
struct Type2Type
{
    typedef T OriginalType;
};

template<typename T, typename U>
T* Create(const U& arg, Type2Type<T>)
{
    return new T(arg);
}

template<typename U>
T* Create(const U& arg, Type2Type<Widget>)
{
    return new Widget(arg, -1);
}

//client
std::string* p_str = Create("Hello", Type2Type<std::string>());
Widget* pw = Create(100, Type2Type<Widget>());
```