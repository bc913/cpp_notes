# TypeLists

## 3.1 - 3.12
```cpp
// Under namespace bc::loki

//to mark the end of a typelist
// and return type not found
class null_t{};

// don't care template
struct empty_t{};

template<typename T, typename U>
struct type_list
{
    typedef T head;
    typedef U tail;
};



// algorithms
namespace tl
{
    template<typename TypeList>
    struct length;
    
    template<>
    struct length<null_t>
    {
        static constexpr std::size_t value = 0;
    };
    
    template<typename T, typename U>
    struct length<type_list<T,U>>
    {
        static constexpr std::size_t value = 1 + length<U>::value;
    };
    
    // index
    template<typename TypeList, std::size_t Index>
    struct type_at;
    
    template<typename Head, typename Tail>
    struct type_at<type_list<Head, Tail>, 0>
    {
        typedef Head    result;
    };
    
    template<typename Head, typename Tail, std::size_t Index>
    struct type_at<type_list<Head, Tail>, Index>
    {
        typedef typename type_at<Tail, Index - 1>::result   result;
    };
    // =====================
    // searching typelists
    // =====================
    template<typename TypeList, typename T>
    struct index_of;
    
    template<typename T>
    struct index_of<null_t, T>
    {
        static constexpr std::size_t value = -1;
    };
    
    // if the head of type_list is T
    template<typename T, typename Tail>
    struct index_of<type_list<T, Tail>, T>
    {
        static constexpr std::size_t value = 0;
    };
    
    template<typename Head, typename Tail, typename T>
    struct index_of<type_list<Head, Tail>, T>
    {
    private:
        static constexpr std::size_t tail_value = index_of<Tail, T>::value;
    public:
        static constexpr std::size_t value = tail_value == -1 ? -1 : tail_value + 1;
    };
    
    // =====================
    // append
    // =====================
    
    template<typename TypeList, typename TypeToAppend>
    struct append;
    
    // if typelist and type tp append is null_t
    template<>
    struct append<null_t, null_t>
    {
        typedef null_t      result;
    };
    
    // if type_list is null_t only
    template<typename T>
    struct append<null_t, T>
    {
        typedef type_list<T, null_t>    result;
    };
    
    // if t_list null_t and T is type_list itself
    template<typename Head, typename Tail>
    struct append<null_t, type_list<Head, Tail>>
    {
        typedef type_list<Head, Tail>   result;
    };
    
    template<typename Head, typename Tail, typename T>
    struct append<type_list<Head, Tail>, T>
    {
        typedef type_list<Head, typename append<Tail, T>::result>  result;
    };
    
    // =====================
    // prepend
    // =====================
    template<typename T, typename TypeList>
    struct prepend;
    
    // if type_list and type to prepend is null_T
    template<>
    struct prepend<null_t, null_t>
    {
        typedef null_t  result;
    };
    
    // if type_list is null_t only
    template<typename T>
    struct prepend<T, null_t>
    {
        typedef type_list<T, null_t> result;
    };
    
    // if type_list is null_t and T is the type_list
    template<typename Head, typename Tail>
    struct prepend<type_list<Head, Tail>, null_t>
    {
        typedef type_list<Head, Tail>   result;
    };
    
    template<typename TypeToPrepend, typename Head, typename Tail>
    struct prepend<TypeToPrepend, type_list<Head, Tail>>    
    {
        typedef type_list<TypeToPrepend, typename prepend<Head, Tail>::result> result;
    };
    
    // =====================
    // Erasing a type
    // =====================
    template<typename TypeList, typename T>
    struct erase;
    
    // if type_list is null_t
    template<typename TypeToErase>
    struct erase<null_t, TypeToErase>
    {
        typedef null_t result;
    };
    
    // if type_to_erase if head
    template<typename TypeToErase, typename Tail>
    struct erase<type_list<TypeToErase, Tail>, TypeToErase>
    {
        typedef Tail    result;
    };
    
    template<typename Head, typename Tail, typename TypeToErase>
    struct erase<type_list<Head, Tail>, TypeToErase>
    {
        typedef type_list<Head, typename erase<Tail, TypeToErase>::result> result;
    };
    
    // =====================
    // Erasing all occurences
    // =====================
    template<typename TypeList, typename T>
    struct erase_all;
    
    // if type_list is null_t
    template<typename TypeToErase>
    struct erase_all<null_t, TypeToErase>
    {
        typedef null_t result;
    };
    
    // if the head of the typelist is type_to_erase
    template<typename TypeToErase, typename Tail>
    struct erase_all<type_list<TypeToErase, Tail>, TypeToErase>
    {
        // do not stop when the to be erased type is found
        typedef typename erase_all<Tail, TypeToErase>::result result;
    };
    
    template<typename Head, typename Tail, typename TypeToErase>
    struct erase_all<type_list<Head, Tail>, TypeToErase>
    {
        typedef type_list<Head, typename erase_all<Tail, TypeToErase>::result> result;
    };
    
    // =====================
    // Erasing duplicates
    // =====================
    template<typename TypeList>
    struct erase_duplicates;
    
    template<>
    struct erase_duplicates<null_t>
    {
        typedef null_t result;
    };
    
    template<typename Head, typename Tail>
    struct erase_duplicates<type_list<Head, Tail>>
    {
    private:
        typedef typename erase_duplicates<Tail>::result l1;
        typedef typename erase<l1, Head>::result        l2;
    public:
        typedef type_list<Head, l2>                     result;
    };
    
    // =====================
    // Replacing a type in a type list
    // =====================
    template<typename TypeList, typename TypeToRemove, typename TypeToPut>
    struct replace;
    
    // if type_list is null
    template<typename TypeToRemove, typename TypeToPut>
    struct replace<null_t, TypeToRemove, TypeToPut>
    {
        typedef null_t result;
    };
    
    // if head is type to remove
    template<typename Head, typename Tail, typename TypeToPut>
    struct replace<type_list<Head, Tail>, Head, TypeToPut>
    {
        typedef type_list<TypeToPut, Tail> result;
    };
    
    template<typename Head, typename Tail, typename TypeToRemove, typename TypeToPut>
    struct replace<type_list<Head, Tail>, TypeToRemove, TypeToPut>
    {
        typedef type_list<Head, typename replace<Tail, TypeToRemove, TypeToPut>::result> result;
    };
    
    // =====================
    // Replace all occurences of a type
    // =====================
    template<typename TypeList, typename TypeToRemove, typename TypeToPut>
    struct replace_all;
    
    // if type_list is null
    template<typename TypeToRemove, typename TypeToPut>
    struct replace_all<null_t, TypeToRemove, TypeToPut>
    {
        typedef null_t result;
    };
    
    // if type to remove is head
    template<typename Head, typename Tail, typename TypeToPut>
    struct replace_all<type_list<Head, Tail>, Head, TypeToPut>
    {
        typedef type_list<TypeToPut, typename replace_all<Tail, Head, TypeToPut>::result> result;
    };
    
    template<typename Head, typename Tail, typename TypeToRemove, typename TypeToPut>
    struct replace_all<type_list<Head, Tail>, TypeToRemove, TypeToPut>
    {
        typedef type_list<Head, typename replace_all<Tail, TypeToRemove, TypeToPut>::result> result;
    };
    // =====================
    // Reverse
    // =====================
    template<typename TypeList>
    struct reverse;
    
    template<>
    struct reverse<null_t>
    {
        typedef null_t result;
    };
    
    template<typename Head, typename Tail>
    struct reverse<type_list<Head, Tail>>
    {
        typedef typename append<typename reverse<Tail>::result, Head>::result   result;
    };
    
    // =====================
    // Most Derived
    // Finds the type in a typelist that is the most derived from a given type
    // Invocation (TList is a typelist, T is a type):
    // MostDerived<TList, T>::Result
    // returns the type in TList that's the most derived from T
    // =====================
    template<typename TypeList, typename Base>
    struct most_derived;
    
    template<typename T>
    struct most_derived<null_t, T>
    {
        typedef T result;
    };
    
    template<typename Head, typename Tail, typename Base>
    struct most_derived<type_list<Head, Tail>, Base>
    {
    private:
        typedef typename most_derived<Tail, Base>::result candidate;
    public:
        /*
            std::conditional<bool, T, U>::type = Select<bool, T, U>::result;
            std::is_base_of<B, D>::value = SuperSubClass<B, D>::value;
            */
        typedef typename std::conditional<
            std::is_base_of<candidate, Head>::value,
                        Head, candidate>::type result;
    };
    
    // =====================
    // Derived to front
    // Arrang the types in the typelist so that most derived
    // types appear first
    // =====================
    template<typename TypeList>
    struct derived_to_front;
    
    template<>
    struct derived_to_front<null_t>
    {
        typedef null_t  result;
    };
    
    template<typename Head, typename Tail>
    struct derived_to_front<type_list<Head, Tail>>
    {
    private:
        typedef typename most_derived<Tail, Head>::result                   the_most_derived;
        typedef typename replace<Tail, the_most_derived, Head>::result      temp;
        typedef typename derived_to_front<temp>::result                     l;
    public:
        typedef type_list<the_most_derived, l>                              result;
    };
    
}

// Convenience
template<typename T>
using type_list_1 = type_list<T, null_t>;

template<typename T, typename U>
using type_list_2 = type_list<T, type_list_1<U>>;

template<typename T1, typename T2, typename T3>
using type_list_3 = type_list<T1, type_list_2<T2, T3>>;

template<typename T1, typename T2, typename T3, typename T4>
using type_list_4 = type_list<T1, type_list_3<T2, T3, T4>>;

template<typename T1, typename T2, typename T3, typename T4, typename T5>
using type_list_5 = type_list<T1, type_list_4<T2, T3, T4, T5>>;
```





