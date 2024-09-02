#ifndef TINY_STL_ALLOCATOR_H_
#define TINY_STL_ALLOCATOR_H_

#include<type_traits>
namespace tinystl{
    template<class T , T v>
    struct m_integral_constant
    {
        static constexpr T value = v;
    };

    template <bool b>
    using m_bool_constant = m_integral_constant <bool ,b>;

    typedef m_bool_constant<true> m_true_type;
    typedef m_bool_constant<false> m_false_type;


    template <class T>
    struct is_pair : simplestl ::m_false_type{};

    template<class T1 ,class T2>
    struct is_pair<simplestl::pair<T1,T2>> : simplestl :: m_true_type {};
}

#endif