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

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <bloom++/_bits/c++config.h>

#ifdef AUX_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

/**
 * @brief Traits of types
 */
template<class vT>
struct traits
{
    typedef vT                  value_type;
    
    inline static void          construct(value_type *p, size_t n, const value_type &v) FORCE_INLINE {
        if(!n)return;
        const value_type *endi = p + n;
        for(;p != endi; ++p)
            ::new ((void*)p) value_type(v);
    }
    
    inline static void          copy_construct(value_type *p1, const value_type *s2, size_t n) FORCE_INLINE {
        if(!n)return;
        if(n == 1){
            ::new ((void*)p1) value_type(*s2);
            return;
        }
        const value_type *endi = p1 + n;
        for(;p1 != endi; ++p1, ++s2)
            ::new ((void*)p1) value_type(*s2);
    }
    
    inline static void          move_construct(value_type *p1, const value_type *s2, size_t n) FORCE_INLINE {
        if(!n)return;
        if(n == 1){
            ::new ((void*)p1) value_type(*s2);
            s2->~value_type();
            return;
        }
        if(p1 > s2 && p1 < s2 + n){
            const value_type *endi = p1 - 1;
            for(p1 += n - 1, s2 += n - 1;p1 != endi; --p1, --s2){
                ::new ((void*)p1) value_type(*s2);
                s2->~value_type();
            }
        }
        else {
            const value_type *endi = p1 + n;
            for(;p1 != endi; ++p1, ++s2){
                ::new ((void*)p1) value_type(*s2);
                s2->~value_type();
            }
        }
    }
    
    inline static void          destroy(value_type *p, size_t n) FORCE_INLINE {
        if(!n)return;
        const value_type *endi = p + n;
        for(;p != endi; ++p)
            p->~value_type();
    }
    
    inline static value_type *  move(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE {
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        if(s1 > s2 && s1 < s2 + n){
            const value_type *endi = s1 - 1;
            for(s1 += n - 1, s2 += n - 1;s1 != endi; --s1, --s2)
                *s1 = *s2;
        }
        else {
            const value_type *endi = s1 + n;
            for(;s1 != endi; ++s1, ++s2)
                *s1 = *s2;
        }
        return s1;
    }
    
    inline static value_type *  copy(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE {
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        const value_type *endi = s1 + n;
        for(;s1 != endi; ++s1, ++s2)
            *s1 = *s2;
        return s1;
    }
    
    inline static value_type *  assign(value_type *s, size_t n, const value_type &a) FORCE_INLINE {
        if(!n)return s;
        if(n == 1){
            *s = a;
            return ++s;
        }
        const value_type *endi = s + n;
        for(;s != endi; ++s)
            *s = a;
        return s;
    }
};

/**
 * @brief Traits of pointers
 */
template<class vT>
struct traits<vT*>
{
    typedef vT *                value_type;
    
    inline static void          construct(value_type *p, size_t n, const value_type &v) FORCE_INLINE {
    }
    
    inline static void          copy_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::copy(p1, p2, n);
    }
    
    inline static void          move_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::move(p1, p2, n);
    }
    
    inline static void          destroy(value_type *p, size_t n) FORCE_INLINE {
    }
    
    inline static value_type *  move(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memmove(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  copy(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memcpy(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  assign(value_type *s, size_t n, const value_type &a) FORCE_INLINE{
        if(!n)return s;
        if(n == 1){
            *s = a;
            return ++s;
        }
        const value_type *endi = s + n;
        for(;s != endi; ++s)
            *s = a;
        return s;
    }
};

template<>
struct traits<char>
{
    typedef char                value_type;
    
    inline static void          construct(value_type *p, size_t n, value_type v) FORCE_INLINE {
    }
    
    inline static void          copy_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::copy(p1, p2, n);
    }
    
    inline static void          move_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::move(p1, p2, n);
    }
    
    inline static void          destroy(value_type *p, size_t n) FORCE_INLINE {
    }
    
    inline static value_type *  move(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memmove(s1, s2, n));
    }
    
    inline static value_type *  copy(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memcpy(s1, s2, n));
    }
    
    inline static value_type *  assign(value_type *s, size_t n, value_type a) FORCE_INLINE{
        if(!n)return s;
        if(n == 1){
            *s = a;
            return ++s;
        }
        return static_cast<value_type*>(memset(s, a, n));
    }
};

template<>
struct traits<unsigned char>
{
    typedef unsigned char       value_type;
    
    inline static void          construct(value_type *p, size_t n, value_type v) FORCE_INLINE {
    }
    
    inline static void          copy_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::copy(p1, p2, n);
    }
    
    inline static void          move_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::move(p1, p2, n);
    }
    
    inline static void          destroy(value_type *p, size_t n) FORCE_INLINE {
    }
    
    inline static value_type *  move(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memmove(s1, s2, n));
    }
    
    inline static value_type *  copy(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memcpy(s1, s2, n));
    }
    
    inline static value_type *  assign(value_type *s, size_t n, value_type a) FORCE_INLINE{
        if(!n)return s;
        if(n == 1){
            *s = a;
            return ++s;
        }
        return static_cast<value_type*>(memset(s, a, n));
    }
};

