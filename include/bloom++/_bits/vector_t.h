/* 
 * Copyright © 2015 Sergei Khairulin <sergei.khairulin@gmail.com>. 
 * All rights reserved.
 *
 * This file is part of Bloom++.
 *
 * Bloom++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bloom++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bloom++.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <bloom++/_bits/c++config.h>
#include <bloom++/_bits/traits.h>
#include <bloom++/exception>

#ifdef AUX_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

/**
 * Vector exception.
 */
class vector_exception: public exception
{
public:
    vector_exception(string msg):exception(msg){}
    virtual ~vector_exception() throw() {}
};

/**
 * @brief The vector class.
 */
template<class vT, class Traits=traits<vT> >
class vector_t
{
public:
    typedef class vector_t<vT, Traits>          Self;
    
private: 
    /// @cond
    
    /*
     * Vector representation.
     */
    struct rep_base {
        size_t size_;
        size_t capacity_;
        int refcount_; //Atomic word
    };
        
    struct rep: rep_base
    {
        static rep_base rep_empty_;
        
        inline static rep* create(size_t size, size_t old_capacity) FORCE_INLINE {
            if(!size)return static_cast<rep*>(&rep_empty_);
            
            const size_t pagesize = 4096;
            const size_t malloc_header_size = 4 * sizeof(void*);
            
            //size_t capacity = ((size_t)(size/cap_step))*cap_step + cap_step;
            size_t capacity = size;
            
            if ( capacity > old_capacity && capacity < 2 * old_capacity)
                capacity = 2 * old_capacity;
            
            size_t alloc_size = capacity * sizeof(vT) + sizeof(rep);

            const size_t adj_size = alloc_size + malloc_header_size;
            if (adj_size > pagesize) {
                const size_t extra = pagesize - adj_size % pagesize;
                capacity += extra / sizeof(vT);
                alloc_size = capacity * sizeof(vT) + sizeof(rep);
            }
            
            rep *r = (rep *)malloc(alloc_size);
            r->size_ = size;
            r->capacity_ = capacity;
            r->refcount_ = 0;
            return r;
        }
        
        inline static rep* create_and_construct(size_t size, size_t old_capacity, const vT &v = vT()){
            rep *r = create(size, old_capacity);
            Traits::construct(r->data(), size, v);
            return r;
        }
        
        inline vT* data() FORCE_INLINE {
            return reinterpret_cast<vT*>(this+1);
        }
        
        inline rep* getRef() FORCE_INLINE {
            if(this != static_cast<rep*>(&rep_empty_))
                ++this->refcount_;
            return this;
        }
        
        inline void releaseRef() FORCE_INLINE {
            if(this != static_cast<rep*>(&rep_empty_)){
                if(this->refcount_)
                    --this->refcount_;
                else {
                    Traits::destroy(this->data(), this->size_);
                    free(this);
                }
            }
        }
        
        inline rep *clone() FORCE_INLINE {
            rep *r = (rep *)malloc(this->capacity_ * sizeof(vT) + sizeof(rep));
            memcpy(reinterpret_cast<void*>(r), reinterpret_cast<void*>(this), 
                   sizeof(rep));
            Traits::copy_construct(r->data(), this->data(), this->size_);
            this->releaseRef();
            return r;
        }
        
        inline rep *clone_for_insert(size_t new_size, size_t insert_index, size_t insert_size) FORCE_INLINE {
            rep *r = create(new_size, this->capacity_);
            
            if(insert_index >= this->size_){
                Traits::copy_construct(r->data(), this->data(), this->size_);
            }
            else {
                if(insert_index > 0)
                    Traits::copy_construct(r->data(), this->data(), insert_index);
                Traits::copy_construct(r->data() + insert_index + insert_size, 
                             this->data() + insert_index, 
                             this->size_ - insert_index);
            }
            this->releaseRef();
            return r;
        }
        
