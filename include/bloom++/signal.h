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

namespace bloom
{

template <class T_Ret>
class slot_base0
{
public:
    virtual ~slot_base0(){}
    virtual T_Ret emit() = 0;
};

template<class T_Ret, class T_Par1>
class slot_base1
{
public:
    virtual ~slot_base1(){}
    virtual T_Ret emit(T_Par1 par1) = 0;
};

template<class T_Ret, class T_Par1, class T_Par2>
class slot_base2
{
public:
    virtual ~slot_base2(){}
    virtual T_Ret emit(T_Par1 par1, T_Par2 par2) = 0;
};

template<class T_Ret, class T_Par1, class T_Par2, class T_Par3>
class slot_base3
{
public:
    virtual ~slot_base3(){}
    virtual T_Ret emit(T_Par1 par1, T_Par2 par2, T_Par3 par3) = 0;
};

template<class T_Ret, class T_Par1, class T_Par2, class T_Par3, class T_Par4>
class slot_base4
{
public:
    virtual ~slot_base4(){}
    virtual T_Ret emit(T_Par1 par1, T_Par2 par2, T_Par3 par3, T_Par4 par4) = 0;
};

template<class T_Ret, class T_Par1, class T_Par2, class T_Par3, class T_Par4, class T_Par5>
class slot_base5
{
public:
    virtual ~slot_base5(){}
    virtual T_Ret emit(T_Par1 par1, T_Par2 par2, T_Par3 par3, T_Par4 par4, T_Par5 par5) = 0;
};

template<class T_Ret, class T_Par1, class T_Par2, class T_Par3, class T_Par4, class T_Par5, class T_Par6>
class slot_base6
{
public:
    virtual ~slot_base6(){}
    virtual T_Ret emit(T_Par1 par1, T_Par2 par2, T_Par3 par3, T_Par4 par4, T_Par5 par5, T_Par6 par6) = 0;
};

template<class T_Ret, class T_Obj>
class mem_slot0: public slot_base0<T_Ret>
{
public:
    typedef T_Ret (T_Obj::*mem_fn)();
    mem_slot0(T_Obj *obj, mem_fn fn):obj_(obj),fn_(fn){}
    virtual T_Ret emit(){
        return (obj_->*fn_)();
    }
    
protected:
    /// @cond
    T_Obj *obj_;
    mem_fn fn_;
    /// @endcond
};

template<class T_Ret, class T_Obj, class T_Par1>
class mem_slot1: public slot_base1<T_Ret, T_Par1>
{
public:
    typedef T_Ret (T_Obj::*mem_fn)(T_Par1);
    mem_slot1(T_Obj *obj, mem_fn fn):obj_(obj),fn_(fn){}
    virtual T_Ret emit(T_Par1 par1){
        return (obj_->*fn_)(par1);
    }
    
protected:
    /// @cond
    T_Obj *obj_;
    mem_fn fn_;
    /// @endcond
};

template<class T_Ret, class T_Obj, class T_Par1, class T_Par2>
class mem_slot2: public slot_base2<T_Ret, T_Par1, T_Par2>
{
public:
    typedef T_Ret (T_Obj::*mem_fn)(T_Par1, T_Par2);
    mem_slot2(T_Obj *obj, mem_fn fn):obj_(obj),fn_(fn){}
    virtual T_Ret emit(T_Par1 par1, T_Par2 par2){
        return (obj_->*fn_)(par1, par2);
    }
    
protected:
    /// @cond
    T_Obj *obj_;
    mem_fn fn_;
    /// @endcond
};

template<class T_Ret, class T_Obj, class T_Par1, class T_Par2, class T_Par3>
class mem_slot3: public slot_base3<T_Ret, T_Par1, T_Par2, T_Par3>
{
public:
    typedef T_Ret (T_Obj::*mem_fn)(T_Par1, T_Par2, T_Par3);
    mem_slot3(T_Obj *obj, mem_fn fn):obj_(obj),fn_(fn){}
    virtual T_Ret emit(T_Par1 par1, T_Par2 par2, T_Par3 par3){
        return (obj_->*fn_)(par1, par2, par3);
    }
    
protected:
    /// @cond
    T_Obj *obj_;
    mem_fn fn_;
    /// @endcond
};

template<class T_Ret, class T_Obj, class T_Par1, class T_Par2, class T_Par3, class T_Par4>
class mem_slot4: public slot_base4<T_Ret, T_Par1, T_Par2, T_Par3, T_Par4>
{
public:
    typedef T_Ret (T_Obj::*mem_fn)(T_Par1, T_Par2, T_Par3, T_Par4);
    mem_slot4(T_Obj *obj, mem_fn fn):obj_(obj),fn_(fn){}
    virtual T_Ret emit(T_Par1 par1, T_Par2 par2, T_Par3 par3, T_Par4 par4){
        return (obj_->*fn_)(par1, par2, par3, par4);
    }
    
protected:
    /// @cond
    T_Obj *obj_;
    mem_fn fn_;
    /// @endcond
};

template<class T_Ret, class T_Obj, class T_Par1, class T_Par2, class T_Par3, class T_Par4, class T_Par5>
class mem_slot5: public slot_base5<T_Ret, T_Par1, T_Par2, T_Par3, T_Par4, T_Par5>
{
public:
    typedef T_Ret (T_Obj::*mem_fn)(T_Par1, T_Par2, T_Par3, T_Par4, T_Par5);
    mem_slot5(T_Obj *obj, mem_fn fn):obj_(obj),fn_(fn){}
    virtual T_Ret emit(T_Par1 par1, T_Par2 par2, T_Par3 par3, T_Par4 par4, T_Par5 par5){
        return (obj_->*fn_)(par1, par2, par3, par4, par5);
    }
    
protected:
    /// @cond
    T_Obj *obj_;
    mem_fn fn_;
    /// @endcond
};

template<class T_Ret, class T_Obj, class T_Par1, class T_Par2, class T_Par3, class T_Par4, class T_Par5, class T_Par6>
class mem_slot6: public slot_base6<T_Ret, T_Par1, T_Par2, T_Par3, T_Par4, T_Par5, T_Par6>
{
public:
    typedef T_Ret (T_Obj::*mem_fn)(T_Par1, T_Par2, T_Par3, T_Par4, T_Par5, T_Par6);
    mem_slot6(T_Obj *obj, mem_fn fn):obj_(obj),fn_(fn){}
    virtual T_Ret emit(T_Par1 par1, T_Par2 par2, T_Par3 par3, T_Par4 par4, T_Par5 par5, T_Par6 par6){
        return (obj_->*fn_)(par1, par2, par3, par4, par5, par6);
    }
    
protected:
    /// @cond
    T_Obj *obj_;
    mem_fn fn_;
    ///@endcond
};

template<class T_Ret, class T_Obj>
inline shared_ptr<slot_base0<T_Ret> >
mem_fun(T_Obj *obj, T_Ret (T_Obj::*fn)()){
    return shared_ptr<slot_base0<T_Ret> >(new mem_slot0<T_Ret, T_Obj>(obj, fn));
}

template<class T_Ret, class T_Obj, class T_Par1>
inline shared_ptr<slot_base1<T_Ret, T_Par1> >
mem_fun(T_Obj *obj, T_Ret (T_Obj::*fn)(T_Par1)){
    return shared_ptr<slot_base1<T_Ret, T_Par1> >(new mem_slot1<T_Ret, T_Obj, T_Par1>(obj, fn));
}

template<class T_Ret, class T_Obj, class T_Par1, class T_Par2>
inline shared_ptr<slot_base2<T_Ret, T_Par1, T_Par2> >
mem_fun(T_Obj *obj, T_Ret (T_Obj::*fn)(T_Par1, T_Par2)){
    return shared_ptr<slot_base2<T_Ret, T_Par1, T_Par2> >(new mem_slot2<T_Ret, T_Obj, T_Par1, T_Par2>(obj, fn));
}

template<class T_Ret, class T_Obj, class T_Par1, class T_Par2, class T_Par3>
inline shared_ptr<slot_base3<T_Ret, T_Par1, T_Par2, T_Par3> >
mem_fun(T_Obj *obj, T_Ret (T_Obj::*fn)(T_Par1, T_Par2, T_Par3)){
    return shared_ptr<slot_base3<T_Ret, T_Par1, T_Par2, T_Par3> >(new mem_slot3<T_Ret, T_Obj, T_Par1, T_Par2, T_Par3>(obj, fn));
}

template<class T_Ret, class T_Obj, class T_Par1, class T_Par2, class T_Par3, class T_Par4>
inline shared_ptr<slot_base4<T_Ret, T_Par1, T_Par2, T_Par3, T_Par4> >
mem_fun(T_Obj *obj, T_Ret (T_Obj::*fn)(T_Par1, T_Par2, T_Par3, T_Par4)){
    return shared_ptr<slot_base4<T_Ret, T_Par1, T_Par2, T_Par3, T_Par4> >(new mem_slot4<T_Ret, T_Obj, T_Par1, T_Par2, T_Par3, T_Par4>(obj, fn));
}

template<class T_Ret, class T_Obj, class T_Par1, class T_Par2, class T_Par3, class T_Par4, class T_Par5>
inline shared_ptr<slot_base5<T_Ret, T_Par1, T_Par2, T_Par3, T_Par4, T_Par5> >
mem_fun(T_Obj *obj, T_Ret (T_Obj::*fn)(T_Par1, T_Par2, T_Par3, T_Par4, T_Par5)){
    return shared_ptr<slot_base5<T_Ret, T_Par1, T_Par2, T_Par3, T_Par4, T_Par5> >(new mem_slot5<T_Ret, T_Obj, T_Par1, T_Par2, T_Par3, T_Par4, T_Par5>(obj, fn));
}

template<class T_Ret, class T_Obj, class T_Par1, class T_Par2, class T_Par3, class T_Par4, class T_Par5, class T_Par6>
inline shared_ptr<slot_base6<T_Ret, T_Par1, T_Par2, T_Par3, T_Par4, T_Par5, T_Par6> >
mem_fun(T_Obj *obj, T_Ret (T_Obj::*fn)(T_Par1, T_Par2, T_Par3, T_Par4, T_Par5, T_Par6)){
    return shared_ptr<slot_base6<T_Ret, T_Par1, T_Par2, T_Par3, T_Par4, T_Par5, T_Par6> >(new mem_slot6<T_Ret, T_Obj, T_Par1, T_Par2, T_Par3, T_Par4, T_Par5, T_Par6>(obj, fn));
}

/**
 * @brief Signal without parameters.
 */
template<class T_Ret>
class signal0
{
public:
    typedef slot_base0<T_Ret>  slot_type;
    void connect(shared_ptr<slot_type> slot){
        slot_ = slot;
    }
    template<class T_Obj>
    void connect(T_Obj *obj, T_Ret (T_Obj::*fn)())
    {
        slot_ = new mem_slot0<T_Ret, T_Obj>(obj, fn);
    }
    T_Ret emit(){
        return slot_->emit();
    }
protected:
    /// @cond
    shared_ptr<slot_type> slot_;
    /// @endcond
};

/**
 * @brief Signal with 1 parameter.
 */
template<class T_Ret, class T_Par1>
class signal1
{
public:
    typedef slot_base1<T_Ret, T_Par1> slot_type;
    void connect(shared_ptr<slot_type> slot){
        slot_ = slot;
    }
    template<class T_Obj>
    void connect(T_Obj *obj, T_Ret (T_Obj::*fn)(T_Par1))
    {
        slot_ = shared_ptr<slot_type> (new mem_slot1<T_Ret, T_Obj, T_Par1>(obj, fn));
    }
    T_Ret emit(T_Par1 par1){
        return slot_->emit(par1);
    }
protected:
    /// @cond
    shared_ptr<slot_type> slot_;
    /// @endcond
};

/**
 * @brief Signal with 2 parameters.
 */
template<class T_Ret, class T_Par1, class T_Par2>
class signal2
{
public:
    typedef slot_base2<T_Ret, T_Par1, T_Par2> slot_type;
    void connect(shared_ptr<slot_type> slot){
        slot_ = slot;
    }
    template<class T_Obj>
    void connect(T_Obj *obj, T_Ret (T_Obj::*fn)(T_Par1, T_Par2))
    {
        slot_ = shared_ptr<slot_type> (new mem_slot2<T_Ret, T_Obj, T_Par1, T_Par2>(obj, fn));
    }
    T_Ret emit(T_Par1 par1, T_Par2 par2){
        return slot_->emit(par1, par2);
    }
protected:
    /// @cond
    shared_ptr<slot_type> slot_;
    /// @endcond
};

/**
 * @brief Signal with 3 parameters.
 */
template<class T_Ret, class T_Par1, class T_Par2, class T_Par3>
class signal3
{
public:
    typedef slot_base3<T_Ret, T_Par1, T_Par2, T_Par3> slot_type;
    void connect(shared_ptr<slot_type> slot){
        slot_ = slot;
    }
    template<class T_Obj>
    void connect(T_Obj *obj, T_Ret (T_Obj::*fn)(T_Par1, T_Par2, T_Par3))
    {
        slot_ = shared_ptr<slot_type> (new mem_slot3<T_Ret, T_Obj, T_Par1, T_Par2, T_Par3>(obj, fn));
    }
    T_Ret emit(T_Par1 par1, T_Par2 par2, T_Par3 par3){
        return slot_->emit(par1, par2, par3);
    }
protected:
    /// @cond
    shared_ptr<slot_type> slot_;
    /// @endcond
};

/**
 * @brief Signal with 4 parameters.
 */
template<class T_Ret, class T_Par1, class T_Par2, class T_Par3, class T_Par4>
class signal4
{
public:
    typedef slot_base4<T_Ret, T_Par1, T_Par2, T_Par3, T_Par4> slot_type;
    void connect(shared_ptr<slot_type> slot){
        slot_ = slot;
    }
    template<class T_Obj>
    void connect(T_Obj *obj, T_Ret (T_Obj::*fn)(T_Par1, T_Par2, T_Par3, T_Par4))
    {
        slot_ = shared_ptr<slot_type> (new mem_slot4<T_Ret, T_Obj, T_Par1, T_Par2, T_Par3, T_Par4>(obj, fn));
    }
    T_Ret emit(T_Par1 par1, T_Par2 par2, T_Par3 par3, T_Par4 par4){
        return slot_->emit(par1, par2, par3, par4);
    }
protected:
    /// @cond
    shared_ptr<slot_type> slot_;
    /// @endcond
};

/**
 * @brief Signal with 5 parameters.
 */
template<class T_Ret, class T_Par1, class T_Par2, class T_Par3, class T_Par4, class T_Par5>
class signal5
{
public:
    typedef slot_base5<T_Ret, T_Par1, T_Par2, T_Par3, T_Par4, T_Par5>  slot_type;
    void connect(shared_ptr<slot_type> slot){
        slot_ = slot;
    }
    template<class T_Obj>
    void connect(T_Obj *obj, T_Ret (T_Obj::*fn)(T_Par1, T_Par2, T_Par3, T_Par4, T_Par5))
    {
        slot_ = shared_ptr<slot_type> (new mem_slot5<T_Ret, T_Obj, T_Par1, T_Par2, T_Par3, T_Par4, T_Par5>(obj, fn));
    }
    T_Ret emit(T_Par1 par1, T_Par2 par2, T_Par3 par3, T_Par4 par4, T_Par5 par5){
        return slot_->emit(par1, par2, par3, par4, par5);
    }
protected:
    /// @cond
    shared_ptr<slot_type> slot_;
    /// @endcond
};

/**
 * @brief Signal with 6 parameters.
 */
template<class T_Ret, class T_Par1, class T_Par2, class T_Par3, class T_Par4, class T_Par5, class T_Par6>
class signal6
{
public:
    typedef slot_base6<T_Ret, T_Par1, T_Par2, T_Par3, T_Par4, T_Par5, T_Par6> slot_type;
    void connect(shared_ptr<slot_type> slot){
        slot_ = slot;
    }
    template<class T_Obj>
    void connect(T_Obj *obj, T_Ret (T_Obj::*fn)(T_Par1, T_Par2, T_Par3, T_Par4, T_Par5, T_Par6))
    {
        slot_ = shared_ptr<slot_type> (new mem_slot6<T_Ret, T_Obj, T_Par1, T_Par2, T_Par3, T_Par4, T_Par5, T_Par6>(obj, fn));
    }
    T_Ret emit(T_Par1 par1, T_Par2 par2, T_Par3 par3, T_Par4 par4, T_Par5 par5, T_Par6 par6){
        return slot_->emit(par1, par2, par3, par4, par5, par6);
    }
protected:
    /// @cond
    shared_ptr<slot_type> slot_;
    /// @endcond
};

} //namespace bloom