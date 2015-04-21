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

#include <bloom++/net/udp/socket.h>

#ifdef LINUX
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ifaddrs.h>
#define CSTPCSTR
#else
#include <winsock2.h>
#include <iphlpapi.h>
#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct ip_mreq
{
    struct in_addr imr_multiaddr;
    struct in_addr imr_interface;
} IP_MREQ, *PIP_MREQ;
#define CSTPCSTR (const char *)
#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#endif

#ifdef NET_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

namespace net
{

namespace udp
{

socket::socket():socket_base()
{
    sd_ = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (sd_ == -1){
        throw bad_socket_init();
    }
}

void socket::bind(const addr_ipv4& addr)
{
    if(socket_base::bind__(addr))
        throw bad_socket_bind();
}

#ifdef LINUX
void socket::multicast(const addr_ipv4 &group, const addr_ipv4 &src_iface)
{
    struct ip_mreq mreq;
    u_int yes = 1;
    
    if (setsockopt(sd_, SOL_SOCKET, SO_REUSEADDR, CSTPCSTR & yes, sizeof (yes)))
        throw bad_socket_multicast("set SO_REUSEADDR failed!");

    bind(src_iface);

    mreq.imr_multiaddr.s_addr = htonl(group.ip_int());
    mreq.imr_interface.s_addr = htonl(src_iface.ip_int());

    if (setsockopt(sd_, IPPROTO_IP, IP_ADD_MEMBERSHIP, CSTPCSTR & mreq, sizeof (mreq)))
        throw bad_socket_multicast("set IP_ADD_MEMBERSHIP failed!");
}
#endif

void socket::allow_broadcast()
{
    int op_val = 1;

#ifdef LINUX
    if (setsockopt(sd_, SOL_SOCKET, SO_BROADCAST, (void*) &op_val, sizeof (op_val)))
        throw bad_socket_broadcast("set SO_BROADCAST failed!");
#else
    if (setsockopt(sd_, SOL_SOCKET, SO_BROADCAST, CSTPCSTR & op_val, sizeof (op_val)))
        throw bad_socket_broadcast("set SO_BROADCAST failed!");
#endif
}

} //namespace udp

} //namespace net

} //namespace bloom
