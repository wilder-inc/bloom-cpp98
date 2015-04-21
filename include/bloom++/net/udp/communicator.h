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
 * Communicator exception.
 */
class communicator_exception: public exception
{
public:
    communicator_exception(string msg):exception(msg){}
    virtual ~communicator_exception() throw() {}
};

/**
 * UDP Socket exception.
 */
/*
class bad_communicator_bind: public communicator_exception
{
public:
    bad_communicator_bind(string msg="failed!"):communicator_exception(string("communicator::bind: ")+msg){}
    virtual ~bad_communicator_bind() throw() {}
};
 */

/**
 * @brief UDP communicator.
 * This is a socket with a threads pool which emits executor signal if 
 * there is incoming datagram. Next thread will emit executor signal
 * after the current thread will free threads pool (after read_and_free or
 * free methods invoked of receiver class).
 * 
 */
class communicator
{
public:

    /**
     * Constructor.
     * @param select_timeout_ms Select timeout in milliseconds.
     */
    communicator(size_t select_timeout_ms);
    
    /**
     * Constructor.
     * Select timeout consist of seconds and nanoseconds.
     * @param select_timeout_sec Select timeout in seconds.
     * @param select_timeout_usec Select timeout in nanoseconds.
     */
    communicator(size_t select_timeout_sec, 
                 size_t select_timeout_usec);
    ~communicator();
    
    signal2<bool, receiver &, sender & > &executor();
    
    void add_threads(unsigned int num);
    unsigned int num_threads();
    
    void bind(const addr_ipv4& ipaddr) ;
#ifdef LINUX
    void multicast(const addr_ipv4 &group, const addr_ipv4 &src_iface) ;
#endif
    void allow_broadcast() ;
    
    sender& get_sender();
    
protected:
    /// @cond
    size_t select_timeout_sec_;
    size_t select_timeout_usec_;
    bool bBlocking_;
    bool bStopping_;
    
    mutex mutexExecutors_;

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