template<>
struct traits<short>
{
    typedef short               value_type;
    
    inline static void          construct(value_type *p, size_t n, value_type v) FORCE_INLINE {
    }
    
    inline static void          copy_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::copy(p1, p2, n);
    }
    
    inline static void          move_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::move(p1, p2, n);
    }
    
    inline static void          destroy(value_type *p, size_t n) FORCE_INLINE {
    }
    
    inline static value_type *  move(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memmove(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  copy(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memcpy(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  assign(value_type *s, size_t n, value_type a) FORCE_INLINE{
        if(!n)return s;
        if(n == 1){
            *s = a;
            return ++s;
        }
        const value_type *endi = s + n;
        for(;s != endi; ++s)
            *s = a;
        return s;
    }
};

template<>
struct traits<unsigned short>
{
    typedef unsigned short      value_type;
    
    inline static void          construct(value_type *p, size_t n, value_type v) FORCE_INLINE {
    }
    
    inline static void          copy_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::copy(p1, p2, n);
    }
    
    inline static void          move_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::move(p1, p2, n);
    }
    
    inline static void          destroy(value_type *p, size_t n) FORCE_INLINE {
    }
    
    inline static value_type *  move(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memmove(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  copy(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memcpy(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  assign(value_type *s, size_t n, value_type a) FORCE_INLINE{
        if(!n)return s;
        if(n == 1){
            *s = a;
            return ++s;
        }
        const value_type *endi = s + n;
        for(;s != endi; ++s)
            *s = a;
        return s;
    }
};

template<>
struct traits<int>
{
    typedef int                 value_type;
    
    inline static void          construct(value_type *p, size_t n, const value_type &v) FORCE_INLINE {
    }
    
    inline static void          copy_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::copy(p1, p2, n);
    }
    
    inline static void          move_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::move(p1, p2, n);
    }
    
    inline static void          destroy(value_type *p, size_t n) FORCE_INLINE {
    }
    
    inline static value_type *  move(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memmove(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  copy(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memcpy(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  assign(value_type *s, size_t n, const value_type &a) FORCE_INLINE{
        if(!n)return s;
        if(n == 1){
            *s = a;
            return ++s;
        }
        const value_type *endi = s + n;
        for(;s != endi; ++s)
            *s = a;
        return s;
    }
};

template<>
struct traits<unsigned int>
{
    typedef unsigned int        value_type;
    
    inline static void          construct(value_type *p, size_t n, const value_type &v) FORCE_INLINE {
    }
    
    inline static void          copy_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::copy(p1, p2, n);
    }
    
    inline static void          move_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::move(p1, p2, n);
    }
    
    inline static void          destroy(value_type *p, size_t n) FORCE_INLINE {
    }
    
    inline static value_type *  move(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memmove(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  copy(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memcpy(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  assign(value_type *s, size_t n, const value_type &a) FORCE_INLINE{
        if(!n)return s;
        if(n == 1){
            *s = a;
            return ++s;
        }
        const value_type *endi = s + n;
        for(;s != endi; ++s)
            *s = a;
        return s;
    }
};

template<>
struct traits<long>
{
    typedef long                value_type;
    
    inline static void          construct(value_type *p, size_t n, const value_type &v) FORCE_INLINE {
    }
    
    inline static void          copy_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::copy(p1, p2, n);
    }
    
    inline static void          move_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::move(p1, p2, n);
    }
    
    inline static void          destroy(value_type *p, size_t n) FORCE_INLINE {
    }
    
    inline static value_type *  move(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memmove(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  copy(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memcpy(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  assign(value_type *s, size_t n, const value_type &a) FORCE_INLINE{
        if(!n)return s;
        if(n == 1){
            *s = a;
            return ++s;
        }
        const value_type *endi = s + n;
        for(;s != endi; ++s)
            *s = a;
        return s;
    }
};

template<>
struct traits<unsigned long>
{
    typedef unsigned long       value_type;
    
    inline static void          construct(value_type *p, size_t n, const value_type &v) FORCE_INLINE {
    }
    
    inline static void          copy_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::copy(p1, p2, n);
    }
    
    inline static void          move_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::move(p1, p2, n);
    }
    
    inline static void          destroy(value_type *p, size_t n) FORCE_INLINE {
    }
    
    inline static value_type *  move(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memmove(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  copy(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memcpy(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  assign(value_type *s, size_t n, const value_type &a) FORCE_INLINE{
        if(!n)return s;
        if(n == 1){
            *s = a;
            return ++s;
        }
        const value_type *endi = s + n;
        for(;s != endi; ++s)
            *s = a;
        return s;
    }
};

template<>
struct traits<long long>
{
    typedef long long           value_type;
    
    inline static void          construct(value_type *p, size_t n, const value_type &v) FORCE_INLINE {
    }
    
    inline static void          copy_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::copy(p1, p2, n);
    }
    
    inline static void          move_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::move(p1, p2, n);
    }
    
    inline static void          destroy(value_type *p, size_t n) FORCE_INLINE {
    }
    
    inline static value_type *  move(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memmove(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  copy(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memcpy(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  assign(value_type *s, size_t n, const value_type &a) FORCE_INLINE{
        if(!n)return s;
        if(n == 1){
            *s = a;
            return ++s;
        }
        const value_type *endi = s + n;
        for(;s != endi; ++s)
            *s = a;
        return s;
    }
};

template<>
struct traits<unsigned long long>
{
    typedef unsigned long long  value_type;
    
    inline static void          construct(value_type *p, size_t n, const value_type &v) FORCE_INLINE {
    }
    
    inline static void          copy_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::copy(p1, p2, n);
    }
    
    inline static void          move_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::move(p1, p2, n);
    }
    
    inline static void          destroy(value_type *p, size_t n) FORCE_INLINE {
    }
    
    inline static value_type *  move(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memmove(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  copy(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memcpy(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  assign(value_type *s, size_t n, const value_type &a) FORCE_INLINE{
        if(!n)return s;
        if(n == 1){
            *s = a;
            return ++s;
        }
        const value_type *endi = s + n;
        for(;s != endi; ++s)
            *s = a;
        return s;
    }
};

template<>
struct traits<double>
{
    typedef double              value_type;
    
    inline static void          construct(value_type *p, size_t n, const value_type &v) FORCE_INLINE {
    }
    
    inline static void          copy_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::copy(p1, p2, n);
    }
    
    inline static void          move_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::move(p1, p2, n);
    }
    
    inline static void          destroy(value_type *p, size_t n) FORCE_INLINE {
    }
    
    inline static value_type *  move(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memmove(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  copy(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memcpy(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  assign(value_type *s, size_t n, const value_type &a) FORCE_INLINE{
        if(!n)return s;
        if(n == 1){
            *s = a;
            return ++s;
        }
        const value_type *endi = s + n;
        for(;s != endi; ++s)
            *s = a;
        return s;
    }
};

template<>
struct traits<float>
{
    typedef float               value_type;
    
    inline static void          construct(value_type *p, size_t n, const value_type &v) FORCE_INLINE {
    }
    
    inline static void          copy_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::copy(p1, p2, n);
    }
    
    inline static void          move_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::move(p1, p2, n);
    }
    
    inline static void          destroy(value_type *p, size_t n) FORCE_INLINE {
    }
    
    inline static value_type *  move(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memmove(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  copy(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memcpy(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  assign(value_type *s, size_t n, const value_type &a) FORCE_INLINE{
        if(!n)return s;
        if(n == 1){
            *s = a;
            return ++s;
        }
        const value_type *endi = s + n;
        for(;s != endi; ++s)
            *s = a;
        return s;
    }
};

template<>
struct traits<long double>
{
    typedef long double         value_type;
    
    inline static void          construct(value_type *p, size_t n, const value_type &v) FORCE_INLINE {
    }
    
    inline static void          copy_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::copy(p1, p2, n);
    }
    
    inline static void          move_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::move(p1, p2, n);
    }
    
    inline static void          destroy(value_type *p, size_t n) FORCE_INLINE {
    }
    
    inline static value_type *  move(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memmove(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  copy(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memcpy(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  assign(value_type *s, size_t n, const value_type &a) FORCE_INLINE{
        if(!n)return s;
        if(n == 1){
            *s = a;
            return ++s;
        }
        const value_type *endi = s + n;
        for(;s != endi; ++s)
            *s = a;
        return s;
    }
};

template<>
struct traits<bool>
{
    typedef bool                value_type;
    
    inline static void          construct(value_type *p, size_t n, const value_type &v) FORCE_INLINE {
    }
    
    inline static void          copy_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::copy(p1, p2, n);
    }
    
    inline static void          move_construct(value_type *p1, const value_type *p2, size_t n) FORCE_INLINE {
        traits<value_type>::move(p1, p2, n);
    }
    
    inline static void          destroy(value_type *p, size_t n) FORCE_INLINE {
    }
    
    inline static value_type *  move(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memmove(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  copy(value_type *s1, const value_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            *s1 = *s2;
            return ++s1;
        }
        return static_cast<value_type*>(memcpy(s1, s2, sizeof(value_type) * n));
    }
    
    inline static value_type *  assign(value_type *s, size_t n, const value_type &a) FORCE_INLINE{
        if(!n)return s;
        if(n == 1){
            *s = a;
            return ++s;
        }
        const value_type *endi = s + n;
        for(;s != endi; ++s)
            *s = a;
        return s;
    }
};


} //namespace bloom