        inline rep *clone_for_append(size_t new_size) FORCE_INLINE {
            rep *r = create(new_size, this->capacity_);

            Traits::copy_construct(r->data(), this->data(), this->size_);
            
            this->releaseRef();
            return r;
        }
        
        inline rep *clone_and_pop_back(size_t new_size) FORCE_INLINE {
            rep *r = create(new_size, this->capacity_);

            Traits::copy_construct(r->data(), this->data(), new_size);
            
            this->releaseRef();
            return r;
        }
        
        inline rep *clone_and_resize(size_t new_size, vT c) FORCE_INLINE {
            rep *r = create(new_size, this->capacity_);

            Traits::copy_construct(r->data(), this->data(), new_size < this->size_ ? new_size : this->size_);
            
            if(new_size > this->size_)
                Traits::construct(r->data() + this->size_, new_size - this->size_, c);
            
            this->releaseRef();
            return r;
        }
        
        inline rep *clone_for_replace(size_t replace_index, size_t replace_size) FORCE_INLINE {
            rep *r = create(this->size_, this->capacity_);
            
            if(replace_index > 0)
                Traits::copy_construct(r->data(), this->data(), replace_index);
            if(replace_index < rep_->size_)
                Traits::copy_construct(r->data() + replace_index + replace_size, 
                             this->data() + replace_index + replace_size, 
                             this->size_ - replace_index - replace_size);
            this->releaseRef();
            return r;
        }
        
        inline rep* clone_and_erase(const size_t new_size, const size_t erase_index, const size_t erase_size) FORCE_INLINE {
            const size_t next = erase_index + erase_size;
            rep *r = create(new_size, this->capacity_);
            if(erase_index > 0)
                Traits::copy_construct(r->data(), this->data(), erase_index);
            if(next < this->size_)
                Traits::copy_construct(r->data() + erase_index, this->data() + next, this->size_ - next);
            this->releaseRef();
            return r;
        }

        inline void erase_data(const size_t new_size, const size_t erase_index, const size_t erase_size) FORCE_INLINE {
            const size_t next = erase_index + erase_size;
            if(next < this->size_)
                Traits::copy(this->data() + erase_index, this->data() + next,
                             this->size_ - next);
            Traits::destroy(this->data() + this->size_ - erase_size, erase_size);
            this->size_ = new_size;
        }
        
    } *rep_;
    
    inline void append_prepare(size_t size) FORCE_INLINE {
        const size_t new_size = rep_->size_ + size;
        if(rep_->refcount_ || new_size > rep_->capacity_)
            rep_ = rep_->clone_for_append(new_size);
        else {
            //Append data to vector
            rep_->size_ = new_size;
        }
    }
    
    inline bool insert_prepare(size_t index, size_t size) FORCE_INLINE {
        size_t new_size = rep_->size_+size;
        if(rep_->refcount_ || new_size > rep_->capacity_){
            rep_ = rep_->clone_for_insert(new_size, index, size);
            return true; // need construct on insert data
        }
        else {
            if(index < rep_->size_){
                //Move data to free space for insert
                const size_t data_after = rep_->size_ - index;
                if(size < data_after){
                    Traits::copy_construct(rep_->data() + rep_->size_,
                                           rep_->data() + rep_->size_ - size,
                                           size);
                    Traits::move(rep_->data() + index + size,
                                 rep_->data() + index, rep_->size_ - index - size);
                    rep_->size_ = new_size;
                    return false; // not need construct on insert data
                }
                if(size == data_after){
                    Traits::copy_construct(rep_->data() + rep_->size_,
                                           rep_->data() + rep_->size_ - size,
                                           size);
                    rep_->size_ = new_size;
                    return false; // not need construct on insert data
                }
                Traits::move_construct(rep_->data() + index + size, 
                                       rep_->data() + index, 
                                       data_after);
                rep_->size_ = new_size;
                return true; // need construct on insert data
            }
            rep_->size_ = new_size;
            return true; // need construct on insert data
        }
    }

