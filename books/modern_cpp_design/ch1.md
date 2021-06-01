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