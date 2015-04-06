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

#include <bloom++/net/addr_ipv4.h>

#ifdef NET_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

namespace net
{

//=================================
//  addr_ipv4
//=================================

addr_ipv4::addr_ipv4()
{
    sockaddr_in_.sin_family = AF_INET;
}

addr_ipv4::addr_ipv4(unsigned short port)
{
    sockaddr_in_.sin_family = AF_INET;
    sockaddr_in_.sin_addr.s_addr = htonl(INADDR_ANY); // */inet_addr("127.0.0.255");
    sockaddr_in_.sin_port = htons(port);
}

addr_ipv4::addr_ipv4(const string &ip, unsigned short port)
{
    sockaddr_in_.sin_family = AF_INET;
    sockaddr_in_.sin_addr.s_addr = inet_addr(ip.c_str());
    sockaddr_in_.sin_port = htons(port);
}

addr_ipv4::addr_ipv4(unsigned int ip, unsigned short port)
{
    sockaddr_in_.sin_family = AF_INET;
    sockaddr_in_.sin_addr.s_addr = htonl(ip);
    sockaddr_in_.sin_port = htons(port);
}

addr_ipv4::addr_ipv4(const addr_ipv4 &addr)
{
    sockaddr_in_.sin_family = addr.sockaddr_in_.sin_family;
    sockaddr_in_.sin_addr.s_addr = addr.sockaddr_in_.sin_addr.s_addr;
    sockaddr_in_.sin_port = addr.sockaddr_in_.sin_port;
}

addr_ipv4::~addr_ipv4()
{
}

bool addr_ipv4::set_ip(const string &ip)
{
    sockaddr_in_.sin_addr.s_addr = inet_addr(ip.c_str());
    if (sockaddr_in_.sin_addr.s_addr == INADDR_NONE){
        if(ip != "255.255.255.255")
            return false;
    }
    return true;
}

void addr_ipv4::set_port(unsigned short port)
{
    sockaddr_in_.sin_port = htons(port);
}

string addr_ipv4::ip() const
{
    return inet_ntoa(sockaddr_in_.sin_addr);
}

unsigned short addr_ipv4::port() const
{
    return ntohs(sockaddr_in_.sin_port);
}

unsigned int addr_ipv4::ip_int() const
{
    return ntohl(sockaddr_in_.sin_addr.s_addr);
}

bool addr_ipv4::operator==(const addr_ipv4 &addr) const
{
    if (sockaddr_in_.sin_addr.s_addr == addr.sockaddr_in_.sin_addr.s_addr &&
            sockaddr_in_.sin_port == addr.sockaddr_in_.sin_port)
        return true;
    return false;
}

bool addr_ipv4::operator!=(const addr_ipv4 &addr) const
{
    if (sockaddr_in_.sin_addr.s_addr != addr.sockaddr_in_.sin_addr.s_addr ||
            sockaddr_in_.sin_port != addr.sockaddr_in_.sin_port)
        return true;
    return false;
}

addr_ipv4& addr_ipv4::operator=(const addr_ipv4 &addr)
{
    sockaddr_in_.sin_family = addr.sockaddr_in_.sin_family;
    sockaddr_in_.sin_addr.s_addr = addr.sockaddr_in_.sin_addr.s_addr;
    sockaddr_in_.sin_port = addr.sockaddr_in_.sin_port;
    return *this;
}

} //namespace net

} //namespace bloom
