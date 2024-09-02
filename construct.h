#ifndef TINY_STL_CONSTRUCT_H_
#define TINY_STL_CONSTRUCT_H_ 

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100) //编译时不会出现编号为4100的warning
#endif


#include<new> //new delete 等方法包含在该头文件中

#include "type_traits.h"
#include "iterator.h"

namespace tinystl
{

 template<class T>
 void construct(T* ptr){
    ::new((void*)ptr) T(); //定位new运算符 ，在ptr所指定的地址new一个T类

 }

 template<class T1, class T2>
 void construct(T1* ptr, const T2& val){
   ::new((void*)ptr) T1(val);
 }

 template<class T , class... Args>
 void construct(T* ptr ,Args&& ...arg){
    ::new((void*)ptr) T(std::forward<Args>(arg)...);
 }


 //
 template<class T>
 void destroy_one(T*,std::true_type){}
 
 template<class T>
 void destroy_one(T* ptr,std::false_type){
    if(ptr!=nullptr){
        ptr->~T();
    }
 }
 

 template<class T>
 void destroy(T* ptr){
    destroy_one(ptr,std::is_trivially_destructible<T>{});
 }

template<class Forward_iter>
void destroy(Forward_iter first, Forward_iter last){
    destroy_cat(first,last,std::is_trivially_destructible<typename iterator_traits<Forward_iter>>{});
}

 template<class Forward_iter>
 void destroy_cat(Forward_iter , Forwrd_iter ,std::true_type){}
 
 template<class Forward_iter>
 void destroy_cat(Forward_iter first , Forward_iter  last, std::false_type){
    for(;first!=last;++first){
           destroy(&*first);
    }
 }

//特化版本
void destroy(char* , char*){}
void destroy(wchar_t* ,wchar_t*){}








}


#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif