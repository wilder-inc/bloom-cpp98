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

#ifdef AUX_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

/**
 * @brief Like std::unique_ptr (C++11) or boost::unique_ptr.
 */
template<class T>
class unique_ptr
{
private:
    mutable T *pointer_;
    
public:
    typedef T element_type;
    
    unique_ptr():pointer_(0){}
    
    explicit unique_ptr(T *ptr):pointer_(ptr){}
    
    template<class Tp1>
    explicit unique_ptr(const unique_ptr<Tp1> &p, T *ptr):pointer_(p.release(ptr)){}
    
    unique_ptr(const unique_ptr &ptr):pointer_(ptr.release()){}
    
    ~unique_ptr(){
        reset();
    }

    unique_ptr &operator=(const unique_ptr &ptr){
        reset(ptr.release());
        return *this;
    }
        
    T & operator*() const {
        return *pointer_;
    }
    
    T * operator->() const {
        return pointer_;
    }

    T * get() const {
        return pointer_;
    }
            
    T *release() const {
        T *ret = pointer_;
        pointer_ = NULL;
        return ret;
    }
    
    template<class Tp1>
    Tp1 *release(Tp1 *ptr) const {
        pointer_ = NULL;
        return ptr;
    }
    
    void reset(T *p = NULL){
        if(pointer_)
            delete pointer_;
        pointer_ = p;
    }
    
    void swap(unique_ptr &p) {
        std::swap(pointer_, p.pointer_);
    }
};

template<class T>
class unique_ptr<T[]>
{
private:
    mutable T *pointer_;
    
public:
    typedef T element_type;
    
    unique_ptr():pointer_(0){}
    
    explicit unique_ptr(T *ptr):pointer_(ptr){}
    
    template<class Tp1>
    explicit unique_ptr(const unique_ptr<Tp1> &p, T *ptr):pointer_(p.release(ptr)){}
    
    unique_ptr(const unique_ptr &ptr):pointer_(ptr.release()){}
    
    ~unique_ptr(){
        reset();
    }
    
    unique_ptr &operator=(const unique_ptr &ptr){
        reset(ptr.release());
        return *this;
    }
    
    T & operator*() const {
        return *pointer_;
    }
    
    T * operator->() const {
        return pointer_;
    }

    T * get() const {
        return pointer_;
    }
    
    T & operator[](size_t index) const {
        return pointer_[index];
    }
            
    T *release() {
        T *ret = pointer_;
        pointer_ = NULL;
        return ret;
    }
    
    template<class Tp1>
    Tp1 *release(Tp1 *ptr) const {
        pointer_ = NULL;
        return ptr;
    }
    
    void reset(T *p = NULL){
        if(pointer_)
            delete [] pointer_;
        pointer_ = p;
    }
    
    void swap(unique_ptr &p) {
        std::swap(pointer_, p.pointer_);
    }
};

/*
 * Comparations
 */

template<class Tp, class Tp1>
bool operator==(const unique_ptr<Tp> &p, const unique_ptr<Tp1> &p1)
{
    return p.get() == p1.get();
}

template<class Tp, class Tp1>
bool operator!=(const unique_ptr<Tp> &p, const unique_ptr<Tp1> &p1)
{
    return p.get() != p1.get();
}

/*
 * Casts
 */

template<class Tp, class Tp1>
inline unique_ptr<Tp> static_pointer_cast(const unique_ptr<Tp1> &p)
{
    return unique_ptr<Tp>(p, static_cast<Tp*>(p.get()));
}

template<class Tp, class Tp1>
inline unique_ptr<Tp> const_pointer_cast(const unique_ptr<Tp1> &p)
{
    return unique_ptr<Tp>(p, const_cast<Tp*>(p.get()));
}

template<class Tp, class Tp1>
inline unique_ptr<Tp> dynamic_pointer_cast(const unique_ptr<Tp1> &p)
{
    return unique_ptr<Tp>(p, dynamic_cast<Tp*>(p.get()));
}

template<class Tp, class Tp1>
inline unique_ptr<Tp> reinterpret_pointer_cast(const unique_ptr<Tp1> &p)
{
    return unique_ptr<Tp>(p, reinterpret_cast<Tp*>(p.get()));
}

} //namespace bloom
