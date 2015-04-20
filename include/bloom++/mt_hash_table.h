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
#include <bloom++/_bits/hash_functions.h>
#include <bloom++/_bits/hash_table_t.h>
#include <bloom++/exception.h>

namespace bloom
{

/**
 * Multi-thread safe hash table exception.
 */
class mtht_exception: public exception
{
public:
    mtht_exception(string msg):exception(msg){}
    virtual ~mtht_exception() throw() {}
};

/**
 * @brief Multi-thread safe hash table.
 */
template<class kT, class vT, class hashT=hash<kT> >
class mt_hash_table : public hash_table_t<kT, vT, hashT >, public mt_store
{
public:
    typedef mt_hash_table<kT, vT, hashT>                                Self;
    typedef kT                                                          key_type;
    typedef vT                                                          value_type;
    typedef pair<const kT, vT >                                         data_place;
    typedef list_t<data_place>                                          base_list;
    typedef hash_table_t<kT, vT, hashT>                                 base_ht;
    typedef list_iterable_t<data_place>                                 iterable;
    typedef mt_list_iterator_t<data_place>                              iterator;
    typedef mt_list_iterator_t<data_place, const data_place >           const_iterator;
    typedef mt_list_reverse_iterator_t<data_place>                      reverse_iterator;
    typedef mt_list_reverse_iterator_t<data_place, const data_place>    const_reverse_iterator;

public:

    explicit mt_hash_table(size_t hash_size, size_t collisions_limit = 8):
        base_ht(hash_size, collisions_limit){}

    bool insert(const key_type &key, const value_type &value)
    {
        /// @cond
        mutex::scoped_unilock sl(m_);
        const size_t index = base_ht::hash_index(key);
        if (base_ht::find_iterable(index, key) != base_list::end_iterable())return false; //Object with same key has been registered by now
        iterable *obj = new iterable(data_place(key, value));
        base_ht::insert_iterable(index, obj);
        return true;
        /// @endcond
    }

    iterator erase(iterator &it) throw(){
        /// @cond
        iterator r(this, it.element_->pNext_);
        if(it.element_ != base_list::end_iterable_)
            delete base_ht::erase_iterable(base_ht::hash_index(static_cast<iterable*>(it.element_)->value_.first), 
                                           it.element_);
        else
            throw mtht_exception("mt_hash_table::erase faild!");
        return r;
        /// @endcond
    }
    
    bool erase(const key_type &key){
        /// @cond
        mutex::scoped_unilock sl(m_);
        const size_t index = base_ht::hash_index(key);
        list_iterable_base *i = base_ht::find_iterable(index, key);
        if(i != base_list::end_iterable()){
            delete base_ht::erase_iterable(index, i);
            return true;
        }
        return false;
        /// @endcond
    }

    iterator find(const key_type &key){
        /// @cond
        m_.lock();
        return iterator(this, base_ht::find_iterable(base_ht::hash_index(key), key));
        /// @endcond
    }
    
    const_iterator find(const key_type &key) const{
        /// @cond
        m_.lock();
        return const_iterator(this, base_ht::find_iterable(base_ht::hash_index(key), key));
        /// @endcond
    }
    
    iterator begin(){
        /// @cond
        m_.lock();
        return iterator(this, base_list::end_iterable_->pNext_);
        /// @endcond
    }
    
    const_iterator begin() const {
        /// @cond
        m_.lock();
        return const_iterator(this, base_list::end_iterable()->pNext_);
        /// @endcond
    }
    
    reverse_iterator rbegin(){
        /// @cond
        m_.lock();
        return reverse_iterator(this, base_list::end_iterable_->pPrev_);
        /// @endcond
    }
    
    const_reverse_iterator rbegin() const {
        /// @cond
        m_.lock();
        return const_reverse_iterator(this, base_list::end_iterable()->pPrev_);
        /// @endcond
    }
    
    iterator end(){
        /// @cond
        m_.lock();
        return iterator(this, base_list::end_iterable_);
        /// @endcond
    }
    
    const_iterator end() const {
        /// @cond
        m_.lock();
        return const_iterator(this, base_list::end_iterable());
        /// @endcond
    }
    
    reverse_iterator rend(){
        /// @cond
        m_.lock();
        return reverse_iterator(this, base_list::end_iterable_);
        /// @endcond
    }
    
    const_reverse_iterator rend() const {
        /// @cond
        m_.lock();
        return const_reverse_iterator(this, base_list::end_iterable());
        /// @endcond
    }
    
    inline size_t size() const FORCE_INLINE {
        /// @cond
        mutex::scoped_unilock sl(m_);
        return base_list::size();
        /// @endcond
    }
     
    inline void clear() FORCE_INLINE {
        /// @cond
        mutex::scoped_unilock sl(m_);
        base_ht::clear();
        /// @endcond
    }
    
    inline void swap(Self &ht) FORCE_INLINE {
        /// @cond
        mutex::scoped_unilock sl(m_);
        base_ht::swap(ht);
        /// @endcond
    }
    
    inline void rehash(size_t hash_size) FORCE_INLINE {
        /// @cond
        mutex::scoped_unilock sl(m_);
        base_ht::rehash();
        /// @endcond
    }
};

} //namespace bloom
