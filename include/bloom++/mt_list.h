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

#include <bloom++/_bits/mt_list_iterator_t.h>
#include <bloom++/_bits/mt_store.h>
#include <bloom++/_bits/list_t.h>

namespace bloom {

/**
 * @brief Multi-thread safe list.
 */
template<class vT>
class mt_list : public list_t<vT>, public mt_store
{
public:
    typedef mt_list<vT>                                 Self;
    typedef list_t<vT>                                  base_list;
    typedef list_iterable_t<vT>                         iterable;
    typedef mt_list_iterator_t<vT>                      iterator;
    typedef mt_list_iterator_t<vT, const vT>            const_iterator;
    typedef mt_list_reverse_iterator_t<vT>              reverse_iterator;
    typedef mt_list_reverse_iterator_t<vT, const vT>    const_reverse_iterator;
    
    mt_list(){}

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
        iterator r(this, it.element_->pNext_);
        if(it.element_ != base_list::end_iterable())
            delete base_list::exclude(it.element_);
        return r;
        /// @endcond
    }

    void push_back(const vT &v)
    {
        /// @cond
        mutex::scoped_unilock sl(m_);
        list_iterable_base *i = new iterable(v);
        base_list::include(base_list::end_iterable_->pPrev_, i);
        /// @endcond
    }

    void push_front(const vT &v)
    {
        /// @cond
        mutex::scoped_unilock sl(m_);
        list_iterable_base *i = new iterable(v);
        base_list::include(base_list::end_iterable_->pNext_, i);
        /// @endcond
    }

    void pop_front()
    {
        /// @cond
        mutex::scoped_unilock sl(m_);
        if(base_list::end_iterable()->pNext_ != base_list::end_iterable())
            delete base_list::exclude(base_list::end_iterable()->pNext_);
        /// @endcond
    }

    void pop_back()
    {
        /// @cond
        mutex::scoped_unilock sl(m_);
        if(base_list::end_iterable()->pPrev_ != base_list::end_iterable())
            delete base_list::exclude(base_list::end_iterable()->pPrev_);
        /// @endcond
    }
    
    iterator begin(){
        m_.lock();
        return iterator(this, base_list::end_iterable()->pNext_);
    }
    
    const_iterator begin() const {
        m_.lock();
        return const_iterator(this, base_list::end_iterable()->pNext_);
    }
    
    reverse_iterator rbegin(){
        m_.lock();
        return reverse_iterator(this, base_list::end_iterable()->pPrev_);
    }
    
    const_reverse_iterator rbegin() const {
        m_.lock();
        return const_reverse_iterator(this, base_list::end_iterable()->pPrev_);
    }
    
    iterator end(){
        m_.lock();
        return iterator(this, base_list::end_iterable());
    }
    
    const_iterator end() const {
        m_.lock();
        return const_iterator(this, base_list::end_iterable());
    }
    
    reverse_iterator rend(){
        m_.lock();
        return reverse_iterator(this, base_list::end_iterable());
    }
    
    const_reverse_iterator rend() const {
        m_.lock();
        return const_reverse_iterator(this, base_list::end_iterable());
    }
    
    inline void transmit_front(base_list &l) FORCE_INLINE{
        mutex::scoped_unilock sl(m_);
        base_list::transmit(base_list::end_iterable()->pNext_, l);
    }
    
    inline void transmit_back(base_list &l) FORCE_INLINE{
        mutex::scoped_unilock sl(m_);
        base_list::transmit(base_list::end_iterable()->pPrev_, l);
    }
    
    inline size_t size() const FORCE_INLINE{
        mutex::scoped_unilock sl(m_);
        return base_list::size();
    }
    
    inline void swap(Self &c) FORCE_INLINE{
        mutex::scoped_unilock sl(m_);
        base_list::swap(c);
    }
    
    inline void clear() FORCE_INLINE{
        mutex::scoped_unilock sl(m_);
        base_list::clear();
    }
};

} //namespace bloom