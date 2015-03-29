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

#include <pthread.h>
#include <errno.h>

namespace bloom
{

class condition_variable;

/**
 * @brief Simple mutex class.
 */
class mutex
{
private:
    friend condition_variable;
    pthread_mutex_t mutex_;

public:

    enum mutex_type{
        normal = PTHREAD_MUTEX_NORMAL,
        recursive = PTHREAD_MUTEX_RECURSIVE,
        errorcheck = PTHREAD_MUTEX_ERRORCHECK
    };
    
    enum mutex_errors{
        r_ok = 0,
        r_busy = EBUSY,
        r_dead_lock = EDEADLK
    };
    
    /**
     * @brief Locking mutex until the object of this class exists.
     */
    class scoped_lock
    {
    public:
        friend condition_variable;
        
        scoped_lock(mutex& m) : m_(m)
        {
            m_.lock();
        }

        ~scoped_lock()
        {
            m_.unlock();
        }
        
    private:
        mutex& m_;
    };
    
    /**
     * @brief Trying to Lock mutex until the object of this class exists.
     * Mutex not be locked if who already locked in this thread...
     */
    class scoped_unilock
    {
    public:
        friend condition_variable;
        
        scoped_unilock(mutex& m) : m_(m)
        {
            try_result_ = m_.lock();
        }

        ~scoped_unilock()
        {
            if(!try_result_)
                m_.unlock();
        }
        
        void detach()
        {
            try_result_ = -1;
        }
        
        mutex& m_;
        
    private:
        int try_result_;
    };
    //mutex():mutex_(PTHREAD_MUTEX_INITIALIZER){} // -std=c++11

    mutex(mutex_type type = normal)
    {
        pthread_mutexattr_t a;
        pthread_mutexattr_init(&a);
        pthread_mutexattr_settype(&a,type);
        pthread_mutex_init(&mutex_, &a);
        pthread_mutexattr_destroy(&a);
    }

    ~mutex()
    {
        pthread_mutex_destroy(&mutex_);
    }

    int lock()
    {
        return pthread_mutex_lock(&mutex_);
    }

    int trylock()
    {
        return pthread_mutex_trylock(&mutex_);
    }

    int unlock()
    {
        return pthread_mutex_unlock(&mutex_);
    }
};

}//namespace bloom