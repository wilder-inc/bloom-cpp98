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

#include <typeinfo>
#include <assert.h>

#ifdef BLOOM_WITH_EXCEPTIONS
#include <exception>
#endif

#ifdef BLOOM_ANY_WITH_SHARED
#include <bloom++/shared/base.h>
#endif

#ifdef BLOOM_ANY_WITH_DYNAMIC
#include <bloom++/dynamic/d_object_node.h>
#endif

#ifdef AUX_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

#include "dynamic/d_object_node.h"

namespace bloom {

#ifdef BLOOM_ANY_WITH_DYNAMIC
namespace dynamic {
#elif defined BLOOM_ANY_WITH_SHARED
namespace shared {
#endif

#ifdef BLOOM_WITH_EXCEPTIONS
class bad_any: public std::exception
{
protected:
    const char *message_;
        
public:
    //bad_any(bad_any &e):message_(e.message_){}
    bad_any(const char *msg):message_(msg){}
    virtual ~bad_any() throw(){}
    virtual const char *what() throw(){
        return message_;
    }
};
#endif

/**
 * @brief Any type value keeper
 */
class any
#ifdef BLOOM_ANY_WITH_DYNAMIC
: public dynamic::d_object_node
#elif defined BLOOM_ANY_WITH_SHARED
: public shared::base
#endif
{
protected:

    class keeper_base
    {
    public:

        virtual ~keeper_base()
        {
        }
        virtual const std::type_info & type() = 0;
        virtual keeper_base * clone() = 0;
    } *content_;

    template<class vT>
    class keeper : public keeper_base
    {
    public:
        vT value_;

        keeper(const vT &value) : value_(value){}

        virtual const std::type_info & type()
        {
            return typeid (vT);
        }

        virtual keeper_base * clone()
        {
            return new keeper(value_);
        }
    };

#ifndef BLOOM_ANY_WITH_SHARED
public:
#endif

    any() : content_(0)
    {
    }

    template<class vT> any(const vT &v)
    {
        content_ = new keeper<vT>(v);
    }
    
    any(const any &o) : content_(o.content_ ? o.content_->clone() : NULL)
    {
    }
    
public:
    
#ifdef BLOOM_ANY_WITH_SHARED
    
    static any *create(){
        return new any();
    }
    
    template<class vT>
    static any *create(const vT &v){
        return new any(v);
    }
    
    static any *create(const any &o){
        return new any(o);
    }
#endif

    ~any()
    {
        delete content_;
    }
    /**
     * @brief Check if it's empty.
     * @return true if empty, false if not.
     */
    bool empty() const
    {
        return !content_;
    }
    /**
     * @brief get type info
     * @return type info
     */
    const std::type_info & type() const
    {
        return content_ ? content_->type() : typeid (void);
    }
    /**
     * @brief Swap objects of class.
     * @param o 2th object
     * @return this object.
     */
    any & swap(any &o)
    {
        std::swap(content_, o.content_);
        return *this;
    }
    /**
     * @brief Convert to real-type.
     * Getting object of real type.
     * @return object of real type.
     */
    template<class vT>
    vT &get()
    {
        keeper<vT> *ptr = dynamic_cast<keeper<vT> *>(content_);
        if(ptr){
            return ptr->value_;
        }
        else {
#ifdef BLOOM_WITH_EXCEPTIONS
            throw bad_any(
#ifdef BLOOM_ANY_WITH_SHARED
                    "bloom::shared::any bad cast"
#elif defined BLOOM_ANY_WITH_DYNAMIC
                    "bloom::dynamic::any bad cast"
#else
                    "bloom::any bad cast"
#endif
                    );
#else
            DEBUG_ERROR("any bad cast...\n");
            assert(0);
#endif
        }
    }
    /**
     * @brief Convert to real-type.
     * Getting object of real-type.
     * @return 
     */
    template<class vT>
    operator vT(){
        return get<vT>();
    }
    
    template<class vT>
    any & operator=(const vT &v)
    {
        any(v).swap(*this);
        return *this;
    }

    any & operator=(const any &o)
    {
        any(o).swap(*this);
        return *this;
    }

    void reset()
    {
        any().swap(*this);
    }
#ifdef BLOOM_ANY_WITH_DYNAMIC
    /**
     * @brief Getting shared::ptr pointer of d_value.
     * @return shared::ptr pointer.
     */
    virtual shared::ptr<any> value(){
        return this;
    }
#endif
};

#ifdef BLOOM_ANY_WITH_DYNAMIC
} //namespace dynamic
#undef BLOOM_ANY_WITH_DYNAMIC
#elif defined BLOOM_ANY_WITH_SHARED
} //namespace shared
#undef BLOOM_ANY_WITH_SHARED
#endif

} //namespace bloom
