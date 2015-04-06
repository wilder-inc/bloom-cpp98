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

#include <bloom++/net/tcp/server.h>

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

server::server(int numAcceptors,
               int numExecutors,
               size_t select_timeout_sec, 
               size_t select_timeout_usec) :
numAcceptors_(numAcceptors),
numExecutors_(numExecutors),
select_timeout_sec_(select_timeout_sec),
select_timeout_usec_(select_timeout_usec),
bBlocking_((select_timeout_sec==select_timeout_usec&&
            select_timeout_sec==0)?true:false),
bStopping_(false),
socket_(new socket)
{
    acceptor_.connect<server>(this, &server::scb_allow);
    disconnector_.connect<server>(this, &server::scb_done);
    
    for (int i = 0; i < numAcceptors_; ++i)
    {
        shared_ptr<thread<server> > thr(new thread<server>(&server::runAcceptor, this));
        acceptors_.push_back(thr);
        thr->start();
    }

    for (int i = 0; i < numExecutors_; ++i)
    {
        shared_ptr<thread<server> > thr(new thread<server>(&server::runExecutor, this));
        executors_.push_back(thr);
        thr->start();
    }
    DEBUG_INFO("Server STARTED!!!\n");
}

server::~server()
{
    bStopping_ = true;
    {
        mutex::scoped_lock sl(mutexAcceptors_);
        cvAcceptors_.notify_all();
    }
    {
        mutex::scoped_lock sl(mutexConnections_);
        cvConnections_.notify_all();
    }
    ::bloom::list<shared_ptr<thread<server> > >::iterator it;
    ::bloom::list<shared_ptr<thread<server> > >::iterator end_it = acceptors_.end();
    for (it = acceptors_.begin(); it != end_it; ++it)
    {
        (*it)->wait();
    }
    end_it = executors_.end();
    for (it = executors_.begin(); it != end_it; ++it)
    {
        DEBUG_INFO("executors wait...\n");
        (*it)->wait();
    }
    DEBUG_INFO("Server STOPPED!!!\n");
}

::bloom::signal1<bool, connection& > &server::acceptor()
{
    return acceptor_;
}

::bloom::signal2<bool, receiver&, connection& > &server::executor()
{
    return executor_;
}

::bloom::signal1<void, connection& > &server::disconnector()
{
    return disconnector_;
}

void server::deny_acceptor()
{
    acceptor_.connect<server>(this, &server::scb_deny);
}

void server::allow_acceptor()
{
    acceptor_.connect<server>(this, &server::scb_allow);
}

int server::bind(const addr_ipv4& ipaddr)
{
    if(!socket_->is_closed())
        return sock_ERROR; //Socket already binded.
    
    if (socket_->bind(ipaddr))
    {
        DEBUG_ERROR("bind faild\n");
        return sock_BIND_ERROR;
    }
    
    mutex::scoped_lock sl(mutexAcceptors_);
    cvAcceptors_.notify_all();
    return sock_OK;
}

void server::runAcceptor()
{
    DEBUG_INFO("Acceptor thread started...\n");
    shared_ptr<connection> conn;
    int ret;
    bool bAdding;
    
    while (!bStopping_)
    {
        {
            mutex::scoped_lock sl(mutexAcceptors_);
            if(socket_->is_closed())
                cvAcceptors_.wait(mutexAcceptors_);

            if(bStopping_)
                break;

            do
            {
                ret = socket_->select(select_timeout_sec_,
                                      select_timeout_usec_, 
                                      select_Read);
                if (ret == sock_SELECT_READY)
                {
                    ret = socket_->listen();
                    if (ret == sock_OK)
                    {
                        ret = socket_->select(select_timeout_sec_, 
                                              select_timeout_usec_, 
                                              select_Read);
                        if (ret == sock_SELECT_READY)
                        {
                            addr_ipv4 remote;
                            int fd = socket_->accept(remote);
                            if(fd != sock_ACCEPT_ERROR){
                                shared_ptr<socket> conn_sock(
                                        new socket(fd, socket_->socket_addr()));
                                conn.reset(new connection(conn_sock, remote));
                                ret = sock_OK;
                            }
                        }
                    }
                }
            }
            while ( ret != sock_OK && !bStopping_ );
        }
        
        if(bStopping_)
            break;
        
        if (ret == sock_OK)
        {
            bAdding = acceptor_.emit(*conn);
        }
        else 
            continue;
        
        if(bAdding){
            mutex::scoped_lock sl(mutexConnections_);
            connections_.push_back(conn);
            conn.reset(); // For thread safe.
            cvConnections_.notify_all();
        }
    }
    DEBUG_INFO("Acceptor thread stopped...\n");
}

void server::runExecutor()
{
    DEBUG_INFO("Executor thread started...\n");
    shared_ptr<connection> conn;
    receiver r;
    
    while (true)
    {   
        {
            mutex::scoped_lock sl(mutexConnections_);

            if(!connections_.size()){
                if(bStopping_){
                    conn.reset(); //safe release
                    r.reset_connection(); //safe release
                    break;
                }
                cvConnections_.wait(mutexConnections_);
                if(bStopping_ && !connections_.size()){
                    conn.reset(); //safe release
                    r.reset_connection(); //safe release
                    break;
                }
            }

            conn = connections_.front();
            connections_.pop_front();

            if(bStopping_)conn->close();
            
            if(conn->is_closing()){
                DEBUG_INFO("Connection destroing!\n");
                disconnector_.emit(*conn);
                continue;
            }

            connections_.push_back(conn);

            r.set_connection(conn);
        }
        
        // Current connection locking.
        mutex::unique_lock ul2(conn->recv_m_);
        
        if(conn->is_closing())
            continue;

        bool bExec  = true;
        if(!bBlocking_){
            if(conn->socket_->select(select_timeout_sec_, 
                       select_timeout_usec_, 
                       select_Read) != sock_SELECT_READY){
                bExec = false;
            }
        }

        if(bExec){
            r.set_locker(ul2);
            
            if(!executor_.emit(r, *conn))
                conn->close();
        }
    }
    DEBUG_INFO("Executor thread stopped...\n");
}

} //namespace tcp

} //namespace net

} //namespace bloom
