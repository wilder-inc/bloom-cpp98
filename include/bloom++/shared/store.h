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

#include <bloom++/shared/ptr.h>


#ifdef SHARED_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

namespace shared
{

class storable;

/**
 * @brief Container info for shared objects.
 * This is a base class of all shared containers.
 */
class store
{
public:

    store()
    {
    }

    virtual ~store()
    {
    }
    virtual void remove(storable *d) = 0;
};

/**
 * @brief Base class of keeper keys with shared objects.
 * Keys need for removing objects from containers.
 */
class storable
{
#ifdef SHARED_DEBUG
private:
    static unsigned int s_count_;
#endif
protected:
    /// @cond
    store * container_;
    /// @endcond
    
    template<class ValueT>
    void insert_storable(ptr<ValueT> v){
        /// @cond
        if(v.counter_)
            v.counter_->insert_storable(this);
        /// @endcond
    }
    
    template<class ValueT>
    void erase_storable(ptr<ValueT> v){
        /// @cond
        if(v.counter_)
            v.counter_->erase_storable(this);
        /// @endcond
    }

public:
    storable(store *container);

    void remove_from_store();
    virtual ~storable();
};

/**
 * @brief Keeper shared object with same key.
 */
template<class ValueT>
class storable_single: public storable
{
public:    
    ptr<ValueT> first;
    
    storable_single(ptr<ValueT> value, store *container):
        storable(container), first(value)
    {
        /// @cond
        insert_storable(first);
        /// @endcond
    }
    virtual ~storable_single(){
        /// @cond
        erase_storable(first);
        /// @endcond
    }
    
    bool operator==(ptr<ValueT> value){
        return first==value;
    }
    
    bool operator!=(ptr<ValueT> value){
        return first!=value;
    }
};

/**
 * @brief Keeper key with shared object.
 */
template<class KeyT, class ValueT>
class storable_pair : public storable
{
public:
    KeyT            first;
    ptr<ValueT>     second;

    storable_pair(const KeyT &key, ptr<ValueT> value, store *container) : 
        storable(container), first(key), second(value)
    {
        /// @cond
        insert_storable(second);
        /// @endcond
    }

    virtual ~storable_pair()
    {
        /// @cond
        erase_storable(second);
        /// @endcond
    }
};

/**
 * @brief Keeper two keys with shared object.
 */
template<class KeyT1, class KeyT2, class ValueT>
class storable_triad : public storable
{
public:
    KeyT1           first;
    ptr<ValueT>     second;
    KeyT2           third;

    storable_triad(const KeyT1 &key1, const KeyT2 &key2, ptr<ValueT> value, store *container) : 
        storable(container), first(key1), second(value), third(key2)
    {
        /// @cond
        insert_storable(second);
        /// @endcond
    }

    virtual ~storable_triad()
    {
        /// @cond
        erase_storable(second);
        /// @endcond
    }
};

} //namespace shared

} //namespace bloom
