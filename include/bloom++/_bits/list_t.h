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

#include <algorithm>
#include <stddef.h>
#include <bloom++/_bits/list_iterable_t.h>
#include <bloom++/_bits/c++config.h>

#ifdef AUX_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

/**
 * @brief Base class for list-based containers.
 */
template<class real_vT>
class list_t
{
protected:
    typedef list_t<real_vT>                     Self;
    typedef list_iterable_t<real_vT>            real_iterable_t;
    
protected:
    /// @cond
    size_t size_;
    list_iterable_base end_iterable_[1];
    
    inline size_t init_empty() const FORCE_INLINE {
        const_cast<list_iterable_base*>(end_iterable_)->pNext_ = const_cast<list_iterable_base *>(end_iterable_);
        const_cast<list_iterable_base*>(end_iterable_)->pPrev_ = const_cast<list_iterable_base *>(end_iterable_);
        return 0;
    }
    /// @endcond
    
    inline list_iterable_base *end_iterable() const FORCE_INLINE {
        /// @cond
        return const_cast<list_iterable_base *>(end_iterable_);
        /// @endcond
    }
    
    inline void include(list_iterable_base *i_prev, list_iterable_base *i) FORCE_INLINE {
        /// @cond
        i->pPrev_ = i_prev;
        i->pNext_ = i_prev->pNext_;

        i_prev->pNext_ = i;
        i->pNext_->pPrev_ = i;
        
        ++size_;
        /// @endcond
    }

    inline real_iterable_t* exclude(list_iterable_base *i) FORCE_INLINE {
        /// @cond
        i->pPrev_->pNext_ = i->pNext_;
        i->pNext_->pPrev_ = i->pPrev_;

        size_--;
        return static_cast<real_iterable_t*>(i);
        /// @endcond
    }
    
    inline void transmit(list_iterable_base *i, list_t &c) FORCE_INLINE {
        /// @cond
        list_iterable_base *cl = c.end_iterable_;
        cl->pPrev_->pNext_ = i->pNext_;
        i->pNext_->pPrev_ = cl->pPrev_;
        cl->pNext_->pPrev_ = i;
        i->pNext_ = cl->pNext_;
        size_ += c.size_;

        cl->pNext_ = cl;
        cl->pPrev_ = cl;
        c.size_ = 0;
        /// @endcond
    }

    inline void swap(Self &c) FORCE_INLINE {
        /// @cond
        if (&c == this)return;
        std::swap(c.end_iterable_[0], end_iterable_[0]);
        c.end_iterable_[0].pNext_->pPrev_ = c.end_iterable_;
        c.end_iterable_[0].pPrev_->pNext_ = c.end_iterable_;
        end_iterable_[0].pNext_->pPrev_ = end_iterable_;
        end_iterable_[0].pPrev_->pNext_ = end_iterable_;
        std::swap(c.size_, size_);
        /// @endcond
    }
    
    inline void clear() FORCE_INLINE {
        /// @cond
        if(!size_)return;
        {
            list_iterable_base *current = end_iterable_->pNext_, *prev;
            size_ = 0;
            while(current != end_iterable_)
            {
                prev = current;
                current = current->pNext_;
                delete static_cast<real_iterable_t*>(prev);
            }
            end_iterable_->pNext_ = end_iterable_;
            end_iterable_->pPrev_ = end_iterable_;
        }
        /// @endcond
    }
    
    inline size_t size() const FORCE_INLINE {
        /// @cond
        return size_;
        /// @endcond
    }

public:

    list_t() : size_(init_empty())
    {}

    ~list_t(){
        /// @cond
        if(size_)
        {
            list_iterable_base *current = end_iterable_->pNext_, *prev;
            while(current != end_iterable_)
            {
                prev = current;
                current = current->pNext_;
                delete static_cast<real_iterable_t*>(prev);
            }
        }
        /// @endcond
    }
    
private:
    list_t(const list_t &);
};

} //namespace bloom
