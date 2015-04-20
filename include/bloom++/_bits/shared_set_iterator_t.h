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

#ifdef AUX_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

//extern int i_count;

namespace bloom
{

/**
 * @brief The iterator_t class
 * 
 * Using shared_set_iterator_t< type > for interator
 * and shared_set_iterator_t< type, const type > for const_interator.
 */
template<class vT, class rvT=vT>
struct shared_set_iterator_t
{
    typedef rvT                             value_type;
    typedef rvT &                           reference;
    typedef rvT *                           pointer;
    typedef shared_set_iterator_t<vT, rvT>  Self;
    typedef list_iterable_t<vT>             iterable;
    
    /// @cond
    list_iterable_base *element_;
    /// @endcond

    shared_set_iterator_t(): element_() {}
    
    explicit shared_set_iterator_t(list_iterable_base *i) : element_(i)
    {}

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
        return &static_cast<iterable*>(element_)->value_.first;
        /// @endcond
    }

    reference operator*()
    {
        /// @cond
        return static_cast<iterable*>(element_)->value_.first;
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
inline bool operator==(const shared_set_iterator_t<vT> &it1, 
        const shared_set_iterator_t<vT, const vT> &it2)
{
    return it1.element_ == it2.element_;
}

template<class vT>
inline bool operator!=(const shared_set_iterator_t<vT> &it1, 
        const shared_set_iterator_t<vT, const vT> &it2)
{
    return it1.element_ != it2.element_;
}

/**
 * @brief The shared_set_reverse_iterator_t class
 * 
 * Using shared_set_reverse_iterator_t< type > for reverse_interator
 * and shared_set_reverse_iterator_t< type, const type > for const_reverse_interator.
 */
template<class vT, class rvT=vT>
struct shared_set_reverse_iterator_t
{
    typedef rvT                             value_type;
    typedef rvT &                           reference;
    typedef rvT *                           pointer;
    typedef shared_set_reverse_iterator_t<vT, rvT> Self;
    typedef list_iterable_t<vT>             iterable;
    
    /// @cond
    list_iterable_base *element_;
    /// @endcond
    
    shared_set_reverse_iterator_t(): element_(){}

    explicit shared_set_reverse_iterator_t(list_iterable_base *i) : element_(i)
    {}

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
        return &static_cast<iterable*>(element_)->value_.first;
        /// @endcond
    }

    reference operator*()
    {
        /// @cond
        return static_cast<iterable*>(element_)->value_.first;
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
inline bool operator==(const shared_set_reverse_iterator_t<vT> &it1, 
        const shared_set_reverse_iterator_t<vT, const vT> &it2)
{
    return it1.element_ == it2.element_;
}

template<class vT>
inline bool operator!=(const shared_set_reverse_iterator_t<vT> &it1, 
        const shared_set_reverse_iterator_t<vT, const vT> &it2)
{
    return it1.element_ != it2.element_;
}

} //namespace bloom

