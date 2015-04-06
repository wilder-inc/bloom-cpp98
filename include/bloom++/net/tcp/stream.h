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

#include <bloom++/stream/io.h>
#include <bloom++/net/tcp/connection.h>

namespace bloom {

namespace net {

namespace tcp {

/**
 * @brief Input/output stream for TCP Connection.
 * @param conn TCP Connection.
 */
class iostream: public stream::io_base
{
public:
    iostream(receiver &r, connection &s);
    virtual ~iostream();
    
    using stream::io_base::operator <<;
    using stream::io_base::operator >>;
    virtual o_base& operator<<(i_base &i);
    virtual i_base& operator>>(o_base &o);
    virtual size_t write(const char *data, size_t size);
    virtual size_t read(char *data, size_t size);
    virtual bool iready();
    virtual bool oready();
private:
    /// @cond
    receiver& r_;
    connection& s_;
    /// @endcond
};

} //namespace tcp

} //namespace net

} //namespace bloom