## TODO: 3.13

## References
### Variadic templates
https://studiofreya.com/cpp/cpp-variadic-templates/

https://dzone.com/articles/variadic-template-c-implementing-unsophisticated-t

https://www.modernescpp.com/index.php/c-insights-variadic-templates

http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n2080.pdf

https://www.ida.liu.se/~TDDD38/lecture/slides/templates_II.pdf


### References
https://blog.galowicz.de/2016/05/08/compile_time_type_lists/
https://github.com/lipk/cpp-typelist
https://codebuddy.pl/c11-typelists-for-beginners/
https://www.codeproject.com/Articles/1077852/TypeLists-and-a-TypeList-Toolbox-via-Variadic-Temp


### tuple
https://github.com/taocpp/tuple/blob/master/include/tao/tuple/tuple.hpp

https://github.com/gcc-mirror/gcc/blob/16e2427f50c208dfe07d07f18009969502c25dc8/libstdc%2B%2B-v3/include/std/tuple

https://github.com/llvm-mirror/libcxx/blob/a12cb9d211019d99b5875b6d8034617cbc24c2cc/include/tuple

https://github.com/gcc-mirror/gcc/blob/16e2427f50c208dfe07d07f18009969502c25dc8/gcc/testsuite/g%2B%2B.dg/cpp0x/variadic-tuple.C


