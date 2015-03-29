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

#include <bloom++/string.h>
#include <bloom++/stream/io.h>
#include <bloom++/stream/iobuffer.h>

#ifdef STREAM_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

namespace stream
{

class stringer: public io_base
{
private:
    bloom::string &str_;
    size_t ipos_;
    
public:
    stringer(bloom::string &str):str_(str),ipos_(0){}
    virtual ~stringer(){}
    
    virtual size_t write(const char *data, size_t size);
    virtual o_base& operator<<(i_base &i);
    virtual bool oready();
    
    virtual size_t read(char *data, size_t size);
    virtual i_base &operator>>(o_base &o);
    virtual bool iready();
    
private:
    explicit stringer(const stringer &o); //< Forbid default constructor
    explicit stringer(); //< Forbid default constructor
};

} //namespace stream

} //namespace bloom