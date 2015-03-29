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
#include <bloom++/_bits/list_iterable_t.h>
#include <bloom++/_bits/mt_store.h>
#include <bloom++/mutex.h>

#ifdef AUX_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

#include "mt_store.h"

//extern int i_count;

namespace bloom
{

/**
 * @brief The mt_iterator_t class
 * 
 * Using mt_list_iterator_t< type > for interator
 * and mt_list_iterator_t< type, const type > for const_interator.
 */
template<class vT, class rvT=vT>
struct mt_list_iterator_t
{
    typedef rvT                             value_type;
    typedef rvT &                           reference;
    typedef rvT *                           pointer;
    typedef mt_list_iterator_t<vT, rvT>     Self;
    typedef list_iterable_t<vT>             iterable;
    
    /// @cond
    const mt_store *store_;
    list_iterable_base *element_;
    /// @endcond

    mt_list_iterator_t(): store_(NULL), element_() {}
    
    explicit mt_list_iterator_t(const mt_store *store, list_iterable_base *i) : 
    store_(store), element_(i)
    {store_->incIterator();}
    
    mt_list_iterator_t(const mt_list_iterator_t &it): 
    store_(it.store_), element_(it.element_)
    {if(store_)store_->incIterator();}
    
    ~mt_list_iterator_t(){
        if(store_)
            store_->decIterator();
    }
    
    Self& operator=(const Self &it)
    {
        if(it.store_ && !store_){
            it.store_->incIterator();
        }
        else if(!it.store_ && store_){
            store_->decIterator();
        }
        store_ = it.store_;
        element_ = it.element_;
        return *this;
    }

    bool operator==(const Self &src) const
    {
        /// @cond
        return element_ == src.element_;
        /// @endcond
    }

    bool operator!=(const Self &src) const
    {
        /// @cond
        return element_ != src.element_;
        /// @endcond
    }
    
    pointer operator->()
    {
        /// @cond
        return &static_cast<iterable*>(element_)->value_;
        /// @endcond
    }

    reference operator*()
    {
        /// @cond
        return static_cast<iterable*>(element_)->value_;
        /// @endcond
    }

    Self& operator++()
    {
        /// @cond
        element_ = element_->pNext_;
        return *this;
        /// @endcond
    }

    Self operator++(int)
    {
        /// @cond
        Self r(*this);
        element_ = element_->pNext_;
        return r;
        /// @endcond
    }

    Self& operator--()
    {
        /// @cond
        element_ = element_->pPrev_;
        return *this;
        /// @endcond
    }

    Self operator--(int)
    {
        /// @cond
        Self r(*this);
        element_ = element_->pPrev_;
        return r;
        /// @endcond
    }
};

template<class vT>
inline bool operator==(const mt_list_iterator_t<vT> &it1, 
        const mt_list_iterator_t<vT, const vT> &it2)
{
    return it1.element_ == it2.element_;
}

template<class vT>
inline bool operator!=(const mt_list_iterator_t<vT> &it1, 
        const mt_list_iterator_t<vT, const vT> &it2)
{
    return it1.element_ != it2.element_;
}

/**
 * @brief The mt_list_reverse_iterator_t class
 * 
 * Using mt_list_reverse_iterator_t< type > for reverse_interator
 * and mt_list_reverse_iterator_t< type, const type > for const_reverse_interator.
 */
template<class vT, class rvT=vT>
struct mt_list_reverse_iterator_t
{
    typedef rvT                                 value_type;
    typedef rvT &                               reference;
    typedef rvT *                               pointer;
    typedef mt_list_reverse_iterator_t<vT, rvT> Self;
    typedef list_iterable_t<vT>                 iterable;
    
    /// @cond
    mt_store *store_;
    list_iterable_base *element_;
    /// @endcond

    mt_list_reverse_iterator_t(): store_(NULL), element_() {}
    
    explicit mt_list_reverse_iterator_t(mt_store *store, list_iterable_base *i) : 
    store_(store), element_(i)
    {store_->incIterator();}
    
    mt_list_reverse_iterator_t(const mt_list_reverse_iterator_t &it): 
    store_(it.store_), element_(it.element_)
    {if(store_)store_->incIterator();}
    
    ~mt_list_reverse_iterator_t(){
        if(store_)
            store_->decIterator();
    }
    
    Self& operator=(const Self &it)
    {
        if(it.store_ && !store_){
            it.store_->incIterator();
        }
        else if(!it.store_ && store_){
            store_->decIterator();
        }
        store_ = it.store_;
        element_ = it.element_;
        return *this;
    }

    bool operator==(const Self &src) const
    {
        /// @cond
        return element_ == src.element_;
        /// @endcond
    }

    bool operator!=(const Self &src) const
    {
        /// @cond
        return element_ != src.element_;
        /// @endcond
    }
    
    pointer operator->()
    {
        /// @cond
        return &static_cast<iterable*>(element_)->value_;
        /// @endcond
    }

    reference operator*()
    {
        /// @cond
        return static_cast<iterable*>(element_)->value_;
        /// @endcond
    }

    Self& operator++()
    {
        /// @cond
        element_ = element_->pPrev_;
        return *this;
        /// @endcond
    }

   Self operator++(int)
    {
        /// @cond
        Self r(*this);
        element_ = element_->pPrev_;
        return r;
        /// @endcond
    }

    Self& operator--()
    {
        /// @cond
        element_ = element_->pNext_;
        return *this;
        /// @endcond
    }

    Self operator--(int)
    {
        /// @cond
        Self r(*this);
        element_ = element_->pNext_;
        return r;
        /// @endcond
    }
};

template<class vT>
inline bool operator==(const mt_list_reverse_iterator_t<vT> &it1, 
        const mt_list_reverse_iterator_t<vT, const vT> &it2)
{
    return it1.element_ == it2.element_;
}

template<class vT>
inline bool operator!=(const mt_list_reverse_iterator_t<vT> &it1, 
        const mt_list_reverse_iterator_t<vT, const vT> &it2)
{
    return it1.element_ != it2.element_;
}

} //namespace bloom

