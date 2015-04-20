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
#include <bloom++/set.h>

namespace bloom
{

namespace shared
{

template<class Tp>
class ptr;


class storable;
/**
 * @brief Counter class for shared objects.
 * 
 * This is a counter of object pointers. Also this
 * class contain info about all containers which contain
 * specified object.
 */
class shared_info
{
private:
    /// @cond
    mutable unsigned int count_;
    bloom::set<storable *> storables_;
#ifdef SHARED_DEBUG
    static unsigned int s_NumObjs_;
#endif
    /// @endcond

protected:
    
    void insert_storable(storable *data);
    void erase_storable(storable *data);
    void remove_from_all_containers();
    virtual ~shared_info();

public:
    template<class Tp>
    friend class ptr;
    
    friend class storable;
    shared_info();
};

} //namespace shared

} //namespace bloom



