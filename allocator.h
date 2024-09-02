#ifndef TINY_STL_ALLOCATOR_H_
#define TINY_STL_ALLOCATOR_H_

#include "construct.h"
#include "util.h"

namespace tinystl{

template<class T>
class allocator{
pubilc:
    typedef T         value_type;
    typedef T*        pointer;
    typedef const T*  const_pointer;
    typedef T&        reference;
    typedef const T&  const_reference;
    typedef size_t    size_type;
    typedef ptrdiff_t difference_type; 

    static T* allocate();
    static T* allocate(size_type n);

    static void deallocate(T* ptr);
    static void deallocate(T* ptr, size_type n);

    static void construct(T* ptr);
    static void construct(T* ptr ,const T& val);
    static void construct(T* ptr , T&& val);
    
    template<class... Args>
    static void construct(T* ptr, Args&& ...args);

    static void destroy(T* ptr);
    static void destroy(T* first ,T* last);

};

template<class T>
T* allocator<T>::allocate(){
    return static_cast<T*>(::operator new(sizeof(T)));
}

template<class T>
T* allocator<T>::allocate(size_type n){
    if(n==0) return nullptr;
    return static_cast<T*>(::operator new(sizeof(T)*n));
}

template<class T>
void allocator<T>::deallocate(T* ptr){
    if(ptr==nullptr) return ; 
    ::operator delete(ptr);  
}

template<class T>
void allocator<T>::deallocate(T* ptr,size_type n){
    if(ptr==nullptr) return ; 
    ::operator delete(ptr);  
}

template<class T>
void allocator<T>::construct(T* ptr){
    simplestl::construct(ptr);
}

template<class T>
void allocator<T>::construct(T* ptr, const T& val){
    simplestl::construct(ptr,val);
}

template<class T>
void allocator<T>::construct(T* ptr, T&& val){
    simplestl::construct(ptr,simplestl::move(val));
}

template<class T>
template<class ...Args>
void allocator<T>::construct(T* ptr, Args&& ...args){
    simplestl::construct(ptr,simplestl::forward<Args>(args)...);
}

template<class T>
void allocator<T>::destroy(T* ptr){
    simplestl::destroy(ptr);
}

template<class T>
void allocator<T>::destroy(T* first,T* last){
    simplestl::destroy(first,last);
}


}

#endif