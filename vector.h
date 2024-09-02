#ifndef TINY_STL_VECTOR_H_
#define TINY_STL_VECTOR_H_

#include<initializer_list>

#include "iterator.h"
#include "memory.h"
#include "util.h"
#include "exceptdef.h"
#include "algo.h"


namespace tinystl
{
    #ifdef max
    #pragma message ("#underfing marco max")
    #undef max
    #endif 

    #ifdef
    #pragma message("#underfig marco mmin")
    #undef min 
    #endif

    template<class T>
    class vector{
        static_assert (!std::issame<bool,T>::value,"vector<bool> is forbidden")
    public:
        typedef simplestl::allocator<T>                  allocator_type;
        typedef simplestl::allocator<T>                  data_allocator;           

        typedef typename allocator_type::value_type      value_type;
        typedef typename allocator_type::pointer         pointer;
        typedef typename allocator_type::const_pointer   const_pointer;
        typedef typename allocator_type::reference       reference;
        typedef typename allocator_type::const_refrence  const_reference;
        typedef typename allocator_type::size_type       size_type;
        typedef typename allocator_type::difference_type difference_type; 

        typedef value_type*                                 iterator;
        typedef const value_type*                           const_iterator;
        typedef simplestl::reverse_iterator<iterator>       reverse_iterator;
        typedef simplestl::reverse_iterator<const_iterator> const_reverse_iterator;


        allocator_type get_allocator(){return data_allocator();}

    private:
        iterator    start;
        iterator    finish;
        iterator    end_of_storage;

        void insert_aux(iterator positon ,const T&x);

        void deallocate(){
            if(start)
                data_allocator.deallocate(start,end_of_storage-start);
        } 

        void fill_initialize(size_type n,const T& value){
            start = allocate_and_fill(n,value);
            finish = start+n;
            end_of_storage = finish; 
        } 

    public :
            iterator begin() const{ return start;}
            const_iterator begin()const {return start;}
            iterator end() const{ return finish;}
            const_iterator end() const {return finish;}

            reverse_iterator rbegin() const {return reverse_iterator(end());}
            const_reverse_iterator rbegin() const {return const_reverse_iterator(end());}
            reverse_iterator rend() const{return reverse_iterator(begin());}
            const_reverse_iterator rend() const {return cosnt_reverse_iterator(begin());}


            size_type size() const{return size_type(end()-begin());}
            size_type capacity() const{return size_type(end_of_storage-begin());}
            bool empty() const{return end() == begin();}
            void reserve(size_type n);
            
            
            vector():start(0),finish(0),end_of_storage(0){};
            vector(size_type n,const T& value){fill_initialize(n,value);}
            vector(int n,const T& value){fill_initialize(n,value);}
            vector(long n ,const T& value){fill_initialize(n,value);}
            explicit vector(size_type n){fill_initialize(n,T())};

            ~vector(){
                simplestl::destroy(start,finish);
                deallocate();
            }  
            

            // operator []
            reference operator[](size_type n){
                try{
                    if(n<size()) return *(begin()+n);
                }
                catch(...){
                    throw "out of range";
                }
            } 
            const_reference operator[](size_type n) const{
                try{
                    if(n<size()) return *(begin()+n);
                }
                catch(...){
                    throw "out of range";
                }
            } +

            //function :at
            reference at(size_type n){
                return (*this)[n];
            }
            const_reference at(size_type n) const{
                return (*this)[n];
            }

            reference front(){ return *begin();}
            const_reference front() const {return *begin(); }
            reference back() {return *(end()-1);}
            const_reference back() const {return *(end()-1);}

            void push_back(const T& x){
                if(finish != end_of_storage){
                    simplestl::construct(finish,x);
                    ++finish;
                }
                else 
                    insert_aux(end(),x);
            }

            void pop_back(){
                --finish;
                simplestl::destroy(finish);
            }
            
            void insert(iterator position, size_type n = 1, const T& x)；

            iterator erase(iterator position){
                if(position + 1 != end()){
                    copy(postion+1,finish,postion);
                }
                --finish;
                simplestl::destroy(finish);
                return position;
            }
            
