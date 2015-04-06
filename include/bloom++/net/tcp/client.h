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
#include <bloom++/net/tcp/connection.h>
#include <bloom++/signal.h>
#include <bloom++/list.h>
#include <bloom++/thread.h>
#include <bloom++/condition_variable.h>

namespace bloom
{

namespace net
{

namespace tcp
{

/**
 * @brief TCP client.
 * 
 * @param ipaddr IPv4 address.
 * @param numExecutors number of executors threads.
 */
class client
{
public:

    client(int numExecutors, 
           size_t select_timeout_sec, 
           size_t select_timeout_usec);
    ~client();

    int bind(const addr_ipv4& ipaddr);
    int connect(const addr_ipv4& serverip);

    signal1<void, connection& > &connector();
    signal2<bool, receiver&, connection&> &executor();
    signal1<void, connection& > &disconnector();
    
private:
    /// @cond
    int numExecutors_;
    size_t select_timeout_sec_;
    size_t select_timeout_usec_;
    bool bBlocking_;

    list<shared_ptr<thread<client> > > executors_;
    mutex clientMutex_;
    condition_variable cv_;
    
    signal1<void, connection& > connector_;
    signal2<bool, receiver&, connection&> executor_;
    signal1<void, connection& > disconnector_;

    //mutex mutexExecutors_;
    shared_ptr<socket> socket_;
    shared_ptr<connection> connection_;

    bool bStopping_;

    void runExecutor();
    
    //Signal callbacks
    bool scb_allow(connection&){return true;}
    bool scb_deny(connection&){return false;}
    void scb_done(connection&){}
    /// @endcond
};

} //namespace tcp

} //namespace net

} //namespace bloom
