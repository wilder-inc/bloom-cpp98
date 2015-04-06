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

#ifdef LINUX
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#elif defined MS_WINDOWS
#include <winsock2.h>
#endif

namespace bloom
{

namespace net
{

class socket_base;

namespace tcp {

class socket;

} //namespace tcp

namespace udp {

class socket;

} //namespace udp

/**
 * @brief IPv4 address.
 */
class addr_ipv4
{
private:
    /// @cond
    struct sockaddr_in sockaddr_in_;
    /// @endcond
    friend class socket_base;
    friend class tcp::socket;
    friend class udp::socket;

public:

    addr_ipv4();
    explicit addr_ipv4(unsigned short port);
    explicit addr_ipv4(const string &ip, unsigned short port);
    explicit addr_ipv4(unsigned int ip, unsigned short port);
    addr_ipv4(const addr_ipv4 &addr);
    ~addr_ipv4();

    bool set_ip(const string &ip);
    void set_port(unsigned short port);
    
    string ip() const;
    unsigned short port() const;
    unsigned int ip_int() const;
    
    bool operator==(const addr_ipv4 &addr) const;
    bool operator!=(const addr_ipv4 &addr) const;
    
    addr_ipv4& operator=(const addr_ipv4 &addr);
};

} //namespace net

} //namespace bloom