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

#include <bloom++/mutex.h>
#include <pthread.h>
#include <errno.h>

namespace bloom
{

/**
 * @brief condition_variable
 */
class condition_variable
{
public:
    condition_variable();
    ~condition_variable();
    
    void wait(mutex &m);
    void wait(mutex::scoped_lock &sl);
    void wait(mutex &m, bool(*func)());
    void wait(mutex::scoped_lock &sl, bool(*func)());
    
    template<class T>
    void wait(mutex &m, bool (T::*method)(), T *obj){
        while(!((obj->*method)()))
            pthread_cond_wait(&cv_, &m.mutex_);
    }
    
    template<class T>
    void wait(mutex::scoped_lock &sl, bool (T::*method)(), T *obj){
        while(!((obj->*method)()))
            pthread_cond_wait(&cv_, &sl.m_.mutex_);
    }
    
    bool wait(mutex &m, long timeout_ms);
    bool wait(mutex::scoped_lock &sl, long timeout_ms);
    bool wait(mutex &m, bool(*func)(), long timeout_ms);
    bool wait(mutex::scoped_lock &sl, bool(*func)(), long timeout_ms);
    
    template<class T>
    bool wait(mutex &m, bool (T::*method)(), T *obj, long timeout_ms){
        timespec ts;
        set_timeout(ts, timeout_ms);
        if(!(obj->*method)())
            if(pthread_cond_timedwait(&cv_, &m.mutex_, &ts) == ETIMEDOUT ||
                    !(obj->*method)())
                return false;
        return true;
    }
    
    template<class T>
    bool wait(mutex::scoped_lock &sl, bool (T::*method)(), T *obj, long timeout_ms){
        timespec ts;
        set_timeout(ts, timeout_ms);
        if(!(obj->*method)())
            if(pthread_cond_timedwait(&cv_, &sl.m_.mutex_, &ts) == ETIMEDOUT ||
                    !(obj->*method)())
                return false;
        return true;
    }
    
    void notify_one();
    void notify_all();
    
private:
    pthread_cond_t cv_;
    void set_timeout(timespec &ts, long ms);
};

}//namespace bloom
