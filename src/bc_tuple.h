//
//  tuple.h
//  CPP_Playground
//
//  Created by Burak Can on 6/26/21.
//  Copyright © 2021 Burak Can. All rights reserved.
//

#ifndef tuple_h
#define tuple_h

#include <cstddef>
#include "bc_type_traits.h"

namespace bc
{
    template<typename...>
    class tuple;
    
    template<>
    class tuple<>{};
    
    template<typename Head, typename... Tail>
    class tuple<Head, Tail...> : private tuple<Tail...>
    {
    private:
        typedef tuple<Tail...> inherited;
        Head m_head;
    public:
        tuple(){}
        tuple(typename bc::add_const_reference<Head>::type v,
              typename bc::add_const_reference<Tail>::type... v_tail)
        : m_head(v), inherited(v_tail...)
        {
            
        }
        
        template<typename... OtherTypes>
        tuple(const tuple<OtherTypes...>& other) : m_head(other.head()), inherited(other.tail())
        {
            
        }
        
        template<typename... OtherTypes>
        tuple& operator=(const tuple<OtherTypes...>& other)
        {
            m_head = other.head();
            tail() = other.tail();
            return *this;
        }
        
        typename bc::add_reference<Head>::type head() { return m_head; }
        typename bc::add_const_reference<Head>::type head() const { return m_head; }
        inherited& tail() { return *this; }
        const inherited& tail() const { return *this; }
        
    };
    
    // Helpers
    // tuple_result
    template<typename T>
    struct make_tuple_result {typedef T type;};
    
    template<typename T>
    struct make_tuple_result<reference_wrapper<T> > { typedef T& type; };
    
    template<typename... Args>
    tuple<typename make_tuple_result<Args>::type...>
    make_tuple(const Args&... values)
    {
        return bc::tuple<typename make_tuple_result<Args>::type...>(values...);
    }
    
    // tuple_size
    template<typename Tuple>
    struct tuple_size;
    
    template<typename... Types>
    struct tuple_size<tuple<Types...>> : public bc::integral_constant<std::size_t, sizeof...(Types)>{};
    
    // tuple_element
    /*
     retrieves the type of theIthvalue.
     We again use a recursive algorithm, butthis time we terminate whenIhits zero:
     */
    template<std::size_t index, typename Tuple>
    struct tuple_element;
    
    template<std::size_t index, typename Head, typename... Tail>
    struct tuple_element<index, tuple<Head, Tail...>>
    {
        typedef typename tuple_element<index - 1, tuple<Tail...>>::type     type;
    };
    
    template<typename Head, typename... Tail>
    struct tuple_element<0, tuple<Head, Tail...>>
    {
        typedef Head type;
    };
    
    // Element access
    namespace impl
    {
        template<std::size_t Index, typename Tuple>
        class get_impl;
        
        template<std::size_t Index, typename Head, typename... Tail>
        class get_impl<Index, tuple<Head, Tail...> >
        {
            typedef typename tuple_element<Index-1, tuple<Tail...>>::type   Element;
            typedef typename bc::add_reference<Element>::type               Ref_Element;
            typedef typename bc::add_const_reference<Element>::type         Const_Ref_Element;
            typedef get_impl<Index - 1, tuple<Tail...> >                    Next;
        public:
            static Ref_Element          get(tuple<Head, Tail...>& t)        { return Next::get(t.tail()); }
            static Const_Ref_Element    get(const tuple<Head, Tail...>& t)  { return Next::get(t.tail()); }
        };
        
        // termination case
        template<typename Head, typename... Tail>
        class get_impl<0, tuple<Head, Tail...>>
        {
            typedef bc::add_reference_t<Head>           Reference;
            typedef bc::add_const_reference_t<Head>     Const_Reference;
        public:
            static Reference        get(tuple<Head, Tail...>& t)                { return t.head(); }
            static Const_Reference  get(const tuple<Head, Tail...>& t)          { return t.head(); }
        };
    }
    
    template<std::size_t Index, typename... Types>
    bc::add_reference_t<typename tuple_element<Index, tuple<Types...>>::type >
    get(tuple<Types...>& t)
    {
        return impl::get_impl<Index, tuple<Types...>>::get(t);
    }
    
    template<std::size_t Index, typename... Types>
    bc::add_const_reference_t<typename tuple_element<Index, tuple<Types...>> :: type>
    get(const tuple<Types...>& t)
    {
        return impl::get_impl<Index, tuple<Types...>>::get(t);
    }
}
#endif /* tuple_h */
