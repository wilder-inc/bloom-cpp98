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

#include <bloom++/net/udp/communicator.h>
#ifdef LINUX
#include <sys/time.h>
#else
#include <time.h>
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

communicator::communicator(int numExecutors,
                           size_t select_timeout_sec, 
                           size_t select_timeout_usec) :
numExecutors_(numExecutors),
select_timeout_sec_(select_timeout_sec),
select_timeout_usec_(select_timeout_usec),
bBlocking_((select_timeout_sec==select_timeout_usec&&
            select_timeout_sec==0)?true:false),
bStopping_(false),
socket_(new socket),
sender_(new sender(socket_))
{
    for (int i = 0; i < numExecutors_; ++i)
    {
        shared_ptr<thread<communicator> > thr (new thread<communicator>(&communicator::runExecutor, this));
        executors_.push_back(thr);
        thr->start();
    }
}

communicator::~communicator()
{
    bStopping_ = true;
    {
        mutex::scoped_lock sl(mutexExecutors_);
        cvExecutors_.notify_all();
    }
    ::bloom::list<shared_ptr<thread<communicator> > >::iterator it;
    ::bloom::list<shared_ptr<thread<communicator> > >::iterator end_it = executors_.end();

    for (it = executors_.begin(); it != end_it; ++it)
    {
        (*it)->wait();
    }
}

int communicator::bind(const addr_ipv4& ipaddr)
{
    mutex::scoped_lock sl(mutexExecutors_);
    if(!socket_->is_closed())
        return sock_ERROR; //Socket already binded.
    
    if(socket_->bind(ipaddr)){
        DEBUG_WARN("bind faild\n");
        return sock_BIND_ERROR;
    }
    cvExecutors_.notify_all();
    return sock_OK;
}

int communicator::multicast(const addr_ipv4& group, const addr_ipv4& src_iface)
{
    mutex::scoped_lock sl(mutexExecutors_);
    if(!socket_->is_closed())
        return sock_ERROR; //Socket already binded.
    
    if(socket_->multicast(group, src_iface)){
        DEBUG_WARN("multicast faild\n");
        return sock_ERROR;
    }
    cvExecutors_.notify_all();
    return sock_OK;
}

int communicator::allow_broadcast()
{
    mutex::scoped_lock sl(mutexExecutors_);
    if(!socket_->is_closed()){
        mutex::scoped_lock sl1(sender_->recv_m_);
        mutex::scoped_lock sl2(sender_->send_m_);
        return socket_->allow_broadcast();
    }
    return sock_ERROR;
}

sender& communicator::get_sender()
{
    return *sender_;
}

::bloom::signal2<bool, receiver &, sender & > &communicator::executor()
{
    return executor_;
}

void communicator::runExecutor()
{
    DEBUG_INFO("Executor thread started...\n");
    
    shared_ptr<sender> s;
    receiver r;
    {
        mutex::scoped_lock sl(mutexExecutors_);
        r.set_connection(sender_);
        s = sender_;
    }
    
    while (!bStopping_)
    {
        {
            mutex::scoped_lock sl(mutexExecutors_);
            if(sender_->is_closing()){
                if(bStopping_){
                    r.reset_connection();
                    s.reset();
                    break;
                }
                cvExecutors_.wait(mutexExecutors_);
                if(bStopping_ && sender_->is_closing()){
                    r.reset_connection();
                    s.reset();
                    break;
                }
                else {
                    s = sender_;
                    r.set_connection(s);
                }
            }
        }
        
        mutex::unique_lock ul(s->recv_m_);
        
        if(bStopping_)s->close();
        if(s->is_closing())continue;

        if(!bBlocking_){
            if(s->socket_->select(select_timeout_sec_, 
                       select_timeout_usec_, 
                       select_Read) != sock_SELECT_READY)
                continue;
        }
        
        r.set_locker(ul);
        
        if(!executor_.emit(r, *s)){
            s->close();
        }
    }
    DEBUG_INFO("Executor thread stopped...\n");
}

/* not need now
long communicator::get_hash_value(addr_ipv4* addr)
{
    long val = ((long)addr->ip_int())<<16;
    val |= addr->port();
    //DEBUG_INFO("hash_val: %d, addr_ipv4: %s:%d\n", val, addr->ip().c_str(), addr->port());
    return val;
}
 */

} //namespace udp

} //namespace net

} //namespace bloom