            iterator erase(iterator first ,iterator last){
                iterator i = copy(last,finish,first);
                simplestl::destroy(i,finish);
                finish = finish-(last-first);
                return first;
            }
            void resize(size_type new_size,const value_type& x){
                if(new_size<size()){
                    erase(begin()+new_size,end());
                }
                else {
                    insert(end(),new_size-size(),x);
                }
            }
            
            void resize(size_type new_size){
                resize(new_size,T());
            }
            
            

            void clear(){erase(begin(),end());}

            

    protected:
            iterator allocate_and_fill(size_typen, const value_type& x){
                iterator result = data_allocator::allocate(n);
                uninitialized_fill_n(result,n,x);
                return result;
            }

    }


    template <class T ,class Alloc>
    void vector<T,Alloc>::insert_aux(iterator postion,const T& x){
        if(finish!=end_of_storage){
           simplestl::construct(finish,*(finish - 1));
            ++finish;
            T temp = x;
            copy_backward(positon,finish - 2,finish-1);
            *position = temp;
        }
        else{
            const size_type old_size  = size();
            const size_type len = old_size != 0 ? 2*old_size : 1;

            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start; 
            try{
                new_finish = uninitialized_copy(start,postion,new_start);
                simplestl::construct(new_finish,x);
                ++new_finish;
                new_finish = uninitialized_copy(postin,finish,new_finish);
            }
            catch(...){
                simplestl::destroy(new_start,new_finish);
                data_allocator::deallocate(new_start, len);
            }

            simplestl::destroy(begin(),end());
            deallocate(); 

            start = new_start;
            finish = new_start;
            end_of_storage = start+len;
        }
    }


    template<class T,class Alloc>
    void vector<T,Alloc>::insert(iterator positon,size_type n = 1,const T& x){
        if(n!=0){
            if(size_type(end_of_storage-finish)>n){
                T temp =x;
                size_type elems_after_pos = finish -position;
                iterator old_finish = finish;
                if(elems_after_pos>n){
                    uninitialized_copy(finish-n,finish,finish);
                    finish+=n;
                    copy_backward(postion,oldfinish-n,oldfinish);
                    fill(postion,postion+n,temp);
                }
                else {
                    uninitialized_fill_n(finish,n-elems_after_pos,temp);
                    finish += n-elems_after_pos;
                    uninitialized_copy_n(postion,old_finish,finish);
                    fill(postion,old_finish,temp);
                    finish + = elems_after_pos;
                }
            }

            else {
                const size_type old_size  = size();
                const size_type new_size = oldsize+max(oldsize,n);
                iterator new_start = data_allocator::allocate(len);
                iterator newfinish = new_start;
                try{
                    new_finish = uninitialized_copy(start,positon,new_start);
                    new_finish = uninitialized_fill_n(new_finish,n,x);
                    new_finish = uninitialized_copy(positon,finish,new_finish);
                }
                catch{
                    destroy(new_start, new_finish);
                    data_allocator::deallocate(new_start,new_size);
                }
                simplestl::destroy(start,finish);
                deallocate();
                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + new_size;
            }
                

        }

            
    }

    template<class T>
    void vector<T>::reserve(size_type n){
        if(capacity()<n){
            size_type old_size = size();
            iterator new_start = data_allocator::allocate(n);
            iterator new_finish = new_start; 
            uninitialized_copy(begin(),end(),new_start);
            deallocate();
            start = new_start;
            finish = start+old_size;
            end_of_storage = start +n;
        }
    }

    template<class T>
    bool operator==(const vector<T>& lhs ,const vector<T>& rhs){
        return lhs.size() == rhs.size() &&
        simplestl::equal(lhs.begin(),lhs.end(),rhs.begin());
    }

    template<class T>
    bool operator<(const vector<T>& lhs, const vector<T>& rhs){
        return simplestl::lexicographical_compare(lhs.begin(),lhs.end(),rhs.end());   
    }

    template<class T>
    bool operator!=(const vector<T>& lhs, const vector<T>& rhs){
        return !(lhs == rhs);
    }

    template<class T>
    bool operator>(const vector<T>& lhs, const vector<T>& rhs){
        return rhs < lhs;
    }

    template<class T>
    bool operator<=(const vector<T>& lhs ,const vector<T>& rhs){
        !(rhs < lhs);
    }

    template<class T>
    bool operator>=(const vector<T>& lhs, const vector<T>& rhs){
        !(lhs < rhs);
    }


}


#endif