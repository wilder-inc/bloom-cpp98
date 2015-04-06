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

#include <bloom++/net/tcp/socket.h>
#include <bloom++/net/receiver_t.h>

namespace bloom
{

namespace net
{

namespace tcp
{

class client;
class server;
class connection;

typedef receiver_t<connection> receiver;

/**
 * @brief TCP connection.
 * 
 * @param s socket.
 * @param remote IPv4 address.
 */
class connection
{
public:
    friend class client;
    friend class server;
    friend class receiver_t<connection>;

    explicit connection(shared_ptr<socket> s, const addr_ipv4& remote);
    ~connection();
    
    size_t send(const char * data, size_t len);
    size_t sendto(const addr_ipv4& dest, const char * data, size_t len);
    
    void close();
    bool is_closing() const;
    
    const addr_ipv4 &remote_addr() const;
    const addr_ipv4 &socket_addr() const;
    
    unsigned int hash() const;

protected:
 
    /// @cond
    addr_ipv4 remoteAddr_;
    mutex recv_m_;
    mutex send_m_;
    shared_ptr<socket> socket_;
    bool bClosing_;
    /// @endcond
    
private:
    connection(const connection &); //disable default constructor
};

} //namespace tcp

} //namespace net

} //namespace bloom
