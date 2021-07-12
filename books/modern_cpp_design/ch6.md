# Singleton

## Static Data + Static Functions != Singleton
Providing a class for singleton with static functions and static data only does not make this class Singleton. This imposes several issues:
- Static functions can not be virtual so can't be customized easily.
- Initialization and clean up can't be easy. There is no central point of implementing them.

## C++ way of things
- Prefer dynamic initialization for the underlying singleton object rather than `static initialization` (with compile time constants). Static initialization occurs before the very first assembly of the prgram gets executed.

- However, C++ does not have pre-defined scheme for the order of the initalization of the dynamically initialized namespace level objects within different translation units.

## Enforcing Singleton's Uniqueness
- Disable copy constructor & operator=().
- Prefer returning references instead of raw pointers from `Instance()` functions. The underlying object might still be dynamically allocated.
- Make destructor private: This will avoids clients to hold a pointer to Singleton object and delete it.

## Destroying the Singleton
The Singleton object is created on demand but the problem is the time of the destruction: `resource leak`.
- The Singleton object mught acquire unbounded set of resouces i.e. network connections, handles to OS mutexes etc.
- The good way of solving this is to find a way Singleton object to be deleted during the application shutdown.

### Meyer's Singleton
Provide the Singleton object through local static object and then return the reference. This is not the same primitive static variables initialized with compile time constants.

```cpp
Singleton& Singleton::Instance()
{
    static Singleton obj;
    return obj;
}
```

If it was initialized with compile time constants, it would be initialized at the load time of the program even it is never used. However, if it is defined as MEyer's definition - initializer is NOT the compile-time constant or it has a constructor of its own - the variable is initialized when the first call is made to that function.

Thanks to the C++ compiler and runtime mechanisms, the compiler will produce the proper destruction mechanism during the application's exit sequence.

## Dead Reference Problem
 > C++ Rule: Runtime support destroy local static objects in the reverse order of their creation.

 If an application uses multiple interacting singletons, an automated way to control their lifetime can NOT be achieved. Some tools can be used.
- Define a local static `destroyed_` member boolean and set it to `true` during construction.

## Solution I to dead reference problem: Phoenix Singleton
Phoenix Singleton always assures that there is an only and only one Singleton exists throught the application's lifetime.
```cpp
class Singleton
{
    // as before
    void KillPhoenixSingleton();
};
// .cpp
void Singleton::OnDeadReference()
{
    Create();
    // underyling data is currently pointing the destructed
    // object so assign the newly created instance to that -old- address
    
    // replacement new
    new (ptr_instance_) Singleton;

    // Queue this new object destruction
    atexit(KillPhoenixSingleton);

    // Reset the flag
    destroyed_ = false;
}

void Singleton::KillPhoenixSingleton()
{
    // call the destructor manually
    pInstance_->~Singleton();
}
```
## Singletons with Longevity:
> One of the major problem of Phoenix Singleton is keeping the state consistent between destruction and reconstruction. It means that when the singleton is first destroyed we should transfer the state so the new instance has it.

Rule of thumb: Any object that uses an object with longevity must have a shorter longevity than the used object.
