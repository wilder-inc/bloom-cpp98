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

communicator::communicator(size_t select_timeout_ms) :
select_timeout_sec_(select_timeout_ms / 1000),
select_timeout_usec_((select_timeout_ms % 1000) * 1000),
bBlocking_(select_timeout_ms == 0 ? true : false),
bStopping_(false),
socket_(new socket),
sender_(new sender(socket_))
{
}

communicator::communicator(size_t select_timeout_sec, 
                           size_t select_timeout_usec) :
select_timeout_sec_(select_timeout_sec),
select_timeout_usec_(select_timeout_usec),
bBlocking_((select_timeout_sec==select_timeout_usec&&
            select_timeout_sec==0)?true:false),
bStopping_(false),
socket_(new socket),
sender_(new sender(socket_))
{
}

communicator::~communicator()
{
    bStopping_ = true;
    
    ::bloom::list<shared_ptr<thread<communicator> > >::iterator it;
    ::bloom::list<shared_ptr<thread<communicator> > >::iterator end_it = executors_.end();

    for (it = executors_.begin(); it != end_it; ++it)
    {
        (*it)->wait();
    }
}

void communicator::add_threads(unsigned int num)
{
    for (int i = 0; i < num; ++i)
    {
        shared_ptr<thread<communicator> > thr (new thread<communicator>(&communicator::runExecutor, this));
        executors_.push_back(thr);
        thr->start();
    }
}

unsigned int communicator::num_threads()
{
    return (unsigned int)executors_.size();
}

void communicator::bind(const addr_ipv4& ipaddr)
{
    mutex::scoped_lock slr(sender_->recv_m_);
    mutex::scoped_lock sls(sender_->send_m_);
    
    socket_->bind(ipaddr);
}

void communicator::multicast(const addr_ipv4& group, const addr_ipv4& src_iface)
{
    mutex::scoped_lock slr(sender_->recv_m_);
    mutex::scoped_lock sls(sender_->send_m_);
    
    socket_->multicast(group, src_iface);
}

void communicator::allow_broadcast()
{
    mutex::scoped_lock slr(sender_->recv_m_);
    mutex::scoped_lock sls(sender_->send_m_);
    
    socket_->allow_broadcast();
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
            }
        }
        
        mutex::unique_lock ul(s->recv_m_);

        if(bStopping_)s->close();
        if(s->is_closing())continue;

        if(!bBlocking_){
            int ret = s->socket_->select(select_timeout_sec_, 
                       select_timeout_usec_, 
                       select_Read);
            if(ret != sock_SELECT_READY){
                if(ret == sock_SELECT_ERROR)
                    DEBUG_INFO("select return ERROR!\n");
                continue;
            }
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
