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
#include <bloom++/exception.h>

namespace bloom
{

namespace net
{

namespace udp
{

/**
 * UDP Socket exception.
 */
class socket_exception: public exception
{
public:
    socket_exception(string msg):exception(msg){}
    virtual ~socket_exception() throw() {}
};

/**
 * UDP Socket exception.
 */
class bad_socket_init: public socket_exception
{
public:
    bad_socket_init(string msg="failed!"):socket_exception(string("socket::socket: ")+msg){}
    virtual ~bad_socket_init() throw() {}
};

/**
 * UDP Socket exception.
 */
class bad_socket_bind: public socket_exception
{
public:
    bad_socket_bind(string msg="failed!"):socket_exception(string("socket::bind: ")+msg){}
    virtual ~bad_socket_bind() throw() {}
};

/**
 * UDP Socket exception.
 */
class bad_socket_multicast: public socket_exception
{
public:
    bad_socket_multicast(string msg="failed!"):socket_exception(string("socket::multicast: ")+msg){}
    virtual ~bad_socket_multicast() throw() {}
};

/**
 * UDP Socket exception.
 */
class bad_socket_broadcast: public socket_exception
{
public:
    bad_socket_broadcast(string msg="failed!"):socket_exception(string("socket::allow_broadcast: ")+msg){}
    virtual ~bad_socket_broadcast() throw() {}
};

/**
 * @brief UDP Socket.
 */
class socket : public socket_base
{
public:

    socket();
    ~socket(){}
    
    /**
     * @brief Initialize socket if not initialized and bind to address.
     * 
     * @param addr Address to bind.
     * @return Status of bind. sock_OK if not errors.
     */
    void bind(const addr_ipv4& addr);

#ifdef LINUX
    void multicast(const addr_ipv4 &group, const addr_ipv4 &src_iface);
#endif
    void allow_broadcast();
};

} //namespace udp

} //namespace net

} //namespace bloom
