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

#include <bloom++/net/tcp/stream.h>

#ifdef NET_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom {

namespace net {

namespace tcp {

iostream::iostream(receiver &r, connection &s):
r_(r),s_(s)
{
}

iostream::~iostream()
{
}

size_t iostream::write(const char* data, size_t size)
{
    size_t ret = s_.send(data, size);
    return ret;
}

size_t iostream::read(char* data, size_t size)
{
    return r_.recv(data, size);
}

bool iostream::iready()
{
    return !r_.is_closing();
}

bool iostream::oready()
{
    return !s_.is_closing();
}

stream::o_base &iostream::operator <<(stream::i_base &i)
{
    i>>*this;
    return *this;
}

stream::i_base &iostream::operator>>(o_base &o)
{
    //TODO:
    return *this;
}

} //namespace tcp

} //namespace net

} //namespace bloom
