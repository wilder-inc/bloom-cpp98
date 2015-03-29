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

#include <time.h>
#include <bloom++/condition_variable.h>

#include "bloom++/mutex.h"

namespace bloom
{

condition_variable::condition_variable()
{
    pthread_cond_init(&cv_, NULL);
}

condition_variable::~condition_variable()
{
    pthread_cond_destroy(&cv_); 
}

void condition_variable::wait(mutex& m, bool(*func)())
{
    while(!(*func)())
        pthread_cond_wait(&cv_, &m.mutex_);   
}

void condition_variable::wait(mutex& m)
{
    pthread_cond_wait(&cv_, &m.mutex_);   
}

void condition_variable::wait(mutex::scoped_lock& sl, bool(*func)())
{
    while(!(*func)())
        pthread_cond_wait(&cv_, &sl.m_.mutex_);   
}

void condition_variable::wait(mutex::scoped_lock& sl)
{
    pthread_cond_wait(&cv_, &sl.m_.mutex_);
}

bool condition_variable::wait(mutex& m, bool(*func)(), long timeout_ms)
{
    timespec ts;
    set_timeout(ts, timeout_ms);
    if(!(*func)())
        if(pthread_cond_timedwait(&cv_, &m.mutex_, &ts) == ETIMEDOUT ||
                !(*func)())
            return false;
    return true;  
}

bool condition_variable::wait(mutex& m, long timeout_ms)
{
    timespec ts;
    set_timeout(ts, timeout_ms);
    if(pthread_cond_timedwait(&cv_, &m.mutex_, &ts) == ETIMEDOUT)
        return false;
    return true;
}

bool condition_variable::wait(mutex::scoped_lock& sl, bool(*func)(), long timeout_ms)
{
    timespec ts;
    set_timeout(ts, timeout_ms);
    if(!(*func)())
        if(pthread_cond_timedwait(&cv_, &sl.m_.mutex_, &ts) == ETIMEDOUT ||
                !(*func)())
            return false;
    return true;
}

bool condition_variable::wait(mutex::scoped_lock& sl, long timeout_ms)
{
    timespec ts;
    set_timeout(ts, timeout_ms);
    if(pthread_cond_timedwait(&cv_, &sl.m_.mutex_, &ts) == ETIMEDOUT)
        return false;
    return true;
}

void condition_variable::notify_one()
{
    pthread_cond_signal(&cv_);
}

void condition_variable::notify_all()
{
    pthread_cond_broadcast(&cv_);
}

void condition_variable::set_timeout(timespec& ts, long ms)
{
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += ms / 1000;
    ts.tv_nsec += (ms % 1000) * 1000000;
}

}//namespace bloom
