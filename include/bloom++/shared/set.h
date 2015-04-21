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
#include <bloom++/shared/store.h>
#include <bloom++/_bits/hash_functions.h>
#include <bloom++/_bits/set_t.h>
#include <bloom++/_bits/shared_set_iterator_t.h>
#include <bloom++/exception.h>

#ifdef SHARED_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

template<class kvT> 
struct hash<shared::ptr<kvT> >
{
public:

    size_t operator()(shared::ptr<kvT> k)
    {
        return ((size_t) k.get() >> 8) ^ ((size_t) k.get() >> 16);
    }
};

namespace shared
{

/**
 * Set exception.
 */
class set_exception: public exception
{
public:
    set_exception(string msg):exception(msg){}
    virtual ~set_exception()  {}
};

/**
 * Hash table exception.
 */
class bad_set_erase: public ht_exception
{
public:
    bad_set_erase(string msg):ht_exception(string("shared::set::erase: ")+msg){}
    virtual ~bad_ht_erase() throw() {}
};

/**
 * @brief Hash table for shared objects.
 */
template<class kvT, class hashT=hash<ptr<kvT> > >
class set : public store, public set_t<ptr<kvT>, hashT, storable_single<kvT> >
{
public:
    typedef set<kvT, hashT>                                                 Self;
    typedef kvT                                                             key_type;
    typedef kvT                                                             value_type;
    typedef storable_single<kvT>                                            data_place;
    typedef list_t<data_place>                                              base_list;
    typedef set_t<ptr<kvT>, hashT, data_place>                              base_set;
    typedef list_iterable_t<data_place>                                     iterable;
    typedef shared_set_iterator_t<data_place>                               iterator;
    typedef shared_set_iterator_t<data_place, const data_place >            const_iterator;
    typedef shared_set_reverse_iterator_t<data_place>                       reverse_iterator;
    typedef shared_set_reverse_iterator_t<data_place, const data_place>     const_reverse_iterator;

public:

    explicit set(size_t hash_size, size_t collisions_limit = 8):
        base_set(hash_size, collisions_limit){}

    bool insert(ptr<value_type> key_value)
    {
        /// @cond
        const size_t index = base_set::hash_index(key_value);
        if (base_set::find_iterable(index, key_value) != base_list::end_iterable())return false; //Object with same key has been registered by now
        iterable *obj = new iterable(key_value, this);
        base_set::insert_iterable(index, obj);
        return true;
        /// @endcond
    }

    iterator erase(iterator &it)  {
        /// @cond
        iterator r(it.element_->pNext_);
        if(it.element_ != base_list::end_iterable_)
            delete base_set::erase_iterable(hash_index(static_cast<iterable*>(it.element_)->value_), 
                                           it.element_);
        else
            throw bad_set_erase("can't erase end element!");
        return r;
        /// @endcond
    }
    
    bool erase(ptr<value_type> key_value){
        /// @cond
        const size_t index = base_set::hash_index(key_value);
        list_iterable_base *i = base_set::find_iterable(index, key_value);
        if(i != base_list::end_iterable()){
            delete base_set::erase_iterable(index, i);
            return true;
        }
        return false;
        /// @endcond
    }

    iterator find(const key_type &key){
        return iterator(base_set::find_iterable(base_set::hash_index(key), key));
    }
    
    const_iterator find(const key_type &key) const{
        return const_iterator(base_set::find_iterable(base_set::hash_index(key), key));
    }
    
    iterator begin(){
        return iterator(base_list::end_iterable_->pNext_);
    }
    
    const_iterator begin() const {
        return const_iterator(base_list::end_iterable()->pNext_);
    }
    
    reverse_iterator rbegin(){
        return reverse_iterator(base_list::end_iterable_->pPrev_);
    }
    
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(base_list::end_iterable()->pPrev_);
    }
    
    iterator end(){
        return iterator(base_list::end_iterable_);
    }
    
    const_iterator end() const {
        return const_iterator(base_list::end_iterable());
    }
    
    reverse_iterator rend(){
        return reverse_iterator(base_list::end_iterable_);
    }
    
    const_reverse_iterator rend() const {
        return const_reverse_iterator(base_list::end_iterable());
    }
    
    inline size_t size() const FORCE_INLINE {
        return base_list::size();
    }
     
    inline void clear() FORCE_INLINE {
        base_set::clear();
    }
    
    inline void swap(Self &ht) FORCE_INLINE {
        base_set::swap(ht);
    }
    
    virtual void rehash(const size_t &hash_size)
    {
        /// @cond
        base_set::hash_size_ = hash_size;
        delete[] base_set::hash_array_;
        base_set::hash_array_ = new typename base_set::hash_pointer[hash_size];
        if(!base_list::size_)return;
        list_iterable_base *curr, *next = base_list::end_iterable()->pNext_;
        base_list::end_iterable()->pNext_ = base_list::end_iterable();
        base_list::end_iterable()->pPrev_ = base_list::end_iterable();
        base_list::size_ = 0;

        do
        {
            curr = next;
            next = next->pNext_;
            base_set::insert_iterable(base_set::hash_index(static_cast<iterable*>(curr)->value_.first), curr);
        }
        while (next != base_list::end_iterable());
        /// @endcond
    }

    virtual void remove(storable *data)
    {
        DEBUG_INFO("removing from store...\n");
        data_place *p = dynamic_cast<data_place *> (data);
        DEBUG_INFO("key: "<<(void*)p->first.get()<<"\n");
        if (p)this->erase(p->first);
        else
        {
            DEBUG_ERROR("slave_pair is NULL");
        }
        DEBUG_INFO("erase done...\n");
    }
};

} //namespace shared

} //namespace bloom
