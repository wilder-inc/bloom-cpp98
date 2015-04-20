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
#include <bloom++/_bits/char_traits.h>
#include <exception>

#ifdef AUX_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

/**
 * String exception.
 */
class string_exception: public std::exception
{
private:
    const char *msg_;
public:
    string_exception(const char *msg):msg_(msg){}
    virtual ~string_exception() throw() {}
    virtual const char *what() throw(){
        return msg_;
    }
};

/**
 * @brief The string_t class.
 */
template<class vT>
class string_t
{
public:
    typedef class string_t<vT>          Self;
    typedef class char_traits<vT>       Traits;
    
private: 
    /// @cond
    
    /*
     * String representation.
     */
    struct rep_base {
        size_t size_;
        size_t capacity_;
        int refcount_; //Atomic word
        vT data_[1];
    };
        
    struct rep: rep_base
    {
        static rep_base rep_empty_;
        
        inline static rep* create(size_t size, size_t old_capacity) FORCE_INLINE {
            if(!size)return static_cast<rep*>(&rep_empty_);
            
            const size_t pagesize = 4096;
            const size_t malloc_header_size = 4 * sizeof(void*);
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
            r->data_[size] = 0;
            return r;
        }
        
        inline static rep* create(const vT *str) FORCE_INLINE {
            rep *r = rep::create(Traits::length(str), 0);
            Traits::copy(r->data_, str, r->size_);
            return r;
        }
        
        inline static rep* create(vT str) FORCE_INLINE {
            rep *r = rep::create(1, 0);
            Traits::copy(r->data_, &str, r->size_);
            return r;
        }
        
