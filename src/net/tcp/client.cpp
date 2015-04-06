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

#include <bloom++/net/tcp/client.h>
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

client::client(int numExecutors, 
               size_t select_timeout_sec, 
               size_t select_timeout_usec) :
numExecutors_(numExecutors),
select_timeout_sec_(select_timeout_sec),
select_timeout_usec_(select_timeout_usec),
bBlocking_((select_timeout_sec==select_timeout_usec&&
            select_timeout_sec==0)?true:false),
bStopping_(false),
socket_(new socket)
{
    connector_.connect<client>(this, &client::scb_done);
    disconnector_.connect<client>(this, &client::scb_done);
    
    for (int i = 0; i < numExecutors_; ++i)
    {
        shared_ptr<thread<client> > thr(new thread<client>(&client::runExecutor, this));
        executors_.push_back(thr);
        thr->start();
    }
    DEBUG_INFO("Client STARTED!!!\n");
}

client::~client()
{
    bStopping_ = true;
    {
        mutex::scoped_lock sl(clientMutex_);
        cv_.notify_all();
    }
    list<shared_ptr<thread<client> > >::iterator it;
    list<shared_ptr<thread<client> > >::iterator end_it = executors_.end();

    for (it = executors_.begin(); it != end_it; ++it)
    {
        (*it)->wait();
    }
    DEBUG_INFO("Client STOPPED!!!\n");
}

signal1<void, connection& > &client::connector()
{
    return connector_;
}

signal2<bool, receiver&, connection& > &client::executor()
{
    return executor_;
}

signal1<void, connection& > &client::disconnector()
{
    return disconnector_;
}

int client::bind(const addr_ipv4& ipaddr)
{
    if(!socket_->is_closed())
        return sock_ERROR; //Socket already binded.
    
    if (socket_->bind(ipaddr))
    {
        DEBUG_ERROR("bind faild\n");
        return sock_BIND_ERROR;
    }
    return sock_OK;
}

int client::connect(const addr_ipv4& serverip)
{
    if(socket_->is_closed())
        return sock_ERROR; //Socket not binded.
    
    if (socket_->connect(serverip)){
        DEBUG_ERROR("connect faild\n");
        return sock_CONNECT_ERROR;
    };
    
    mutex::scoped_lock sl(clientMutex_);
    connection_.reset(new connection(socket_, serverip));
    connector_.emit(*connection_);
    cv_.notify_all();
    return sock_OK;
}

void client::runExecutor()
{
    DEBUG_INFO("TCP Client Executor thread started...\n");
    
    shared_ptr<connection> conn;
    receiver r;
    {
        mutex::scoped_lock sl(clientMutex_);
        conn = connection_;
        r.set_connection(conn);
    }
    
    while (true){
        {
            mutex::scoped_lock sl(clientMutex_);
            if(!connection_.get()){
                if(bStopping_){
                    conn.reset();
                    r.reset_connection();
                    break;
                }
                cv_.wait(clientMutex_);
                if(bStopping_ && !connection_.get()){
                    conn.reset();
                    r.reset_connection();
                    break;
                }
                else {
                    conn = connection_;
                    r.set_connection(conn);
                }
            }
            else if(connection_->is_closing()){
                DEBUG_INFO("Connection need stopping!\n");
                disconnector_.emit(*connection_);
                connection_.reset();
                continue;
            }
        }
        
        mutex::unique_lock ul(conn->recv_m_);
        
        if(bStopping_)conn->close();
        if(conn->is_closing())continue;
        
        if(!bBlocking_){
            if(conn->socket_->select(select_timeout_sec_, 
                       select_timeout_usec_, 
                       select_Read) != sock_SELECT_READY)
                continue;
        }
        
        r.set_locker(ul);
        
        if(!executor_.emit(r, *conn)){
            conn->close();
        }
    }
    DEBUG_INFO("TCP Client Executor thread stopped...\n");
}

} //namespace tcp

} //namespace net

} //namespace bloom
