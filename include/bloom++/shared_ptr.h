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

#ifdef BLOOM_SHARED_PTR_MT
#include <bloom++/mutex.h>
#endif

#ifdef AUX_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

struct shared_counter__
{
    mutable unsigned int count_;
    shared_counter__(): count_(1){}
};

/**
 * @brief Like std::shared_ptr (C++11) or boost::shared_ptr.
 * Not thread safe!!!
 */
template<class T>
class shared_ptr
{
private:
    shared_counter__ *counter_;
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
    
    inline shared_counter__ *get_inc_counter() const {
        if(counter_)
            ++counter_->count_;
        return counter_;
    }
    
    template<class Tp1>
    friend class shared_ptr;

public:
    typedef T element_type;

    shared_ptr() : counter_(0), pointer_(0)
    {}

    explicit shared_ptr(T *ptr) : 
        counter_(new shared_counter__()),
        pointer_(ptr)
    {}
    
    shared_ptr(const shared_ptr &p) :
        counter_(p.get_inc_counter()),
        pointer_(p.pointer_)
    {}
    
    template<class Tp1>
    explicit shared_ptr(const shared_ptr<Tp1> &p, T *ptr) :
        counter_(p.get_inc_counter()),
        pointer_(ptr)
    {}

    ~shared_ptr() {
        dec_counter();
    }

    shared_ptr& operator=(const shared_ptr &p) {
        //DEBUG_INFO("operator = \n");
        shared_ptr(p).swap(*this);
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
        shared_ptr().swap(*this);
    }
    
    template<class Tp1>
    void reset(Tp1 *p) {
        shared_ptr(p).swap(*this);
    }
    
    void swap(shared_ptr &p) {
        std::swap(counter_, p.counter_);
        std::swap(pointer_, p.pointer_);
    }
};

template<class T>
class shared_ptr<T[]>
{
private:
    shared_counter__ *counter_;
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
    
    inline shared_counter__ *get_inc_counter() const {
        if(counter_)
            ++counter_->count_;
        return counter_;
    }
    
    template<class Tp1>
    friend class shared_ptr;

public:
    typedef T element_type;

    shared_ptr() : counter_(0), pointer_(0)
    {}

    explicit shared_ptr(T *ptr) : 
        counter_(new shared_counter__()),
        pointer_(ptr)
    {}
    
    shared_ptr(const shared_ptr &p) :
        counter_(p.get_inc_counter()),
        pointer_(p.pointer_)
    {}
    
    template<class Tp1>
    explicit shared_ptr(const shared_ptr<Tp1> &p, T *ptr) :
        counter_(p.get_inc_counter()),
        pointer_(ptr)
    {}

    ~shared_ptr() {
        dec_counter();
    }

    shared_ptr& operator=(const shared_ptr &p) {
        //DEBUG_INFO("operator = \n");
        shared_ptr(p).swap(*this);
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
        shared_ptr().swap(*this);
    }
    
    template<class Tp1>
    void reset(Tp1 *p) {
        shared_ptr(p).swap(*this);
    }
    
    void swap(shared_ptr &p) {
        std::swap(counter_, p.counter_);
        std::swap(pointer_, p.pointer_);
    }
};

/*  //this is C++11 code
template<class T, class... Args>
inline shared_ptr<T> make_shared(Args&&... args)
{
  return shared_ptr<T>(new T(std::forward<Args>(args)...));
}
 */

/*
 * Comparations
 */

template<class Tp, class Tp1>
bool operator==(const shared_ptr<Tp> &p, const shared_ptr<Tp1> &p1)
{
    return p.get() == p1.get();
}

template<class Tp, class Tp1>
bool operator!=(const shared_ptr<Tp> &p, const shared_ptr<Tp1> &p1)
{
    return p.get() != p1.get();
}

/*
 *  make_shared
 */

template<class T>
inline shared_ptr<T> make_shared()
{
    return shared_ptr<T>(new T());
}

template<class T, class P1>
inline shared_ptr<T> make_shared(const P1 &p1)
{
    return shared_ptr<T>(new T(p1));
}

template<class T, class P1, class P2>
inline shared_ptr<T> make_shared(const P1 &p1, const P2 &p2)
{
    return shared_ptr<T>(new T(p1, p2));
}

template<class T, class P1, class P2, class P3>
inline shared_ptr<T> make_shared(const P1 &p1, const P2 &p2, const P3 &p3)
{
    return shared_ptr<T>(new T(p1, p2, p3));
}

template<class T, class P1, class P2, class P3, class P4>
inline shared_ptr<T> make_shared(const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4)
{
    return shared_ptr<T>(new T(p1, p2, p3, p4));
}

template<class T, class P1, class P2, class P3, class P4, class P5>
inline shared_ptr<T> make_shared(const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5)
{
    return shared_ptr<T>(new T(p1, p2, p3, p4, p5));
}

template<class T, class P1, class P2, class P3, class P4, class P5, class P6>
inline shared_ptr<T> make_shared(const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5, const P6 &p6)
{
    return shared_ptr<T>(new T(p1, p2, p3, p4, p5, p6));
}



/*
 * Casts
 */

template<class Tp, class Tp1>
inline shared_ptr<Tp> static_pointer_cast(const shared_ptr<Tp1> &p)
{
    return shared_ptr<Tp>(p, static_cast<Tp*>(p.get()));
}

template<class Tp, class Tp1>
inline shared_ptr<Tp> const_pointer_cast(const shared_ptr<Tp1> &p)
{
    return shared_ptr<Tp>(p, const_cast<Tp*>(p.get()));
}

template<class Tp, class Tp1>
inline shared_ptr<Tp> dynamic_pointer_cast(const shared_ptr<Tp1> &p)
{
    return shared_ptr<Tp>(p, dynamic_cast<Tp*>(p.get()));
}

template<class Tp, class Tp1>
inline shared_ptr<Tp> reinterpret_pointer_cast(const shared_ptr<Tp1> &p)
{
    return shared_ptr<Tp>(p, reinterpret_cast<Tp*>(p.get()));
}

} //namespace bloom

