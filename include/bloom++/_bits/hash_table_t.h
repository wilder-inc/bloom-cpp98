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

#include <utility>
#include <string.h>
#include <bloom++/_bits/c++config.h>
#include <bloom++/_bits/list_t.h>


#ifdef AUX_DEBUG
#define __BLOOM_WITH_DEBUG
#include <bloom++/log.h>
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

using std::pair;
using std::make_pair;


/**
 * @brief Hash table base.
 */
template<class kT, class vT, class hashT=hash<kT>, class rdpT = pair<const kT, vT> >
class hash_table_t : public list_t<rdpT >
{
public:
    typedef hash_table_t<kT, vT, hashT, rdpT>                           Self;
    typedef kT                                                          key_type;
    typedef vT                                                          value_type;
    typedef rdpT                                                        data_place;
    typedef list_t<data_place>                                          base_list;
    typedef list_iterable_t<data_place>                                 iterable;
    
private:
    /// @cond
    struct hash_pointer
    {
        list_iterable_base *pointer_;
        size_t size_;
        
        hash_pointer() : pointer_(0), size_(0)
        {}
    };
    /// @endcond

protected:

    void insert_iterable(const size_t index, list_iterable_base *obj){
        /// @cond
        if (hash_array_[index].pointer_)
            base_list::include(hash_array_[index].pointer_->pPrev_, obj);
        else 
            base_list::include(base_list::end_iterable()->pPrev_, obj);
        
        hash_array_[index].pointer_ = obj;
        hash_array_[index].size_++;

        if (hash_array_[index].size_ > collisions_limit_){
            DEBUG_INFO(log::pf("hash_table: Collisions(%d) are more then %d! rehashing... new hash_size = %d    %d\n", 
                       (int)hash_array_[index].size_, 
                       (int)collisions_limit_, 
                       (int)hash_size_ * 2, (size_t)this));
            rehash(hash_size_ * 2); // May be not *2?
        }
        /// @endcond
    }
    
    list_iterable_base *find_iterable(const size_t index, const key_type &key) const {
        /// @cond
        list_iterable_base *obj = hash_array_[index].pointer_;
        if (!obj)
            return base_list::end_iterable();//iterator(this, NULL); //NO OBJECT FOUND

        for(size_t i = 0; i < hash_array_[index].size_; i++){
            if (static_cast<iterable*>(obj)->value_.first == key)
                return obj;//iterator(this, obj);
            obj = obj->pNext_;
        }
        return base_list::end_iterable();//iterator(this, NULL); //NO OBJECT FOUND
        /// @endcond
    }
    
    iterable* erase_iterable(const size_t index, list_iterable_base *obj){
        /// @cond        
        hash_array_[index].size_--;
        if(!hash_array_[index].size_)
            hash_array_[index].pointer_ = 0;
        else if(hash_array_[index].pointer_ == obj)
            hash_array_[index].pointer_ = obj->pNext_;
        return base_list::exclude(obj);
        /// @endcond
    }
    
    void rehash(const size_t &hash_size)
    {
        /// @cond
        hash_size_ = hash_size;
        delete[] hash_array_;
        hash_array_ = new hash_pointer[hash_size];
        if(!base_list::size_)return;
        list_iterable_base *curr, *next = base_list::end_iterable()->pNext_;
        base_list::end_iterable()->pNext_ = base_list::end_iterable();
        base_list::end_iterable()->pPrev_ = base_list::end_iterable();
        base_list::size_ = 0;

        do
        {
            curr = next;
            next = next->pNext_;
            insert_iterable(hash_index(static_cast<iterable*>(curr)->value_.first), curr);
        }
        while (next != base_list::end_iterable());
        /// @endcond
    }
    
    void clear()
    {
        /// @cond
        base_list::clear();
        delete[] hash_array_;
        hash_array_ = new hash_pointer[hash_size_];
        /// @endcond
    }
    
    void swap(Self &ht){
        /// @cond
        if(&ht==this)return;
        std::swap(ht.hash_array_, hash_array_);
        std::swap(ht.hash_size_, hash_size_);
        std::swap(ht.collisions_limit_, collisions_limit_);
        base_list::swap(ht);
        /// @endcond
    }
    
    inline const size_t hash_index(const key_type &key) const FORCE_INLINE {
        return hashT()(key) % hash_size_;
    }

public:

    explicit hash_table_t(size_t hash_size, size_t collisions_limit = 8):
    hash_size_(hash_size),
    collisions_limit_(collisions_limit)
    {
        /// @cond
        hash_array_ = new hash_pointer[hash_size];
        /// @endcond
    }

    ~hash_table_t()
    {
        /// @cond
        delete[] hash_array_;
        /// @endcond
    }

private:
    /// @cond
    hash_pointer *hash_array_;
    size_t hash_size_;
    size_t collisions_limit_; //rehash if more then theat
    /// @endcond
};

} //namespace bloom
