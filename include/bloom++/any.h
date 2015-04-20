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
#include <bloom++/exception.h>


#ifdef AUX_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>


namespace bloom {

/**
 * Bad any cast exception.
 */
class bad_any_cast: public bloom::exception
{
public:
    bad_any_cast(string msg):exception(msg){}
    virtual ~bad_any_cast() throw(){}
};

/**
 * @brief Any type value keeper
 */
class any
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

public:
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
        if(!ptr){
            throw bad_any_cast(fast_ostring("bad cast to \"", 
                                            typeid(vT).name(), 
                                            "\"!"));
        }
        return ptr->value_;
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

};

} //namespace bloom
