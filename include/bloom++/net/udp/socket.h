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

#include <bloom++/net/socket_base.h>

namespace bloom
{

namespace net
{

namespace udp
{

/**
 * @brief UDP Socket.
 */
class socket : public socket_base
{
public:

    socket() : socket_base() {}
    ~socket(){}
    
    /**
     * @brief Initialize socket if not initialized and bind to address.
     * 
     * @param addr Address to bind.
     * @return Status of bind. sock_OK if not errors.
     */
    int bind(const addr_ipv4& addr);

#ifdef LINUX
    int multicast(const addr_ipv4 &group, const addr_ipv4 &src_iface);
#endif
    int allow_broadcast();
};

} //namespace udp

} //namespace net

} //namespace bloom
