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

#include <bloom++/shared/shared_info.h>
#include <bloom++/shared/store.h>
#include <bloom++/set.h>


#ifdef SHARED_DEBUG
#include <bloom++/log.h>
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

template<>
struct hash<shared::storable *>
{
public:

    size_t operator()(shared::storable * const&k)
    {
        return ((size_t) k >> 8) ^ ((size_t) k >> 16);
    }
};

namespace shared
{

/**
 * base methods
 */

#ifdef SHARED_DEBUG
unsigned int shared_info::s_NumObjs_ = 0;
#endif

shared_info::shared_info() :
count_(1),
storables_(8)
{
#ifdef SHARED_DEBUG
    s_NumObjs_++;
    DEBUG_INFO("created shared::base... num of objects: "<<s_NumObjs_<<"\n");
#endif
}

shared_info::~shared_info()
{
    DEBUG_INFO("begin..."<<(size_t)this<<"\n");

    /*
     * Not need to invoke remove_from_all_containers(), because this
     * object destroying only if pointers counter == 0.
     */
    
#ifdef SHARED_DEBUG
    s_NumObjs_--;
#endif
    DEBUG_INFO("done "<<(size_t)this
#ifdef SHARED_DEBUG
            <<" num of objects: "<<s_NumObjs_
#endif
            <<"\n");
}

void shared_info::remove_from_all_containers()
{
    DEBUG_INFO("begin...\n");
    bloom::set<storable *>::iterator it = storables_.begin();
    bloom::set<storable *>::iterator end_it = storables_.end();
    while (it != end_it )
    {
        DEBUG_INFO("storables_ size: "<<storables_.size()<<"\n");
        storable *data = *it;
        if (data)
        {
            DEBUG_INFO(log::pf("delete storable data %d\n", data));
            data->remove_from_store();
            erase_storable(data);
            DEBUG_INFO("delete storable data done...\n");
        }
        else
        {
            DEBUG_ERROR("storable * = 0");
            storables_.erase(data);
        }
        it = storables_.begin();
    }
    DEBUG_INFO("done...\n");
}


void shared_info::insert_storable(storable *data)
{
    storables_.insert(data);
}

void shared_info::erase_storable(storable *data)
{
    storables_.erase(data);
}


} //namespace shared

} //namespace bloom