        inline rep* reallocate(size_t capacity) FORCE_INLINE {
            const size_t pagesize = 4096;
            const size_t malloc_header_size = 4 * sizeof(void*);
            
            if ( capacity > this->capacity_ && capacity < 2 * this->capacity_)
                capacity = 2 * this->capacity_;

            size_t alloc_size = capacity * sizeof(vT) + sizeof(rep);

            const size_t adj_size = alloc_size + malloc_header_size;
            if (adj_size > pagesize) {
                const size_t extra = pagesize - adj_size % pagesize;
                capacity += extra / sizeof(vT);
                alloc_size = capacity * sizeof(vT) + sizeof(rep);
            }
            
            rep *r = (rep *)realloc(this, alloc_size);
            r->capacity_ = capacity;
            return r;
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
                    free(this);
                }
            }
        }
        
        inline rep *clone() FORCE_INLINE {
            rep *r = (rep *)malloc(this->capacity_ * sizeof(vT) + sizeof(rep));
            memcpy(reinterpret_cast<void*>(r), reinterpret_cast<void*>(this), 
                   (this->size_ + 1) * sizeof(vT) + sizeof(rep));
            this->releaseRef();
            return r;
        }
        
        inline rep *clone_for_insert(size_t new_size, size_t insert_index, size_t insert_size) FORCE_INLINE {
            rep *r = create(new_size, this->capacity_);
            
            if(insert_index >= this->size_){
                Traits::copy(r->data_, this->data_, this->size_);
            }
            else {
                if(insert_index > 0)
                    Traits::copy(r->data_, this->data_, insert_index);
                Traits::copy(r->data_ + insert_index + insert_size, 
                             this->data_ + insert_index, 
                             this->size_ - insert_index);
            }
            this->releaseRef();
            return r;
        }
        
        inline rep *clone_for_resize(size_t new_size) FORCE_INLINE {
            rep *r = create(new_size, this->capacity_);

            Traits::copy(r->data_, this->data_, new_size < this->size_ ? new_size : this->size_);
            
            this->releaseRef();
            return r;
        }
        
        inline rep *clone_and_resize(size_t new_size, vT c) FORCE_INLINE {
            rep *r = create(new_size, this->capacity_);

            Traits::copy(r->data_, this->data_, new_size < this->size_ ? new_size : this->size_);
            
            if(new_size > this->size_)
                Traits::assign(r->data_ + this->size_, new_size - this->size_, c);
            
            this->releaseRef();
            return r;
        }
        
        inline rep *clone_for_replace(size_t replace_index, size_t replace_size) FORCE_INLINE {
            rep *r = create(this->size_, this->capacity_);
            
            if(replace_index > 0)
                Traits::copy(r->data_, this->data_, replace_index);
            if(replace_index < rep_->size_)
                Traits::copy(r->data_ + replace_index + replace_size, 
                             this->data_ + replace_index + replace_size, 
                             this->size_ - replace_index - replace_size);
            this->releaseRef();
            return r;
        }
        
        inline rep* clone_and_erase(const size_t new_size, const size_t erase_index, const size_t erase_size) FORCE_INLINE {
            const size_t next = erase_index + erase_size;
            rep *r = create(new_size, this->capacity_);
            if(erase_index > 0)
                Traits::copy(r->data_, this->data_, erase_index);
            if(next < this->size_)
                Traits::copy(r->data_ + erase_index, this->data_ + next, this->size_ - next);
            this->releaseRef();
            return r;
        }

        inline void erase_data(const size_t new_size, const size_t erase_index, const size_t erase_size) FORCE_INLINE {
            const size_t next = erase_index + erase_size;
            if(next < this->size_)
                Traits::copy(this->data_ + erase_index, this->data_ + next,
                             this->size_ - next);
            
            this->size_ = new_size;
            this->data_[new_size] = 0;
        }
        
    } *rep_;
    
    inline void append_prepare(size_t size) FORCE_INLINE {
        size_t new_size = rep_->size_ + size;
        if(rep_->refcount_)
            rep_ = rep_->clone_for_resize(new_size);
        else {
            //Append data to string
            if(new_size > rep_->capacity_)
                rep_ = rep_->reallocate(new_size);
            rep_->size_ = new_size;
            rep_->data_[new_size] = 0;
        }
    }
    
    inline void insert_prepare(size_t index, size_t size) FORCE_INLINE {
        size_t new_size = rep_->size_+size;
        if(rep_->refcount_)
            rep_ = rep_->clone_for_insert(new_size, index, size);
        else if(index < rep_->size_){
            if(new_size > rep_->capacity_)
                rep_ = rep_->clone_for_insert(new_size, index, size);
            else {
                //Insert data to string
                Traits::move(rep_->data_ + index + size, 
                        rep_->data_ + index, rep_->size_ - index);
                rep_->size_ = new_size;
                rep_->data_[new_size] = 0;
            }
        }
        else {
            //Append data to string
            if(new_size > rep_->capacity_)
                rep_ = rep_->reallocate(new_size);
            rep_->size_ = new_size;
            rep_->data_[new_size] = 0;
        }
    }

    /// @endcond
    
