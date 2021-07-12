# Object Factories
## typeid & std::type_info
`typeid` might seem a proper solution to get an information about a type during runtime but it is NOT secure enough.

`std::type_info::name()` function produces a `const char *` for the human readible name of the given type. However, it is not always secure.

- There is not guarantee that the given name is the actual name of the type. It might be anything.
- The generated names might not be unique so this function might produce the same name for two different given type.
- The generated name is also NOT unique throughout the application's lifetime. This means making multiple calls to this function during the runtime might produce different results for the same type.