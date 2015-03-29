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
#include <unistd.h>

namespace bloom
{

struct NoClass
{

    NoClass()
    {
    }

    ~NoClass()
    {
    }
};

/**
 * @brief Thread using.
 */
template<typename T, typename P1 = NoClass, typename P2 = NoClass, typename P3 = NoClass,
typename P4 = NoClass, typename P5 = NoClass, typename P6 = NoClass, typename P7 = NoClass>
class thread
{
public:
    typedef void (T::*threadFnPtr)();
    typedef void (T::*threadFnPtr1)(P1);
    typedef void (T::*threadFnPtr2)(P1, P2);
    typedef void (T::*threadFnPtr3)(P1, P2, P3);
    typedef void (T::*threadFnPtr4)(P1, P2, P3, P4);
    typedef void (T::*threadFnPtr5)(P1, P2, P3, P4, P5);
    typedef void (T::*threadFnPtr6)(P1, P2, P3, P4, P5, P6);
    typedef void (T::*threadFnPtr7)(P1, P2, P3, P4, P5, P6, P7);

private:
    pthread_t thread_;
    threadFnPtr func_;
    threadFnPtr1 func1_;
    threadFnPtr2 func2_;
    threadFnPtr3 func3_;
    threadFnPtr4 func4_;
    threadFnPtr5 func5_;
    threadFnPtr6 func6_;
    threadFnPtr7 func7_;
    T * obj_;
    bool bDestroy_;
    P1 p1_;
    P2 p2_;
    P3 p3_;
    P4 p4_;
    P5 p5_;
    P6 p6_;
    P7 p7_;

    thread(const thread& copy); // copy constructor denied

    static void *thread_func(void *d)
    {
        ((thread *) d)->run();
        pthread_exit(0);
    }

    void run()
    {
        if (func_)(obj_->*func_)();
        else if (func1_)(obj_->*func1_)(p1_);
        else if (func2_)(obj_->*func2_)(p1_, p2_);
        else if (func3_)(obj_->*func3_)(p1_, p2_, p3_);
        else if (func4_)(obj_->*func4_)(p1_, p2_, p3_, p4_);
        else if (func5_)(obj_->*func5_)(p1_, p2_, p3_, p4_, p5_);
        else if (func6_)(obj_->*func6_)(p1_, p2_, p3_, p4_, p5_, p6_);
        else if (func7_)(obj_->*func7_)(p1_, p2_, p3_, p4_, p5_, p6_, p7_);
        if (bDestroy_)delete this;
    }

public:

    thread(threadFnPtr fn, T *obj) :
    func_(fn), func1_(0), func2_(0), func3_(0), func4_(0), func5_(0), func6_(0), func7_(0), obj_(obj), bDestroy_(false)
    {
    }

    thread(threadFnPtr1 fn, T *obj) : func_(0),
    func1_(fn), func2_(0), func3_(0), func4_(0), func5_(0), func6_(0), func7_(0), obj_(obj), bDestroy_(false)
    {
    }

    thread(threadFnPtr2 fn, T *obj) : func_(0), func1_(0),
    func2_(fn), func3_(0), func4_(0), func5_(0), func6_(0), func7_(0), obj_(obj), bDestroy_(false)
    {
    }

    thread(threadFnPtr3 fn, T *obj) : func_(0), func1_(0), func2_(0),
    func3_(fn), func4_(0), func5_(0), func6_(0), func7_(0), obj_(obj), bDestroy_(false)
    {
    }

    thread(threadFnPtr4 fn, T *obj) : func_(0), func1_(0), func2_(0), func3_(0),
    func4_(fn), func5_(0), func6_(0), func7_(0), obj_(obj), bDestroy_(false)
    {
    }

    thread(threadFnPtr5 fn, T *obj) : func_(0), func1_(0), func2_(0), func3_(0), func4_(0),
    func5_(fn), func6_(0), func7_(0), obj_(obj), bDestroy_(false)
    {
    }

    thread(threadFnPtr6 fn, T *obj) : func_(0), func1_(0), func2_(0), func3_(0), func4_(0), func5_(0),
    func6_(fn), func7_(0), obj_(obj), bDestroy_(false)
    {
    }

