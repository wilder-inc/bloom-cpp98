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

#include <bloom++/_bits/c++config.h>
#include <bloom++/_bits/hash_functions.h>
#include <bloom++/_bits/set_t.h>
#include <bloom++/_bits/mt_list_iterator_t.h>
#include <bloom++/mutex.h>
#include <bloom++/exception.h>

namespace bloom
{

/**
 * MT Set exception.
 */
class mt_set_exception: public exception
{
public:
    mt_set_exception(string msg):exception(msg){}
    virtual ~mt_set_exception() throw() {}
};

/**
 * @brief Multi-thread safe set.
 */
template<class kvT, class hashT=hash<kvT> >
class mt_set : public set_t<kvT, hashT >, public mt_store
{
public:
    typedef mt_set<kvT, hashT>                                          Self;
    typedef kvT                                                         key_type;
    typedef kvT                                                         value_type;
    typedef kvT                                                         data_place;
    typedef list_t<data_place>                                          base_list;
    typedef set_t<kvT, hashT >                                          base_set;
    typedef list_iterable_t<data_place>                                 iterable;
    typedef mt_list_iterator_t<data_place>                              iterator;
    typedef mt_list_iterator_t<data_place, const data_place >           const_iterator;
    typedef mt_list_reverse_iterator_t<data_place>                      reverse_iterator;
    typedef mt_list_reverse_iterator_t<data_place, const data_place>    const_reverse_iterator;

public:

    explicit mt_set(size_t hash_size, size_t collisions_limit = 8):
        base_set(hash_size, collisions_limit){}

    bool insert(const value_type &value)
    {
        /// @cond
        mutex::scoped_unilock sl(m_);
        const size_t index = base_set::hash_index(value);
        if (base_set::find_iterable(index, value) != base_list::end_iterable())return false; //Object with same key has been registered by now
        iterable *obj = new iterable(data_place(value));
        base_set::insert_iterable(index, obj);
        return true;
        /// @endcond
    }

    iterator erase(iterator &it) throw() {
        /// @cond
        iterator r(this, it.element_->pNext_);
        if(it.element_ != base_list::end_iterable())
            delete base_set::erase_iterable(hash_index(static_cast<iterable*>(it.element_)->value_.first), 
                                           it.element_);
        else
            throw set_exception("mt_set::erase faild!");
        return r;
        /// @endcond
    }
    
    bool erase(const key_type &key){
        /// @cond
        mutex::scoped_unilock sl(m_);
        const size_t index = base_set::hash_index(key);
        list_iterable_base *i = base_set::find_iterable(index, key);
        if(i != base_list::end_iterable()){
            delete base_set::erase_iterable(index, i);
            return true;
        }
        return false;
        /// @endcond
    }

    iterator find(const key_type &key){
        m_.lock();
        return iterator(this, base_set::find_iterable(base_set::hash_index(key), key));
    }
    
    const_iterator find(const key_type &key) const{
        m_.lock();
        return const_iterator(this, base_set::find_iterable(base_set::hash_index(key), key));
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
    
    inline size_t size() const {
        mutex::scoped_unilock sl(m_);
        return base_list::size();
    }
     
    inline void clear() FORCE_INLINE {
        mutex::scoped_unilock sl(m_);
        base_set::clear();
    }
    
    inline void swap(Self &s) FORCE_INLINE {
        mutex::scoped_unilock sl(m_);
        base_set::swap(s);
    }
    
    inline void rehash(size_t hash_size){
        mutex::scoped_unilock sl(m_);
        base_set::rehash();
    }
};

} //namespace bloom
