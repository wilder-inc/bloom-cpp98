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
 * @brief TCP Server.
 * @param ipaddr IPv4 address.
 * @param numAcceptors number of acceptors threads.
 * @param numExecutors number of executors threads.
 */
class server
{
public:

    server(int numAcceptors,
           int numExecutors,
           size_t select_timeout_sec, 
           size_t select_timeout_usec);
    ~server();

    int bind(const addr_ipv4& ipaddr);
    signal1<bool, connection& > &acceptor();
    signal2<bool, receiver&, connection& > &executor();
    signal1<void, connection& > &disconnector();
    void allow_acceptor();
    void deny_acceptor();

private:
    /// @cond
    int numAcceptors_;
    int numExecutors_;
    size_t select_timeout_sec_;
    size_t select_timeout_usec_;
    bool bBlocking_;

    list<shared_ptr<thread<server> > > acceptors_;
    list<shared_ptr<thread<server> > > executors_;

    list<shared_ptr<connection> > connections_;

    mutex mutexAcceptors_;
    condition_variable cvAcceptors_;
    mutex mutexExecutors_;
    mutex mutexConnections_;
    condition_variable cvConnections_;
    shared_ptr<socket> socket_;
    signal1<bool, connection& > acceptor_;
    signal2<bool, receiver&, connection& > executor_;
    signal1<void, connection& > disconnector_;

    bool bStopping_;

    void runAcceptor();
    void runExecutor();
    
    //Signal callbacks
    bool scb_allow(connection&){return true;}
    bool scb_deny(connection&){return false;}
    void scb_done(connection& conn){}
    /// @endcond
};

} //namespace tcp

} //namespace net

} //namespace bloom
