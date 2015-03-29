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

#include <iostream>
#include <string>
#include <bloom++/stream/io.h>
#include <bloom++/shared_ptr.h>

#define BLOOM_STREAM_READ_STEP 2048

namespace bloom
{

namespace stream
{

/**
 * @brief Output stream from bloom to std::ostream.
 */
class ostream: public o_base
{
private:
    std::ostream &o_;
    
public:
    ostream(std::ostream &o):o_(o){}
    
    using o_base::operator <<;
    
    virtual ostream& operator<<(i_base &i){
        char data[BLOOM_STREAM_READ_STEP];
        size_t sz = i.read(data, BLOOM_STREAM_READ_STEP);
        if(sz){
            write(data, sz);
            if(sz == BLOOM_STREAM_READ_STEP){
                (*this)<<i;
            }
        }
        return *this;
    }
    
    virtual size_t write(const char *data, size_t size){
        o_.write(data, size);
        return size;
    }
    
    virtual bool oready(){
        return o_;
    }
};

/**
 * @brief Input stream from std::istream to bloom.
 */
class istream: public i_base
{
private:
    std::istream &i_;
    
public:
    istream(std::istream &i):i_(i){}
    
    using i_base::operator >>;
    
    virtual istream &operator>>(o_base &o){
        char data[BLOOM_STREAM_READ_STEP];
        size_t sz = read(data, BLOOM_STREAM_READ_STEP);
        if(sz){
            o.write(data, sz);
            if(sz == BLOOM_STREAM_READ_STEP){
                return (*this)>>o;
            }
        }
        return *this;
    }
    
    virtual size_t read(char *data, size_t size){
        i_.read(data, size);
        if(i_)return size;
        return i_.gcount();
    }
    
    virtual bool iready(){
        return i_.good();
    }
};

/**
 * @brief Input/Output stream between bloom++ stream and std stream.
 */
class iostream: public io_base
{
private:
    std::iostream &io_;
    
public:
    iostream(std::iostream &io): io_(io){}
    
    using i_base::operator >>;
    
    virtual iostream &operator>>(o_base &o){
        char data[BLOOM_STREAM_READ_STEP];
        size_t sz = read(data, BLOOM_STREAM_READ_STEP);
        if(sz){
            o.write(data, sz);
            if(sz == BLOOM_STREAM_READ_STEP){
                return (*this)>>o;
            }
        }
        return *this;
    }
    
    virtual size_t read(char *data, size_t size){
        io_.read(data, size);
        if(io_)return size;
        return io_.gcount();
    }
    
    virtual bool iready(){
        return io_;
    }
    
    using o_base::operator <<;
    
    virtual iostream& operator<<(i_base &i){
        char data[BLOOM_STREAM_READ_STEP];
        size_t sz = i.read(data, BLOOM_STREAM_READ_STEP);
        if(sz){
            write(data, sz);
            if(sz == BLOOM_STREAM_READ_STEP){
                (*this)<<i;
            }
        }
        return *this;
    }
    
    virtual size_t write(const char *data, size_t size){
        io_.write(data, size);
        return size;
    }
    
    virtual bool oready(){
        return io_;
    }
};

} //namespace stream

/**
 * @brief Output bloom input streams to std::ostream stream.
 * 
 * @param o output stream
 * @param ib bloom++ stream
 * @return o
 */
std::ostream &operator<<(std::ostream &o, bloom::stream::i_base &ib);

/**
 * @brief Input from std::istream to bloom.
 * 
 * @param i output stream
 * @param ob bloom++ stream
 * @return true if success.
 */
std::istream &operator>>(std::istream &i, bloom::stream::o_base &ob);

} //namespace bloom