public:
    typedef vT *                                iterator;
    typedef const vT *                          const_iterator;
    
    string_t():
    rep_(rep::create(0, 0)){
    }
    
    string_t(const Self &str):
    rep_(str.rep_->getRef()){
    }
    
    string_t(const vT *str):
    rep_(rep::create(str)){
    }
    
    string_t(vT ch):
    rep_(rep::create(ch)){
    }
    
    ~string_t(){
        /// @cond
        rep_->releaseRef();
        /// @endcond
    }
    
    Self &operator=(const Self &str){
        /// @cond
        rep_->releaseRef();
        rep_ = str.rep_->getRef();
        return *this;
        /// @endcond
    }
    
    Self &operator=(const vT *str){
        /// @cond
        rep_->releaseRef();
        rep_ = rep::create(str);
        return *this;
        /// @endcond
    }
    
    Self &operator=(vT ch){
        /// @cond
        rep_->releaseRef();
        rep_ = rep::create(ch);
        return *this;
        /// @endcond
    }
    
    Self& append(vT c){
        return append(&c, 1);
    }
    
    Self& append(const vT *cstr){
        return append(&cstr, Traits::length(cstr));
    }
    
    Self& append(const vT *str, size_t size) {
        /// @cond
        if(!size)return *this;
        const size_t old_size = rep_->size_;
        append_prepare(size);
        Traits::copy(rep_->data_ + old_size, str, size);
        return *this;
        /// @endcond
    }
    
    Self& append(const Self &str){
        if(!str.rep_->size_)return *this;
        const size_t old_size = rep_->size_;
        const size_t size = str.rep_->size_;
        append_prepare(str.rep_->size_);
        Traits::copy(rep_->data_ + old_size, str.rep_->data_, size);
        return *this;
    }
    
    Self& insert(size_t index, vT c){
        return insert(index, &c, 1);
    }
    
    Self& insert(size_t index, const vT *cstr){
        /// @cond
        return insert(index, cstr, Traits::length(cstr));
        /// @endcond
    }
    
    Self& insert(size_t index, const vT *values, size_t size) throw(){
        /// @cond
        if(!size)return *this;
        if(index > rep_->size_)
            throw string_exception("bloom::string::insert: index out of range");
        insert_prepare(index, size);
        Traits::copy(rep_->data_ + index, values, size);
        return *this;
        /// @endcond
    }
    
    Self& insert(size_t index, const Self &str) throw(){
        if(!str.rep_->size_)return *this;
        if(index > rep_->size_)
            throw string_exception("bloom::string::insert: index out of range");
        if(&str == this){
            const size_t size = str.rep_->size_;
            insert_prepare(index, str.rep_->size_);
            Traits::copy(rep_->data_ + index, rep_->data_, index);
            Traits::copy(rep_->data_ + index * 2, rep_->data_ + index + size, size - index);
        }
        else {
            insert_prepare(index, str.rep_->size_);
            Traits::copy(rep_->data_ + index, str.rep_->data_, str.rep_->size_);
        }
        return *this;
    }

    Self& replace(size_t index, const vT *values, size_t size) throw() {
        /// @cond
        if(!size)return *this;
        if(index+size > rep_->size_)
            throw string_exception("bloom::string::replace: out of range");
        if(rep_->refcount_)
            rep_ = rep_->clone_for_replace(index, size);
        Traits::copy(rep_->data_ + index, values, size);
        return *this;
        /// @endcond
    }
    
    Self& replace(size_t index, const Self &str) throw(){
        if(&str == this){
            if(index)
                throw string_exception("bloom::string::replace: trying to replace self by self with offset");
            return *this;
        }
        return replace(index, str.rep_->data_, str.rep_->size_);
    }
    
    Self& replace(size_t index, size_t size, vT c) throw(){
        /// @cond
        if(index+size > rep_->size_)
            throw string_exception("bloom::string::replace: out of range");
        if(rep_->refcount_)
            rep_ = rep_->clone_for_replace(index, size);
        Traits::assign(rep_->data_ + index, size, c);
        return *this;
        /// @endcond
    }
    
    Self& erase(size_t index, size_t size = 1) throw(){
        /// @cond
        if(!size)return *this;
        if(index+size > rep_->size_)
            throw string_exception("bloom::string::erase: out of range");
        
        const size_t new_size = rep_->size_ - size;
        
        if(rep_->refcount_ || new_size < rep_->capacity_ / 2)
            rep_ = rep_->clone_and_erase(new_size, index, size);
        else {
            rep_->erase_data(new_size, index, size);
        }
        return *this;
        /// @endcond
    }
    
    vT &operator[](size_t index){
        /// @cond
        if(index < rep_->size_){
            if(rep_->refcount_)
                rep_ = rep_->clone();
            return rep_->data_[index];
        }
        resize(index+1);
        return rep_->data_[index];
        /// @endcond
    }
    
    const vT &operator[](size_t index) const {
        /// @cond
        if(index < rep_->size_)
            return rep_->data_[index];
        throw string_exception("bloom::string::operator[] const: out of range");
        /// @endcond
    }
    
    Self &operator+=(const Self &str){
        return append(str);
    }
    
    Self operator+(const Self &str){
        /// @cond
        Self v(*this);
        v.append(str);
        return v;
        /// @endcond
    }
    
    Self &operator+=(vT c){
        return append(&c, 1);
    }
    
    Self operator+(vT elem){
        /// @cond
        Self v(*this);
        v.append(&elem, 1);
        return v;
        /// @endcond
    }
    
    Self &operator+=(const vT *str){
        return append(str, Traits::length(str));
    }
    
    Self operator+(const vT *str){
        /// @cond
        Self v(*this);
        v.append(str, Traits::length(str));
        return v;
        /// @endcond
    }
    
    size_t size() const{
        /// @cond
        return rep_->size_;
        /// @endcond
    }
    
    void resize(size_t size, vT c = vT()){
        /// @cond
        if(rep_->refcount_ || size < rep_->capacity_ / 2){
            rep_ = rep_->clone_and_resize(size, c);
        }
        else {
            const size_t old_size = rep_->size_;
            if(size > rep_->capacity_)
                rep_ = rep_->reallocate(size);
            if(size > old_size)
                Traits::assign(rep_->data_ + old_size, size - old_size, c);
            rep_->size_ = size;
            rep_->data_[size] = 0;
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
        return rep_->data_;
        /// @endcond
    }
    
    vT *data() {
        /// @cond
        if(rep_->refcount_)
            rep_ = rep_->clone();
        return rep_->data_;
        /// @endcond
    }
    
    const vT *c_str() const {
        /// @cond
        return rep_->data_;
        /// @endcond
    }
    
    size_t length() const {
        /// @cond
        return rep_->size_;
        /// @endcond
    }
    
    bool operator==(const Self &str) const{
        /// @cond
        if(&str == this)return true;
        if(rep_->size_ != str.rep_->size_)return false;
        return Traits::compare(rep_->data_, str.rep_->data_, rep_->size_) == 0;
        /// @endcond
    }
    
    bool operator==(const vT *str) const{
        /// @cond
        size_t sz = Traits::length(str);
        if(rep_->size_ != sz)return false;
        return Traits::compare(rep_->data_, str, sz) == 0;
        /// @endcond
    }
    
    bool operator!=(const Self &str) const{
        /// @cond
        if(&str == this)return false;
        if(rep_->size_ != str.rep_->size_)return false;
        return Traits::compare(rep_->data_, str.rep_->data_, rep_->size_) != 0;
        /// @endcond
    }
    
    bool operator!=(const vT *str) const{
        /// @cond
        size_t sz = Traits::length(str);
        if(rep_->size_ != sz)return false;
        return Traits::compare(rep_->data_, str, sz) != 0;
        /// @endcond
    }
    
    void swap(Self &str){
        /// @cond
        if(&str == this)return;
        std::swap(str.rep_, rep_);
        /// @endcond
    }
    
    iterator find(size_t index){
        /// @cond
        if(rep_->refcount_)
            rep_ = rep_->clone();
        return iterator(rep_->data_ + index);
        /// @endcond
    }
    
    const_iterator find(size_t index) const{
        /// @cond
        return const_iterator(rep_->data_ + index);
        /// @endcond
    }
    
    iterator begin(){
        /// @cond
        if(rep_->refcount_)
            rep_ = rep_->clone();
        return iterator(rep_->data_);
        /// @endcond
    }
    
    const_iterator begin() const{
        /// @cond
        return const_iterator(rep_->data_);
        /// @endcond
    }
    
    iterator end(){
        /// @cond
        if(rep_->refcount_)
            rep_ = rep_->clone();
        return iterator(rep_->data_ + rep_->size_);
        /// @endcond
    }
    
    const_iterator end() const{
        /// @cond
        return const_iterator(rep_->data_ + rep_->size_);
        /// @endcond
    }
};

} //namespace bloom
