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

#include <bloom++/exception.h>
#include <bloom++/_bits/list_t.h>
#include <bloom++/_bits/list_iterator_t.h>
#include <bloom++/exception.h>

namespace bloom
{

/**
 * List exception.
 */
class list_exception: public exception
{
public:
    list_exception(string msg):exception(msg){}
    virtual ~list_exception() throw() {}
};

/**
 * List erase exception.
 */
class bad_list_erase: public list_exception
{
public:
    bad_list_erase(string msg):list_exception(string("list::erase: ")+msg){}
    virtual ~bad_list_erase() throw() {}
};

/**
 * List back exception.
 */
class bad_list_back: public list_exception
{
public:
    bad_list_back(string msg):list_exception(string("list::back: ")+msg){}
    virtual ~bad_list_back() throw() {}
};

/**
 * List front exception.
 */
class bad_list_front: public list_exception
{
public:
    bad_list_front(string msg):list_exception(string("list::front: ")+msg){}
    virtual ~bad_list_front() throw() {}
};

/**
 * List back exception.
 */
class bad_list_pop_back: public list_exception
{
public:
    bad_list_pop_back(string msg):list_exception(string("list::pop_back: ")+msg){}
    virtual ~bad_list_pop_back() throw() {}
};

/**
 * List front exception.
 */
class bad_list_pop_front: public list_exception
{
public:
    bad_list_pop_front(string msg):list_exception(string("list::pop_front: ")+msg){}
    virtual ~bad_list_pop_front() throw() {}
};

/**
 * @brief List.
 */
template<class vT>
class list : public list_t<vT>
{
public:
    typedef list<vT>                                    Self;
    typedef list_t<vT>                                  base_list;
    typedef list_iterable_t<vT>                         iterable;
    typedef list_iterator_t<vT>                         iterator;
    typedef list_iterator_t<vT, const vT>               const_iterator;
    typedef list_reverse_iterator_t<vT>                 reverse_iterator;
    typedef list_reverse_iterator_t<vT, const vT>       const_reverse_iterator;

    void insert(const iterator &it, const vT &v)
    {
        /// @cond
        list_iterable_base *i = new iterable(v);
        base_list::include(it.element_, i);
        /// @endcond
    }

    void insert_after(const iterator &it, const vT &v)
    {
        /// @cond
        list_iterable_base *i = new iterable(v);
        base_list::include(it.element_, i);
        /// @endcond
    }

    void insert_before(const iterator &it, const vT &v)
    {
        /// @cond
        list_iterable_base *i = new iterable(v);
        base_list::include(it.element_->pPrev_, i);
        /// @endcond
    }

    iterator erase(const iterator &it) 
    {
        /// @cond
        iterator r(it.element_->pNext_);
        if(it.element_ != base_list::end_iterable())
            delete base_list::exclude(it.element_);
        else
            throw bad_list_erase("can't erase end element!");
        return r;
        /// @endcond
    }

    void push_back(const vT &v)
    {
        /// @cond
        list_iterable_base *i = new iterable(v);
        base_list::include(base_list::end_iterable_->pPrev_, i);
        /// @endcond
    }

    void push_front(const vT &v)
    {
        /// @cond
        list_iterable_base *i = new iterable(v);
        base_list::include(base_list::end_iterable_->pNext_, i);
        /// @endcond
    }

    vT & back()
    {
        /// @cond
        if(!base_list::size())
            throw bad_list_back("container is empty!");
        return static_cast<iterable*>(base_list::end_iterable()->pPrev_)->value_;
        /// @endcond
    }
    
    const vT & back() const throw()
    {
        /// @cond
        if(!base_list::size())
            throw bad_list_back("container is empty!");
        return static_cast<iterable*>(base_list::end_iterable()->pPrev_)->value_;
        /// @endcond
    }

    vT & front()
    {
        /// @cond
        if(!base_list::size())
            throw bad_list_front("container is empty!");
        return static_cast<iterable*>(base_list::end_iterable()->pNext_)->value_;
        /// @endcond
    }
    
    const vT & front() const
    {
        /// @cond
        if(!base_list::size())
            throw bad_list_front("container is empty!");
        return static_cast<iterable*>(base_list::end_iterable()->pNext_)->value_;
        /// @endcond
    }

    void pop_front()
    {
        /// @cond
        if(base_list::end_iterable()->pNext_ != base_list::end_iterable())
            delete base_list::exclude(base_list::end_iterable()->pNext_);
        else
            throw bad_list_pop_front("container is empty!");
        /// @endcond
    }

    void pop_back()
    {
        /// @cond
        if(base_list::end_iterable()->pPrev_ != base_list::end_iterable())
            delete base_list::exclude(base_list::end_iterable()->pPrev_);
        else
            throw bad_list_pop_back("container is empty!");
        /// @endcond
    }
    
    iterator begin(){
        return iterator(base_list::end_iterable()->pNext_);
    }
    
    const_iterator begin() const {
        return const_iterator(base_list::end_iterable()->pNext_);
    }
    
    reverse_iterator rbegin(){
        return reverse_iterator(base_list::end_iterable()->pPrev_);
    }
    
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(base_list::end_iterable()->pPrev_);
    }
    
    iterator end(){
        return iterator(base_list::end_iterable());
    }
    
    const_iterator end() const {
        return const_iterator(base_list::end_iterable());
    }
    
    reverse_iterator rend(){
        return reverse_iterator(base_list::end_iterable());
    }
    
    const_reverse_iterator rend() const {
        return const_reverse_iterator(base_list::end_iterable());
    }
    
    inline void transmit_front(base_list &l) FORCE_INLINE{
        base_list::transmit(base_list::end_iterable()->pNext_, l);
    }
    
    inline void transmit_back(base_list &l) FORCE_INLINE{
        base_list::transmit(base_list::end_iterable()->pPrev_, l);
    }
    
    inline size_t size() const FORCE_INLINE{
        return base_list::size();
    }
    
    inline void swap(Self &c) FORCE_INLINE{
        base_list::swap(c);
    }
    
    inline void clear() FORCE_INLINE{
        base_list::clear();
    }
};

} //namespace bloom
