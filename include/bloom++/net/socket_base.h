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

#include <bloom++/net/addr_ipv4.h>
#include <bloom++/mutex.h>

namespace bloom 
{

namespace net
{

template<class Tc>
class receiver_t;

template<class Tc>
class sender_t;
/**
 * @brief Return values for sockets functions.
 */
typedef enum
{
    sock_OK = 0,
    sock_ERROR = -1,
    sock_FD_ERROR = -2,
    sock_BIND_ERROR = -3,
    sock_SELECT_ERROR = -4,
    sock_ACCEPT_ERROR = -5,
    sock_CONNECT_ERROR = -6,
    sock_IPADDR_ERROR = -7,
    sock_TIMEOUT = -8,
    sock_BUSY = -9,
    sock_SELECT_NOTHING = -10,
    sock_SELECT_READY = -11
} return_value;

typedef enum
{
    select_Write = 1,
    select_Read =  2
} select_type;

/**
 * @brief Base class for sockets.
 */
class socket_base
{
public:
    template<class Tc> friend class sender_t;
    template<class Tc> friend class receiver_t;

    socket_base();
    ~socket_base();
    
    bloom::string get_if_name() const;
    int get_MAC(const string &ifaddr, char *mac) const;

    int select(int tv_sec, int tv_usec, select_type type);
    int select(timeval &timeVal, select_type type);

    size_t send(const char * data, size_t len);
    size_t sendto(const addr_ipv4& dest, const char * data, size_t len);
    size_t recv(char * data, size_t len);
    size_t recvfrom(char * data, size_t len, addr_ipv4& from);
    
    int read_socket_addr(addr_ipv4 &addr) const;
    const addr_ipv4 &socket_addr() const;
    
    void close();
    bool is_closed() const;
    
    unsigned int hash() const;
    
protected:
    
#ifdef LINUX
    explicit socket_base(int sd, const addr_ipv4 &addr);
#elif defined MS_WINDOWS
    explicit socket_base(SOCKET sd);
#endif
    
    /// @cond
#ifdef LINUX
    int sd_;
#elif defined MS_WINDOWS
    SOCKET sd_;
#endif
    
    int bind__(const addr_ipv4& addr);
    
    addr_ipv4 addr_;
    /// @endcond
};

} //namespace net

} //namespace bloom
