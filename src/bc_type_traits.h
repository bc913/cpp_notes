//
//  bc_type_traits.h
//  CPP_Playground
//
//  Created by Burak Can on 6/26/21.
//  Copyright © 2021 Burak Can. All rights reserved.
//

#ifndef bc_type_traits_h
#define bc_type_traits_h

#include <type_traits>

namespace bc
{    
    template<typename T, T v>
    struct integral_constant
    {
        static constexpr T          value = v;
        typedef T                   value_type;
        typedef integral_constant   type;
        constexpr operator value_type() const noexcept { return value; }
    };

    template<bool Val>
    using bool_constant = integral_constant<bool, Val>;
    using true_type = bool_constant<true>;
    using false_type = bool_constant<false>;
    
    // const
    // remove_const
    template<typename T>
    struct remove_const { typedef T type; };
    
    template<typename T>
    struct remove_const<const T> { typedef T type; };
    
    template<typename T>
    using remove_const_t = typename remove_const<T>::type;
    // add_const
    template<typename T>
    struct add_const { typedef const T type; };
    
    template<typename T>
    using add_const_t = typename add_const<T>::type;
    
    // is_const
    template<typename T>
    struct is_const : public false_type{};
    
    template<typename T>
    struct is_const<const T> : public true_type{};
    
    template<typename T>
    struct is_const<const T&> : public true_type{};
    
    template<typename T>
    struct is_const<const T&&> : public true_type{};
    
    template<typename T>
    inline constexpr bool is_const_v = is_const<T>::value;
    
    // reference
    // remove reference
    template<typename T>
    struct remove_reference { typedef T type;};
    
    template<typename T>
    struct remove_reference<T&> { typedef T type; };
    
    template<typename T>
    struct remove_reference<T&&> { typedef T type; };
    
    template<typename T>
    using remove_reference_t = typename remove_reference<T>::type;
    
    // add_reference
    template<typename T>
    struct add_reference
    {
        typedef T& type;
    };
    
    template<typename T>
    struct add_reference<T&>
    {
        typedef T& type;
    };
    
    template<typename T>
    using add_reference_t = typename add_reference<T>::type;
    
    // add_const_reference
    template<typename T>
    struct add_const_reference
    {
        typedef const T& type;
    };
    
    template<typename T>
    struct add_const_reference<T&>
    {
        typedef T& type;
    };
    
    template<typename T>
    using add_const_reference_t = typename add_const_reference<T>::type;
    
    // reference_wrapper
    template<typename T>
    class reference_wrapper
    {
    public:
        typedef T type;
        reference_wrapper(type& arg) noexcept : ptr(std::addressof(arg)) {}
        reference_wrapper(type&& arg) = delete; //do not bind to temps
        // access
        operator type&() const noexcept { return *ptr; }
        type&      get() const noexcept { return *ptr; }
        
    private:
        type* ptr;
    };
    
    template <class T>
    inline reference_wrapper<T> ref(T& arg) noexcept
    {
        return reference_wrapper<T>(arg);
    }

    template <class T>
    inline reference_wrapper<T> ref(reference_wrapper<T> arg) noexcept
    {
        return bc::ref(arg.get());
    }

    template <class T>
    inline reference_wrapper<const T> cref(const T& arg) noexcept
    {
        return reference_wrapper<const T>(arg);
    }

    template <class T>
    inline reference_wrapper<const T> cref(reference_wrapper<T> arg) noexcept
    {
        return cref(arg.get());
    }
    
    // conditional
    template<bool flag, typename TrueType, typename FalseType>
    struct conditional
    {
        typedef TrueType type;
    };
    
    template<typename TrueType, typename FalseType>
    struct conditional<false, TrueType, FalseType>
    {
        typedef FalseType type;
    };
}


#endif /* bc_type_traits_h */
