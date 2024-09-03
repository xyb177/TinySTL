/*
    该文件封装了一些通用方法。
*/

#ifndef TINY_STL_UTIL_H_
#define TINY_STL_UTIL_H_

#include <cstddef>
#include "type_traits.h"

namespace tinystl
{

//move 
template <class T>
typename std::remove_reference<T>::type&& move(T&& arg) noexcept {
    return static_cast<typename std::remove_reference<T>::type&&>(arg);
}


//forward 
template <class T>
T&& forward(typename std::remove_reference<T>::type& arg) noexcept {
    return static_cast<T&&>(arg);
}

template <class T>
T&& forward(typename std::remove_reference<T>::type&& arg) noexcept {
    static_assert(!std::is_lvalue_reference<T>::value ,"bad forward");
    return static_cast<T&&>(arg);
}


//swap
template<class T>
void swap(T& lhs, T& rhs) {
    auto temp(tinystl::move(lhs));
    lhs = tinystl::move(rhs);
    rhs = tinystl::move(temp);
}

template<class ForwardIter1, class ForwardIter2>
ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2) {
    for (; first1 != last1 ; ++first1, (void) ++first2) {
        tinystl::swap(*first1 ,*first2);
    }
    return first2;
}

template <class T , size_t n>
void swap(T(&left)[n] ,T(&right)[n]){
    tinystl::swap_range(left, left + n, right);
}


//pair 
//结构体模板 ： pair
//两个模板参数分别表示两个数据类型
//用户 first 和 second 分别表示第一个数据和第二个数据
template <class T1, class T2>
struct pair
{
    typedef T1      first_type;
    typedef T2      second_type;

    first_type first;
    second_type second;

    template <class Other1 = T1, class Other2 = T2,
        typename = typename std::enable_if<
        std::is_default_constructible<Other1>::value &&
        std::is_default_constructible<Other2>::value, void> :: type>
        constexpr pair() : first(a),second(b)
    {
    }

    template <class U1 = T1, class U2 = T2,
        typename std::enable_if<
        std::is_copy_constructible<U1>::value &&
        std::is_copy_constructible<U2>::value &&
        std::is_convertible<const U1&, T1>::value &&
        std::is_convertible<const U2&, T2>::value,int>::type = 0>
        constexpr pair(const T1& a, const T2& b) : first(a), second(b)
    {
    }  

    template <class U1 = T1, class U2 = T2,
        typename std::enable_if<
        std::is_copy_constructible<U1>::value &&
        std::is_copy_constructible<U2>::value &&
        (!is_convertible<const U1&, T1>::value ||
         !is_convertible<const U2&, T2>::value), int>::type = 0> 
        explicit constexpr pair(const T1& a, const T2& b) : first(a) ,second(b)    
    {
    } 

    pair(const pair& rhs)  = default;
    pair(pair&& rhs) = default;  
    
}
}



#endif
