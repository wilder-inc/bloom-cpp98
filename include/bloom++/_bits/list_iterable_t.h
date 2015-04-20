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

#include <stddef.h>

#ifdef AUX_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

//extern int i_count;

namespace bloom
{

struct list_iterable_base
{
    list_iterable_base *pNext_;
    list_iterable_base *pPrev_;
};

/**
 * @brief The iterable class
 */
template<class vT>
struct list_iterable_t: public list_iterable_base
{  
    list_iterable_t<vT>(){}
    
    template<class P1>
    list_iterable_t<vT>(P1 p1): value_(p1){}
    
    template<class P1, class P2>
    list_iterable_t<vT>(P1 p1, P2 p2): value_(p1, p2){}
    
    template<class P1, class P2, class P3>
    list_iterable_t<vT>(P1 p1, P2 p2, P3 p3): value_(p1, p2, p3){}
    
    template<class P1, class P2, class P3, class P4>
    list_iterable_t<vT>(P1 p1, P2 p2, P3 p3, P4 p4): value_(p1, p2, p3, p4){}
    
    template<class P1, class P2, class P3, class P4, class P5>
    list_iterable_t<vT>(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5): 
        value_(p1, p2, p3, p4, p5){}
    
    template<class P1, class P2, class P3, class P4, class P5, class P6>
    list_iterable_t<vT>(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6): 
        value_(p1, p2, p3, p4, p5, p6){}
    
    vT value_;
};

} //namespace bloom

