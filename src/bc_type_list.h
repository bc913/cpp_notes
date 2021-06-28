//
//  bc_type_list.h
//  CPP_Playground
//
//  Created by Burak Can on 6/27/21.
//  Copyright © 2021 Burak Can. All rights reserved.
//

#ifndef bc_type_list_h
#define bc_type_list_h

#include <cstddef>
#include "bc_type_traits.h"

namespace bc
{
    template<typename... Types>
    struct type_list{};
    
    // size
    //fwd decl
    template<typename... Types>
    struct type_list_size;
    
    template<>
    struct type_list_size<type_list<>>
    {
        static constexpr std::size_t value = 0;
    };
    
    template<typename Head, typename... Tail>
    struct type_list_size<type_list<Head, Tail...>>
    {
        static constexpr std::size_t value = 1 + type_list_size<type_list<Tail...>>::value;
    };
    
    template<typename... Types>
    struct type_list_size
    {
        static constexpr std::size_t value = type_list_size<type_list<Types...>>::value;
        // static constexpr std::size_t value = sizeof...(Types);
    };
    
    
    // Index
    // it will not even compile for out of bounds index
    template<std::size_t Index, typename... TypeList>
    struct type_at_index;
    
    template<std::size_t Index, typename Head, typename... Tail>
    struct type_at_index<Index, type_list<Head, Tail...>>
    {
        typedef typename type_at_index<Index-1, type_list<Tail...>>::result    result;
    };
    
    template<typename Head, typename... Tail>
    struct type_at_index<0, type_list<Head, Tail...>>
    {
        typedef Head    result;
    };
    
    template<std::size_t Index, typename... Types>
    struct type_at_index
    {
        typedef typename type_at_index<Index, type_list<Types...>>::result    result;
    };
    
    // Prepend
    template<typename ToPrepend, typename... Types>
    struct prepend
    {
        typedef type_list<ToPrepend, Types...> result;
    };
    
    template<typename ToPrepend, typename... Types>
    struct prepend<ToPrepend, type_list<Types...>>
    {
        typedef type_list<ToPrepend, Types...> result;
    };

    template<typename ToPrepend, typename... Types>
    struct prepend<type_list<Types...>, ToPrepend>
    {
        typedef type_list<Types..., ToPrepend> result;
    };
    
    
    // Append
    template<typename ToAppend, typename... Types>
    struct append
    {
        typedef type_list<Types..., ToAppend> result;
    };
    
    template<typename ToAppend, typename... Types>
    struct append<ToAppend, type_list<Types...>>
    {
        typedef type_list<Types..., ToAppend> result;
    };
    
    template<typename ToAppend, typename... Types>
    struct append<type_list<Types...>, ToAppend>
    {
        typedef type_list<ToAppend, Types...>   result;
    };
    
}
#endif /* bc_type_list_h */
