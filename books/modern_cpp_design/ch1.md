# Policy Based Class Design

## What is Policy Class?
Policies define interface in the form of class or class template. The class template version will be emphasized mostly in this context.

Policies have similarities with traits but traits are focused on types while policies focus on the behavior. Since it has a generic nature through templates, it is bound at compilation time.

This would bring lots of advantages:
- No run-time surprises. Every error can be handled during compile time.
- The client defines which policy and behavior to use rather as opposed to what run-time polymorphism does.
- Static type information is established.
- No overhead of the virtual functions.


Policy classes are intended for standalone use through inheritance or composition.

## Basic implementation
Some basic rules:
- Keep the destructor as `protected` so only derived or host classes can destroy policy classes. There is no space or run-time overhead since it is not virtual.


```cpp
struct OpNewCreator
{
    static T* Create(){ return new T;}
protected:
    ~OpNewCreator(){}
};

template<typename T>
struct MallocCreator
{
    static T* Create()
    {
        void* buf = std::malloc(sizeof(T));
        if(!buf) return nullptr;
        return new(buf) T;
    }
protected:
    ~MallocCreator(){}
};

template<typename T>
struct PrototypeCreator
{
    PrototypeCreator(T* ptr = nullptr) : m_proto(ptr){}
    T* Create() 
    {
        return m_proto ? m_proto->Clone() : nullptr;
    }

    T* GetPrototype() {return m_proto; }
    void SetPrototype(T* ptr) { m_proto = ptr;}
protected:
    ~PrototypeCreator(){ delete m_proto; }
private:
    T* m_proto;
};

class Widget{/**/};

template<template<typename> typename CreationPolicy>
class MyWidgetManager : public CreationPolicy<Widget>
{
};

template<typename T, template<typename Created> typename CreationPolicy>
class Manager : public CreationPolicy<T>
{
};
int main()
{
    MyWidgetManager<MallocCreator> mw;
    Manager<int, OpNewCreator> mw2;
    return 0;
}
```
## Combining Policy Classes
Multiple policy classes can be combined under the same host class.

```cpp
template<typename T> 
struct NoChecking
{
    static void Check(T*){}
};

template<typename T>
struct EnforceNotNull
{
    class NullPointerException : public std::exception{};
    static void Check(T* ptr)
    {
        if(!ptr)
            throw NullPointerException();
    }
};

template<typename T> T* GetDefaultValue();

template<typename T>
struct EnsureNotNull
{
    static void Check(T*& ptr)
    {
        if(!ptr)
            ptr = GetDefaultValue<T>();
    }
};

template<typename T>
class DefaultSmartPtrStorage
{
public:
    typedef T* pointer_t;
    typedef T& reference_t;
protected:
    pointer_t get_pointer() { return ptr_; }
    void set_pointer(pointer_t ptr) { ptr_ = ptr; }
public:
    pointer_t ptr_;
};

template <
    typename T,
    template<typename> typename CheckingPolicy,
    template<typename> typename ThreadingModel,
    template<typename> typename Storage = DefaultSmartPtrStorage
>
class SmartPtr : public CheckingPolicy<T>, public Storage<T> {};

// Usage
typedef SmartPtr<Widget, EnforceNotNull, SingleThreaded> SafeWidgetPtr;

```

## Conversions
Conversions between incompatible policies can be achieved within host classes templated copy constructors, conversion operator overloads or both. (Check section 1.11)

## Orthogonal decomposition between policies
When decomposing host classes wtih Policy classes, try to compose `orhogonal` policies. Orthogonal policies mean that the policies are completely independent from each other. Each policy does not know anything about the other.

## Additional resources to read
[Wrapper](https://stroustrup.com/wrapper.pdf)
[Execute-Around pattern](https://stackoverflow.com/questions/16859519/how-to-wrap-calls-of-every-member-function-of-a-class-in-c11)
[Template Inheritance](https://blog.feabhas.com/2014/06/template-inheritance/)
