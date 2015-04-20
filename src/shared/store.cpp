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

#include <bloom++/shared/store.h>


#ifdef SHARED_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

namespace shared
{

#ifdef SHARED_DEBUG
unsigned int storable::s_count_ = 0;
#endif

/**
 * storable
 */
storable::storable(store *container) :
container_(container)
{
#ifdef SHARED_DEBUG
    s_count_++;
    DEBUG_INFO("create storable s_count_:"<<s_count_<<" "<<this<<"\n");
#endif
}

void storable::remove_from_store()
{
    if (container_) {
        container_->remove(this);
    }
}

storable::~storable()
{
#ifdef SHARED_DEBUG
    s_count_--;
    DEBUG_INFO("destroy storable  s_count_:"<<s_count_<<" "<<this<<"\n");
#endif
}

} //namespace shared

} //namespace bloom