    thread(threadFnPtr7 fn, T *obj) : func_(0), func1_(0), func2_(0), func3_(0), func4_(0), func5_(0), func6_(0),
    func7_(fn), obj_(obj), bDestroy_(false)
    {
    }

    ~thread()
    {
    }

    void destroyAfter()
    {
        bDestroy_ = true;
    }

    int start()
    {
        if(!func_){
            return -1;
        }
        return pthread_create(&thread_, NULL,
                              thread::thread_func, (void*) this);
    }
    
    int start(P1 p1)
    {
        if(!func1_){
            return -1;
        }
        p1_ = p1;
        return pthread_create(&thread_, NULL,
                              thread::thread_func, (void*) this);
    }
    
    int start(P1 p1, P2 p2)
    {
        if(!func2_){
            return -1;
        }
        p1_ = p1;
        p2_ = p2;
        return pthread_create(&thread_, NULL,
                              thread::thread_func, (void*) this);
    }
    
    int start(P1 p1, P2 p2, P3 p3)
    {
        if(!func3_){
            return -1;
        }
        p1_ = p1;
        p2_ = p2;
        p3_ = p3;
        return pthread_create(&thread_, NULL,
                              thread::thread_func, (void*) this);
    }
    
    int start(P1 p1, P2 p2, P3 p3, P4 p4)
    {
        if(!func4_){
            return -1;
        }
        p1_ = p1;
        p2_ = p2;
        p3_ = p3;
        p4_ = p4;
        return pthread_create(&thread_, NULL,
                              thread::thread_func, (void*) this);
    }
    
    int start(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
    {
        if(!func5_){
            return -1;
        }
        p1_ = p1;
        p2_ = p2;
        p3_ = p3;
        p4_ = p4;
        p5_ = p5;
        return pthread_create(&thread_, NULL,
                              thread::thread_func, (void*) this);
    }
    
    int start(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
    {
        if(!func6_){
            return -1;
        }
        p1_ = p1;
        p2_ = p2;
        p3_ = p3;
        p4_ = p4;
        p5_ = p5;
        p6_ = p6;
        return pthread_create(&thread_, NULL,
                              thread::thread_func, (void*) this);
    }
    
    int start(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
    {
        if(!func7_){
            return -1;
        }
        p1_ = p1;
        p2_ = p2;
        p3_ = p3;
        p4_ = p4;
        p5_ = p5;
        p6_ = p6;
        p7_ = p7;
        return pthread_create(&thread_, NULL,
                              thread::thread_func, (void*) this);
    }

    int wait()
    {
        return pthread_join(thread_, NULL);
    }

    static void start(threadFnPtr fn, T *obj)
    {
        thread *t = new thread(fn, obj);
        t->destroyAfter();
        t->start();
    }

    static void start(threadFnPtr1 fn, T *obj, P1 p1)
    {
        thread *t = new thread(fn, obj);
        t->destroyAfter();
        t->start(p1);
    }

    static void start(threadFnPtr2 fn, T *obj, P1 p1, P2 p2)
    {
        thread *t = new thread(fn, obj);
        t->destroyAfter();
        t->start(p1, p2);
    }

    static void start(threadFnPtr3 fn, T *obj, P1 p1, P2 p2, P3 p3)
    {
        thread *t = new thread(fn, obj);
        t->destroyAfter();
        t->start(p1, p2, p3);
    }

    static void start(threadFnPtr4 fn, T *obj, P1 p1, P2 p2, P3 p3, P4 p4)
    {
        thread *t = new thread(fn, obj);
        t->destroyAfter();
        t->start(p1, p2, p3, p4);
    }

    static void start(threadFnPtr5 fn, T *obj, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
    {
        thread *t = new thread(fn, obj);
        t->destroyAfter();
        t->start(p1, p2, p3, p4, p5);
    }

    static void start(threadFnPtr6 fn, T *obj, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
    {
        thread *t = new thread(fn, obj);
        t->destroyAfter();
        t->start(p1, p2, p3, p4, p5, p6);
    }

    static void start(threadFnPtr7 fn, T *obj, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
    {
        thread *t = new thread(fn, obj);
        t->destroyAfter();
        t->start(p1, p2, p3, p4, p5, p6, p7);
    }
};



} //namespace bloom
