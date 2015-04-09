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

#include <bloom++/shared_ptr.h>
#include <bloom++/mutex.h>
#include <bloom++/net/receiver_t.h>

namespace bloom
{

namespace net
{

namespace udp
{

class socket;
class communicator;
class sender;

typedef class receiver_t<sender> receiver;

/**
 * @brief Data sender for udp-sockets.
 */
class sender
{
public:
    friend class communicator;
    friend class receiver_t<sender>;
    
    explicit sender(shared_ptr<socket> sock);
    
    /**
     * @brief Send data to dest.
     * 
     * @param dest IP address of receiver.
     * @param data Data for send.
     * @param size Size of sending data.
     * @return Size of sended or errorcode.
     */
    size_t sendto(const addr_ipv4& dest, const char * data, size_t size);
    
    void close();
    bool is_closing() const;
    
    const addr_ipv4 &socket_addr() const;
    bloom::string get_if_name() const;
    int get_MAC(const string &ifaddr, char *mac) const;
    
    unsigned int hash() const;
    
private:
    sender(const sender &); //disable default constructor.
    
    /// @cond
    mutex recv_m_;
    mutex send_m_;
    shared_ptr<socket> socket_;
    bool bClosing_;
    /// @endcond
};

} //namespace udp

} //namespace net

} //namespace bloom