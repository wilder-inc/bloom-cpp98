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

#include <bloom++/shared_ptr.h>
#include <bloom++/mutex.h>

#include "addr_ipv4.h"

namespace bloom
{

namespace net
{

namespace tcp {

class server;
class client;

} //namespace tcp

namespace udp {

class communicator;

} //namespace udp

/**
 * @brief Async/Sync data receiver for sockets.
 * 
 * Using in "executors" callback methods.
 */
template<class Tc>
class receiver_t
{
public:
    friend class udp::communicator;
    friend class tcp::server;
    friend class tcp::client;
    
    receiver_t():ul_(0){}
    receiver_t(shared_ptr<Tc> conn, mutex::unique_lock &ul):
    connection_(conn), ul_(&ul)
    {}
    
    /**
     * @brief Receive data from socket without freeing threads loop.
     * 
     * @param buffer Buffer for received data.
     * @param len Size of buffer.
     * @return Received data size or errorcode (<0).
     */
    size_t recv(char *buffer, size_t len){
        return connection_->socket_->recv(buffer, len);
    }
    
    /**
     * @brief Receive data from socket and freeing threads loop.
     * 
     * Using for async read data.
     * 
     * @param buffer Buffer for received data.
     * @param len Size of buffer.
     * @return Received data size or errorcode.
     * 
     */
    size_t recv_and_free(char *buffer, size_t len){
        size_t ret = connection_->socket_->recv(buffer, len);
        ul_->unlock();
        return ret;
    }
    
    /**
     * @brief Receive data from socket without freeing threads loop.
     * 
     * @param buffer Buffer for received data.
     * @param len Size of buffer.
     * @param from IP address of sender.
     * @return Received data size or errorcode.
     */
    size_t recvfrom(char *buffer, size_t len, addr_ipv4& from){
        return connection_->socket_->recvfrom(buffer, len, from);
    }
    
    /**
     * @brief Receive data from socket and freeing threads loop.
     * 
     * Using for async read data.
     * 
     * @param buffer Buffer for received data.
     * @param len Size of buffer.
     * @param from IP address of sender.
     * @return Received data size or errorcode.
     * 
     */
    size_t recvfrom_and_free(char *buffer, size_t len, addr_ipv4& from){
        size_t ret = connection_->socket_->recvfrom(buffer, len, from);
        ul_->unlock();
        return ret;
    }
    
    /**
     * @brief Freeing threads loop.
     */
    void free(){
        ul_->unlock();
    }
    
    bool is_closing() const {
        return connection_->is_closing();
    }
    
    const addr_ipv4 &remote_addr() const{
        return connection_->remote_addr();
    }
    
    const addr_ipv4 &socket_addr() const{
        return connection_->socket_addr();
    }
    
    unsigned int hash() const{
        return connection_->hash();
    }
    
protected:
    
    /// @cond
    void set(shared_ptr<Tc> conn, mutex::unique_lock &ul){
        connection_ = conn;
        ul_ = &ul;
    }
    
    void set_connection(shared_ptr<Tc> conn){
        connection_ = conn;
    }
    
    void reset_connection(){
        connection_.reset();
    }
    
    void set_locker(mutex::unique_lock &ul){
        ul_ = &ul;
    }
    /// @endcond
    
private:
    receiver_t(const receiver_t &); //disable default constructor
    
    /// @cond
    shared_ptr<Tc> connection_;
    mutex::unique_lock *ul_;
    /// @endcond
};

} //namespace net

} //namespace bloom