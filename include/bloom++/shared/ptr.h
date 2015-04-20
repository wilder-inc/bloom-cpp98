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

#include <bloom++/shared/shared_info.h>

#ifdef SHARED_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

namespace shared
{

class storable;
/**
 * @brief Like std::ptr (C++11) or boost::ptr.
 * Not thread safe!!!
 */
template<class T>
class ptr
{
private:
    shared_info *counter_;
    T *pointer_;
    
    inline void destroy() {
        delete counter_;
        delete pointer_;
    }

    inline void dec_counter() {
        if (counter_)
	    if(!--counter_->count_)
                destroy();
    }
    
    inline shared_info *get_inc_counter() const {
        if(counter_)
            ++counter_->count_;
        return counter_;
    }
    
    template<class Tp1>
    friend class ptr;

public:
    typedef T element_type;

    ptr() : counter_(0), pointer_(0)
    {}

    explicit ptr(T *ptr) : 
        counter_(new shared_info()),
        pointer_(ptr)
    {}
    
    ptr(const ptr &p) :
        counter_(p.get_inc_counter()),
        pointer_(p.pointer_)
    {}
    
    template<class Tp1>
    explicit ptr(const ptr<Tp1> &p, T *ptr) :
        counter_(p.get_inc_counter()),
        pointer_(ptr)
    {}

    ~ptr() {
        dec_counter();
    }

    ptr& operator=(const ptr &p) {
        //DEBUG_INFO("operator = \n");
        ptr(p).swap(*this);
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

    void reset() {
        ptr().swap(*this);
    }
    
    template<class Tp1>
    void reset(Tp1 *p) {
        ptr(p).swap(*this);
    }
    
    void remove_from_all() {
        counter_->remove_from_all_containers();
    }
    
    void swap(ptr &p) {
        std::swap(counter_, p.counter_);
        std::swap(pointer_, p.pointer_);
    }
    
    friend class storable;
};

template<class T>
class ptr<T[]>
{
private:
    shared_info *counter_;
    T *pointer_;
    
    inline void destroy() {
        delete counter_;
        delete [] pointer_;
    }

    inline void dec_counter() {
        if (counter_)
	    if(!--counter_->count_)
                destroy();
    }
    
    inline shared_info *get_inc_counter() const {
        if(counter_)
            ++counter_->count_;
        return counter_;
    }
    
    template<class Tp1>
    friend class ptr;

public:
    typedef T element_type;

    ptr() : counter_(0), pointer_(0)
    {}

    explicit ptr(T *ptr) : 
        counter_(new shared_info()),
        pointer_(ptr)
    {}
    
    ptr(const ptr &p) :
        counter_(p.get_inc_counter()),
        pointer_(p.pointer_)
    {}
    
    template<class Tp1>
    explicit ptr(const ptr<Tp1> &p, T *ptr) :
        counter_(p.get_inc_counter()),
        pointer_(ptr)
    {}

    ~ptr() {
        dec_counter();
    }

    ptr& operator=(const ptr &p) {
        //DEBUG_INFO("operator = \n");
        ptr(p).swap(*this);
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

    void reset() {
        ptr().swap(*this);
    }
    
    template<class Tp1>
    void reset(Tp1 *p) {
        ptr(p).swap(*this);
    }
    
    void remove_from_all() {
        counter_->remove_from_all_containers();
    }
    
    void swap(ptr &p) {
        std::swap(counter_, p.counter_);
        std::swap(pointer_, p.pointer_);
    }
    
    friend class storable;
};

/*
 * Comparations
 */

template<class Tp, class Tp1>
bool operator==(const ptr<Tp> &p, const ptr<Tp1> &p1)
{
    return p.get() == p1.get();
}

template<class Tp, class Tp1>
bool operator!=(const ptr<Tp> &p, const ptr<Tp1> &p1)
{
    return p.get() != p1.get();
}

} //namespace shared

} //namespace bloom

