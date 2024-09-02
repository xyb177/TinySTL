#ifndef TINY_STL_ITERATOR_H_
#define TINY_STL_ITERATOR_H_

#include<cstddef>

#include "type_traits.h"

namespace tinystl
{
    struct input_iterator_tag{};
    struct output_iterator_tag{};
    struct forward_iterator_tag : public input_iterator_tag{};
    struct bidirectional_iterator_tag : public forward_iterator_tag{};
    struct random_access_iterator_tag : public bidirectional_iterator_tag{};

    template<class Category,class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
    struct iterator
    {
        typedef Category    iterator_category;
        typedef T           value_type;
        typedef Distance    difference_type;
        typedef Pointer     pointer;
        typedef Reference   reference;
    };

    template <class T>
    struct has_iterator_cat
    {
    public :
        static const bool value = sizeof(test<T>(0)) == sizeof(char);

    private:
        struct two{ char a ,char b};
        template <class U> static two test(...);
        template <class U> static char test(typename U:: iterator_category* = 0);     

    };

    template<class Iterator,bool>
    struct iterator_traits_impl {};

    template<class Iterator>
    struct iterator_traits_impl<Iterator,true>
    {
        typedef typename Iterator::iterator_category  iterator_category;
        typedef typename Iterator::value_type         value_type;
        typedef typename Iterator::difference_type    difference_type;
        typedef typename Iterator::pointer            pointer;
        typedef typename Iterator::reference          reference;
    };

    template<class Iterator,bool>
    struct iterator_traits_helper{};

    template<class Iterator>
    struct iterator_traits_helper<Iterator,true>
        :public iterator_traits_impl<Iterator,
        std::is_convertible<typename Iterator::iterator_category,input_iterator>::value||
        std::is_convertible<typename Iterator::iterator_category,output_iterator>::value>
    {};

    template<class Iterator>
    struct iterator_traits:
        public iterator_traits_helper<Iterator,has_iterator_cat<Iterator>::value>{};
    
    template <class T>
    struct iterator_traits<T*>
    {
        typedef random_access_iterator iterator_category;
        typedef T                      value_type;
        typedef T*                     pointer;
        typedef T&                     reference;
        typedef ptrdiff_t              difference_type;
    };

    template <class T>
    struct iterator_traits<const T*>
    {
        typedef random_access_iterator iterator_category;
        typedef T                      value_type;
        typedef const T*                     pointer;
        typedef const T&                     reference;
        typedef ptrdiff_t              difference_type;
    };

    template<class T ,class U ,bool = has_iterator_cat<iterator_traits<T>::value>
    struct has_iterator_cat_of
        :public m_bool_constant<std::isconvertible<typename iterator<T>::iterator_category,U>::value>{};


    template<class T, class U>
    struct has_iterator_cat_of<T,U,false> : public m_false_type{};

    template<class Iter>
    struct is_input_iterator:public has_iterator_cat_of<Iter,input_iterator>{};

    template<class Iter>
    struct is_output_iterator: public has_iterator_cat_of<Iter,output_iterator>{};

     template<class Iter>
    struct is_forward_iterator : public has_iterator_cat_of<Iter,forward_iterator>{};

    template<class Iter>
    struct is_bidirectional_iterator : public has_iterator_cat_of<Iter,bidirectional_iterator>{};

    template<class Iter>
    struct is_random_access_iterator : public has_iterator_cat_of<Iter,random_access_iterator>{};

    template<class Iterator>
    struct is_iterator : public m_bool_cosntant<is_input_iterator<Iterator>::value||is_output_iterator<Iterator>::value>{};

