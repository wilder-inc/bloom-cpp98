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

#include <bloom++/signal.h>
#include <bloom++/thread.h>
#include <bloom++/list.h>
#include <bloom++/condition_variable.h>
#include <bloom++/net/udp/socket.h>
#include <bloom++/net/udp/sender.h>

namespace bloom
{

namespace net
{

namespace udp
{

/**
 * @brief UDP communicator.
 * 
 * @param numExecutors number of executors threads.
 * @param select_timeout_sec Select timeout sec.
 * @param select_timeout_usec Select timeout usec.
 */
class communicator
{
public:

    communicator(int numExecutors, 
                 size_t select_timeout_sec, 
                 size_t select_timeout_usec);
    ~communicator();
    
    signal2<bool, receiver &, sender & > &executor();
    
    int bind(const addr_ipv4& ipaddr);
#ifdef LINUX
    int multicast(const addr_ipv4 &group, const addr_ipv4 &src_iface);
#endif
    int allow_broadcast();
    
    sender& get_sender();
    
protected:
    /// @cond
    int numExecutors_;
    size_t select_timeout_sec_;
    size_t select_timeout_usec_;
    bool bBlocking_;
    bool bStopping_;
    
    mutex mutexExecutors_;
    condition_variable cvExecutors_;

    list<shared_ptr<thread<communicator> > > executors_;

    shared_ptr<socket> socket_;
    shared_ptr<sender> sender_;
    
    signal2<bool, receiver &, sender & > executor_;
    
    void runExecutor();
    /// @endcond
};

} //namespace udp

} //namespace net

} //namespace bloom
