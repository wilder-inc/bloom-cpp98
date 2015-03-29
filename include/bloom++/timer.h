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

#include <pthread.h>
#include <unistd.h>
#include <bloom++/time.h>

namespace bloom
{


/**
 * @brief Timer in milliseconds.
 */
template<class T>
class timer
{
public:
    typedef void (T::*timerFnPtr)(void);

private:
    pthread_t thread_;
    timerFnPtr func_;
    T * obj_;
    long interval_;
    long last_invoked_;
    bool bRunning_;
    bool bStopping_;

    static void *thread_func(void *d)
    {
        ((timer *) d)->timer_loop();
        pthread_exit(0);
    }

    void timer_loop()
    {
        last_invoked_ = get_milli_sec();
        (obj_->*func_)();
        while (!bStopping_)
        {
            long current_timer = get_milli_sec();
            if (current_timer - last_invoked_ >= interval_)
            {
                (obj_->*func_)();
                last_invoked_ = current_timer;
            }
            usleep(1000);
        }
        bRunning_ = false;
        bStopping_ = false;
    }

public:

    timer(timerFnPtr func, T *obj, time_t interval) : func_(func), obj_(obj), interval_(interval), bRunning_(false), bStopping_(false)
    {
    }

    int start()
    {
        if(bStopping_ || bRunning_)return -1;
        bRunning_ = true;
        bStopping_ = false;
        return pthread_create(&thread_, NULL, timer::thread_func, (void*) this);
    }

    int wait()
    {
        return pthread_join(thread_, NULL);
    }

    void stop()
    {
        if(bStopping_ || !bRunning_)return;
        bStopping_ = true;
        wait();
    }

    bool isRunning()
    {
        return bRunning_;
    }
};

} //namespace bloom