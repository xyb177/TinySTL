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


}

#endif