    /// @endcond
    
protected:
    
    vector_t() throw():
    rep_(rep::create(0, 0)){
    }
    
    vector_t(size_t size, const vT &v = vT()) throw():
    rep_(rep::create_and_construct(size, 0, v)){
    }
    
    vector_t(const Self &vec) throw():
    rep_(vec.rep_->getRef()){
    }
    
    ~vector_t() throw(){
        /// @cond
        rep_->releaseRef();
        /// @endcond
    }
    
    void assign(const Self &vec){
        /// @cond
        rep_->releaseRef();
        rep_ = vec.rep_->getRef();
        /// @endcond
    }

    void append(const vT *vec, size_t size) {
        /// @cond
        if(!size)return *this;
        //const size_t old_size = rep_->size_;
        append_prepare(size);
        Traits::copy_construct(rep_->data() + rep_->size_ - size, vec, size);
        /// @endcond
    }
    
    void append(const Self &vec){
        /// @cond
        if(!vec.rep_->size_)return *this;
        const size_t old_size = rep_->size_;
        const size_t size = vec.rep_->size_;
        append_prepare(vec.rep_->size_);
        Traits::copy_construct(rep_->data() + old_size, vec.rep_->data(), size);
        /// @endcond
    }
    
    void insert(size_t index, const vT *values, size_t size) {
        /// @cond
        if(!size)return;
        if(index > rep_->size_)
            throw vector_exception("vector_t::insert: index > rep_->size_"); //throw
        if(insert_prepare(index, size))
            Traits::copy_construct(rep_->data() + index, values, size);
        else
            Traits::copy(rep_->data() + index, values, size);
        /// @endcond
    }
    
    void insert(size_t index, const Self &vec) {
        /// @cond
        if(!vec.rep_->size_)return *this;
        if(index > rep_->size_)
            throw vector_exception("vector_t::insert: index > rep_->size_"); //throw
        if(&vec == this){
            const size_t size = vec.rep_->size_;
            if(insert_prepare(index, vec.rep_->size_)){
                Traits::copy_construct(rep_->data() + index, rep_->data(), index);
                Traits::copy_construct(rep_->data() + index * 2, rep_->data() + index + size, size - index);
            }
            else {
                Traits::copy(rep_->data() + index, rep_->data(), index);
                Traits::copy(rep_->data() + index * 2, rep_->data() + index + size, size - index);
            }
        }
        else {
            if(insert_prepare(index, vec.rep_->size_))
                Traits::copy_construct(rep_->data() + index, vec.rep_->data(), vec.rep_->size_);
            else
                Traits::copy(rep_->data() + index, vec.rep_->data(), vec.rep_->size_);
        }
        /// @endcond
    }

    void replace(size_t index, const vT *values, size_t size) {
        /// @cond
        if(!size)return *this;
        if(index+size > rep_->size_)
            throw vector_exception("vector_t::replace: index + size > rep_->size_"); // throw
        if(rep_->refcount_){
            rep_ = rep_->clone_for_replace(index, size);
            Traits::copy_construct(rep_->data() + index, values, size);
        }
        else
            Traits::copy(rep_->data() + index, values, size);
        /// @endcond
    }
    
    void replace(size_t index, const Self &vec) {
        /// @cond
        if(&vec == this){
            if(index)
                throw vector_exception("vector_t::replace: trying to replace self by self with offset"); // throw
            return *this;
        }
        replace(index, vec.rep_->data(), vec.rep_->size_);
        /// @endcond
    }
    
    void assign(size_t index, size_t size, const vT &c) {
        /// @cond
        if(index+size > rep_->size_)
            throw vector_exception("vector_t::assing: index + size > rep_->size_"); // throw
        if(rep_->refcount_){
            rep_ = rep_->clone_for_replace(index, size);
            Traits::construct(rep_->data() + index, size, c);
        }
        else
            Traits::assign(rep_->data() + index, size, c);
        /// @endcond
    }
    
