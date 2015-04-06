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

#include <bloom++/net/tcp/socket.h>
#include <bloom++/net/tcp/connection.h>

#ifdef NET_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

#include "bloom++/shared_ptr.h"

namespace bloom
{

namespace net
{

namespace tcp
{

socket::socket(int sd, const addr_ipv4& addr):
socket_base(sd, addr)
{
}

int socket::bind(const addr_ipv4& addr)
{
    socket_base::close();
    sd_ = ::socket(AF_INET, SOCK_STREAM, 0/*ipproto*/);
    if (sd_ == -1)
    {
        DEBUG_WARN("socket - could not create a socket\n");
        return sock_ERROR;
    }
    
    return socket_base::bind__(addr);
}

int socket::listen()
{
    int ret = ::listen(sd_, 2048);
    if(ret == 0){
        return sock_OK;
    }
    return sock_ERROR;
}

int socket::accept(addr_ipv4 &remote)
{
#ifdef LINUX
    unsigned int dest_len =
#else
    int dest_len =
#endif
            sizeof (struct sockaddr_in);
    
    int connfd = ::accept(sd_, (sockaddr *) (&remote.sockaddr_in_), &dest_len);
    if(connfd == -1){
        return sock_ACCEPT_ERROR;
    }
    
    return connfd;
}

int socket::connect(const addr_ipv4& dest)
{
    int connret = ::connect(sd_, (sockaddr *) (&dest.sockaddr_in_), sizeof (struct sockaddr_in));
    if(connret == 0){
        return sock_OK;
    }
    return sock_CONNECT_ERROR;
}

} //namespace tcp

} //namespace net

} //namespace bloom
