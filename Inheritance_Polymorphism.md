# Inheritance
Before starting this chapter, let me phrase this advice:
> Always prefer composition over inheritance when applicable. Use inheritance when you have to.

## **Motto: Push policy up and implementation down!!!**
Design your system through policies or contracts and let your implementation details to obey them. This principle works well with other idioms and design principles.


- Derived class is customization of a base class. Base class should impose pre/post conditions and child classes should always respect. Base class is the contract.

- Consider using Nonvirtual Interface (NVI) pattern by making virtual functions nonpublic and public functions nonvirtual.
    * Another way of seperation of interface and implementation
    * nonvirtual public methods are representing the interfaces.
    * virtual protected/private methods provide customization point (implementation detail)
    * Base class controls the policy through checking pre/post conditions. Instrumentation can be inserted at this level.
    * Supports single reponsibility principle. Nonvirtual public function does not have to specify interface and implementation together. 

- Do not inherit from classes which are not designed to be a base class i.e. `std::string`. Prefer composition:
    * public nonvirtual destructor causes undefined behavior by deleting pointers to string that points to der_string.
    * std::string is not protected against slicing

- Prefer `protected` methods rather than `protected` fields: It is as bad as `public` since it is giving access to its internals to its child class.

- Do not call virtual functions from in `constructors` and `destructors`.

- Make base class destructors `public virtual` or `protected non-virtual`.

- Prefer `clone()` approach to avoid slicing.

- Never redefine inherited non-virtual functions in the child class.


**Dependency Inversion Principle**: Modules and implementation details should depend on abstractions. Provide `Abstract Interface`: It is a class definition with

* pure virtual function declerations
* No member data - no state (preffered)
* No implementation
* virtual destructor if `polymorpohic deletion` to be supported.

## Rules:
- Do not redeclare default arguments of virtual functions when overriding.
- `Private` methods can also be virtual and be overriden by child classes.
## Inheritance types
There three inheritance types `public`, `protected`, `private` , `multiple` and `virtual`. These do not compete with each other. They represent different idioms to model a specific logic.

### Public Inheritance:
Represents `is a` relationship. 
### Protected Inheritance:

### Private Inheritance:
Represents `implemented in terms of ` relationship such as `composition`.
### Multiple Inheritance:

### Virtual Inheritance:

### Empty Base Optimization

# Polymorphism
Base class destructor should be `virtual`.
Control copy & move behavior through base classes.
If copy is disabled, use `clone()` approach to copy.

## Static vs. Dynamic Polymorphism

# Consider non-virtual ways to customize behavior