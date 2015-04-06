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

#include <bloom++/net/tcp/connection.h>

#ifdef NET_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

namespace net
{

namespace tcp
{

connection::connection(shared_ptr<socket> sock, const addr_ipv4& remote):
socket_(sock), remoteAddr_(remote), bClosing_(false)
{
    //DEBUG_INFO("creating connection...\n");
}

connection::~connection()
{
    DEBUG_INFO("destroying connection...\n");
}

size_t connection::send(const char* data, size_t len)
{
    mutex::scoped_lock sl(send_m_);
    return socket_->send(data, len);
}

size_t connection::sendto(const addr_ipv4& dest, const char* data, size_t len)
{
    mutex::scoped_lock sl(send_m_);
    return socket_->sendto(dest, data, len);
}

const addr_ipv4& connection::socket_addr() const
{
    return socket_->socket_addr();
}

const addr_ipv4& connection::remote_addr() const
{
    return remoteAddr_;
}

void connection::close()
{
    bClosing_ = true;
}

bool connection::is_closing() const
{
    return bClosing_;
}

unsigned int connection::hash() const
{
    return socket_->hash();
}

} //namespace tcp

} //namespace net

} //namespace bloom
