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

#include <bloom++/net/udp/sender.h>
#include <bloom++/net/udp/socket.h>

namespace bloom
{

namespace net
{

namespace udp
{

sender::sender(shared_ptr<socket> sock): socket_(sock)
{
}

size_t sender::sendto(const addr_ipv4& dest, const char* data, size_t size)
{
    mutex::scoped_lock sl(send_m_);
    return socket_->sendto(dest, data, size);
}

void sender::close()
{
    bClosing_ = true;
}

bool sender::is_closing() const
{
    return bClosing_;
}

const addr_ipv4 &sender::socket_addr() const
{
    return socket_->socket_addr();
}

unsigned int sender::hash() const
{
    return socket_->hash();
}

bloom::string sender::get_if_name() const
{
    return socket_->get_if_name();
}

int sender::get_MAC(const string &ifaddr, char *mac) const
{
    return socket_->get_MAC(ifaddr, mac);
}

} //namespace udp

} //namespace net

} //namespace bloom