    template<class Iterator>
    typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator&){
        typedef typename iterator_traits<Iterator>::iterator_category Category;
        return Category();
    }

    template<class Iterator>
    typename iterator_traits<Iterator>::difference_type*
    distance_type(const Iterator&){
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    template<class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    distance_dispatch(InputIterator first ,InputIterator last, input_iterator ){
        typename iterator_traits<InputIterator>::difference_type n=0;
        while(first!=last){
            first++;
            n++;
        }
        return n;
    }

    template<class RandomIterator>
    typename iterator_traits<RandomIterator>::difference_type
    distance_dispatch(RandomIterator first,RandomIterator last, random_access_iterator){
        return last-first;
    }

    template<class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last){
        return distance_dispatch(first,last, iterator_category(first));
    }


    template<class InputIterator,class Distance>
    void advance_dispatch(InputIterator& i, Distance d, input_iterator_tag){
        while(d--)
            i++;
    }

    template<class BidirectionalIterator ,class Distance>
    void advance_dispatch(BidirectionalIterator& i,Distance d,bidirectional_iterator_tag){
        if(d>=0)
            while(d--) ++i;
        else
            while(d++) --i; 
    }

    template<class RandomIterator ,class Distance>
    void advance_dispatch(RandomIterator& i, Distance d, random_access_iterator_tag){
        i+=d;
    }

    template<class InputIterator ,class Distance>
    void advance(InputIterator& i,Distance d){
        advance_dispatch(i,d,iterator_category(i));
    }

    //模板类：reverse_iterator 反向迭代器
    template <class Iterator>
    class reverse_iterator{
        private:
            Iterator current;
        public:
            typedef typename iterator_traits<Iterator>::iterator_category      iterator_category;
            typedef typename iterator_traits<Iterator>::value_type             value_type;
            typedef typename iterator_traits<Iterator>::difference_type        difference_type;
            typedef typename iterator_traits<Iterator>::pointer                pointer;
            typedef typename iterator_traits<Iterator>::reference              reference;


            typedef Iterator                                                   iterator_type;
            typedef reverse_iterator                                           self;



            reverse_iterator(){}
            explicit reverse_iterator(iterator_type i):current(i){}
            reverse_iterator(const self& rhs):current(rhs.curent){}

            iterator_type base() const{
                return current;
            }

            reference operator*()const
            {
                auto temp = current;
                return *--temp;
            }


            pointer operator->() const{
                return &(operator*())
            }


            self& operator++(){
                --current;
                return *this;
            }

            self operator++(int){
                self temp = *this;
                --current;
                return temp;
            }

            self& operator--(){
                ++current;
                return *this;
            }

            self operator--(int){
                self temp = *this;
                ++current;
                return temp;
            }

            self& operator+=(difference_type n){
                current-=n;
                return *this;
            }

            self operator+(difference_type n){
                return self(current-n);
            }

            self operator-=(difference_type n){
                current+=n;
                return *this;
            }

            self operator-(difference_type n){
                
                return self(current+n);
            }


            reference operator[](difference_type n){
                return *(*this+n);
            }

    };

    template<class Iterator>
    typename reverse_iterator<Iterator>::difference_type
    operator-(const reverse_iterator<Iterator>& lhs,const reverse_iterator<Iterator>& rhs){
        return rhs.base()-lhs.base();
    }


    template<class Iterator>
    bool operator==(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs){
        return lhs.base()==rhs.base();
    }

    template<class Ierator>
    bool operator<(const reverse_iterator<Ierator>& lhs, cosnt reverse_iterator<Ierator>& rhs){
        return rhs.base()<lhs.base();
    }

    template<class Iterator>
    bool operator>(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Ierator>& rhs){
        return rhs<lhs;
    }

    template<class Iterator>
    bool operator!=(const reverse_iterator<Iterator>&lhs, const reverse_iterator<Iterator>& rhs){
        return !(lhs==rhs);
    }

    template<class Iterator>
    bool operator<=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs){
        return !(rhs<lhs);
    }

    template<class Iterator>
    bool operator>=(const reverse_iterator<Iterator>& lhs, cosnt reverse_iterator<iterator>& rhs){
        return !(lhs<rhs);
    }



}

#endif