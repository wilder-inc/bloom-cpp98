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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <bloom++/log.h>
#include <bloom++/stream/iostream.h>
#include <iostream>

#define BLOOM_LOG_LIST_MAX 128
#define BLOOM_LOG_READ_BUFFER 1024

#ifdef LOG_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom {

log::log():
bStopping_(false),
maxSize_(0),
minSize_(0),
logThread_(&log::logThread, this)
{
    logThread_.start();
}

log::~log()
{
    bStopping_ = true;
    logThread_.wait();
    if(file_.is_open())close();
}

bool log::open(string filename, size_t max_size)
{
    mutex::scoped_lock sl(fileMutex_);
    if(file_.is_open())return false;
    file_.open(filename.c_str(), std::ios::out | std::ios::in | std::ios::ate);
    if(!file_){
        file_.open(filename.c_str(), std::ios::out | std::ios::in | std::ios::trunc);
        if(!file_)
            return false;
    }
    maxSize_ = max_size * 1024;
    minSize_ = (size_t)(max_size * 0.75) * 1024;
    filename_ = filename;
    logThread_.start();
    return true;
}

log &log::operator <<(string str)
{
    bloom::mutex::scoped_lock sl(listMutex_);
    
    /* Using default constructor (const char *) for copy string object! (BUGFIX)
     * Else object won't be copied (class string using 
     * Copy-On-Write optimization!!!) and won't be multi-thread safe!
     */
    logs_.push_back(str.c_str());
    
    if(logs_.size()>BLOOM_LOG_LIST_MAX){
        logs_.pop_front();
    }
    cv_.notify_one();
    return *this;
}

void log::close()
{
    mutex::scoped_lock sl(fileMutex_);
    if(!file_.is_open())return;
    file_.flush();
    file_.close();
    filename_ = "";
}

void log::clear()
{
    mutex::scoped_lock sl(fileMutex_);
    if(!file_)return;
    file_.seekg(0, std::ios::beg);
    if(truncate(filename_.c_str(), 0))
        std::cerr<<"clear: truncate error...\n"; 
}

string log::pf(const char *format, ...)
{
    va_list args;
    va_start (args, format);
    char msg[256];
    vsnprintf(msg, 256, format, args);
    va_end(args);
    return msg;
}

bool log::cv_check()
{
    return (bool)logs_.size();
}

void log::logThread()
{
    while(!bStopping_){
        listMutex_.lock();
        cv_.wait<log>(listMutex_, &log::cv_check, this);
        
        string str = logs_.front();
        logs_.pop_front();
        listMutex_.unlock();

        {
            mutex::scoped_lock sl(fileMutex_);
            if(file_.is_open()){
                size_t size;
                file_<<str;
                file_.flush();
                
                size = file_.tellg();
                if(size > maxSize_){
                    deleteFirstLines(size);
                }
            }
            else {
                //DEBUG_ERROR("Error: "<<filename_<<" write failed : "<<*msg<<"\n");
                std::cout<<str;
            }
        }
       
        usleep(100);
    }
}

void log::deleteFirstLines(size_t f_size)
{
    size_t point = f_size - minSize_;
    file_.seekg(point, std::ios::beg);
    int i;
    for(i = 0; i < minSize_; ++i){
        char ch;
        file_.read(&ch, 1);
        if(!file_){
            std::cerr<<"deleteFirstLines: IOError on searching end of line...\n";
            return;
        }
        if(ch == '\n')break;
    }
    if(i == maxSize_)
        file_.seekg(point, std::ios::beg);
    
    size_t trunc_size = f_size - file_.tellg();
    size_t copyed = 0;
    char buf[BLOOM_LOG_READ_BUFFER];
    while(true){
        size_t residual = f_size - file_.tellg();
        if(residual > BLOOM_LOG_READ_BUFFER){
            file_.read(buf, BLOOM_LOG_READ_BUFFER);
            if(!file_){
                std::cerr<<"deleteFirstLines: IOError reading block...\n";
                return;
            }
            size_t pos = file_.tellg();
            file_.seekg(copyed, std::ios::beg);
            file_.write(buf, BLOOM_LOG_READ_BUFFER);
            file_.seekg(pos, std::ios::beg);
            copyed += BLOOM_LOG_READ_BUFFER;
        }
        else {
            file_.read(buf, residual);
            if(!file_){
                std::cerr<<"deleteFirstLines: IOError reading last block...\n";
                return;
            }
            file_.seekg(copyed, std::ios::beg);
            file_.write(buf, residual);
            break;
        }
    }
    
    if(truncate(filename_.c_str(), trunc_size))
        std::cerr<<"deleteFirstLines: truncate error...\n"; 
}

size_t log::queue_size()
{
    return logs_.size();
}

} //namespace bloom