    void erase(size_t index, size_t size = 1) {
        /// @cond
        if(!size)return;
        if(index+size > rep_->size_)
            throw vector_exception("vector_t::erase: index + size > rep_->size_"); // throw
        
        const size_t new_size = rep_->size_ - size;
        
        if(rep_->refcount_ || new_size < rep_->capacity_ / 2)
            rep_ = rep_->clone_and_erase(new_size, index, size);
        else {
            rep_->erase_data(new_size, index, size);
        }
        /// @endcond
    }
    
    vT &operator[](size_t index){
        /// @cond
        if(index < rep_->size_){
            if(rep_->refcount_)
                rep_ = rep_->clone();
            return rep_->data()[index];
        }
        resize(index+1);
        return rep_->data()[index];
        /// @endcond
    }
    
    const vT &operator[](size_t index) const {
        /// @cond
        if(index < rep_->size_)
            return rep_->data()[index];
        throw vector_exception("vector_t::operator[]: index > rep_->size_"); // need throw
        /// @endcond
    }
    
    void push_back(const vT &c){
        if(rep_->refcount_ || rep_->size_ == rep_->capacity_)
            rep_ = rep_->clone_for_append(rep_->size_ + 1);
        else {
            //Append data to vector
            ++rep_->size_;
        }
        Traits::copy_construct(rep_->data() + rep_->size_ - 1, &c, 1);
    }
    
    void pop_back() {
        if(!rep_->size_)
            throw vector_exception("vector_t::pop_back: vector is empty"); // need throw
        const size_t size = rep_->size_ - 1;
        if(rep_->refcount_ || size < rep_->capacity_ / 2 || size > rep_->capacity_){
            rep_ = rep_->clone_and_pop_back(size);
        }
        else {
            rep_->size_ = size;
            Traits::destroy(rep_->data() + size, 1);
        }
    }
    
    size_t size() const{
        /// @cond
        return rep_->size_;
        /// @endcond
    }
    
    size_t capacity() const{
        /// @cond
        return rep_->capacity_;
        /// @endcond
    }
    
    void resize(size_t size, vT c = vT()){
        /// @cond
        if(rep_->refcount_ || size < rep_->capacity_ / 2 || size > rep_->capacity_){
            rep_ = rep_->clone_and_resize(size, c);
        }
        else {
            const size_t old_size = rep_->size_;
            if(size > old_size)
                Traits::construct(rep_->data() + old_size, size - old_size, c);
            if(size < old_size)
                Traits::destroy(rep_->data() + size, old_size - size);
            rep_->size_ = size;
            rep_->data()[size] = 0;
        }
        /// @endcond
    }
    
    void clear(){
        /// @cond
        resize(0);
        /// @endcond
    }
    
    const vT *data() const {
        /// @cond
        return rep_->data();
        /// @endcond
    }
    
    vT *data() {
        /// @cond
        if(rep_->refcount_)
            rep_ = rep_->clone();
        return rep_->data();
        /// @endcond
    }
    
    void swap(Self &vec){
        /// @cond
        if(&vec == this)return;
        std::swap(vec.rep_, rep_);
        /// @endcond
    }
    
    vT* begin(){
        /// @cond
        if(rep_->refcount_)
            rep_ = rep_->clone();
        return rep_->data();
        /// @endcond
    }
    
    vT* const begin() const{
        /// @cond
        return rep_->data();
        /// @endcond
    }
    
    vT* end(){
        /// @cond
        if(rep_->refcount_)
            rep_ = rep_->clone();
        return rep_->data() + rep_->size_;
        /// @endcond
    }
    
    vT* const end() const{
        /// @cond
        return rep_->data() + rep_->size_;
        /// @endcond
    }
};

template<class vT>
class vector<vT>::rep_base vector<vT>::rep::rep_empty_ = {0, 0, 1};

} //namespace bloom
