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

#include<string>
#include<typeinfo>
#include<bloom++/string.h>

namespace bloom
{

namespace stream
{

class i_base;

/**
 * @brief Output stream base class.
 */
class o_base
{
public:
    virtual ~o_base(){}
    
    virtual size_t write(const char *data, size_t size) = 0;
    virtual o_base& operator<<(i_base &i) = 0;
    virtual bool oready() = 0;
};

/**
 * @brief Input stream base class.
 */
class i_base
{
public:
    virtual ~i_base(){};

    virtual size_t read(char *data, size_t size) = 0;
    virtual i_base &operator>>(o_base &o) = 0;
    virtual bool iready() = 0;
};

/**
 * @brief Base input/output stream class.
 */
class io_base : public o_base, public i_base
{
};

} //namespace stream

} //namespace bloom
