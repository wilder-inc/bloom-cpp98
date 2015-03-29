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
#include <bloom++/_bits/c++config.h>

#ifdef AUX_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

template<class charT>
struct char_traits
{
    typedef charT       char_type;
    
    inline static int          compare(const char_type *s1, const char_type *s2, size_t n) FORCE_INLINE;
    inline static size_t       length(const char_type *s) FORCE_INLINE;
    inline static char_type *  find(const char_type *s1, size_t n, const char_type &a) FORCE_INLINE;
    inline static char_type *  move(const char_type *s1, const char_type *s2, size_t n) FORCE_INLINE;
    inline static char_type *  copy(const char_type *s1, const char_type *s2, size_t n) FORCE_INLINE;
    inline static char_type *  assign(const char_type *s, size_t n, char_type a) FORCE_INLINE;
};

template<>
struct char_traits<char>
{
    typedef char        char_type;
    
    inline static int          compare(const char_type *s1, const char_type *s2, size_t n) FORCE_INLINE{
        return memcmp(s1, s2, n);
    }
    
    inline static size_t       length(const char_type *s) FORCE_INLINE{
        if(s[0] == 0)
            return 0;
        if(s[1] == 0)
            return 1;
        return strlen(s);
    }
    
    inline static const char_type *  find(const char_type *s, size_t n, const char_type &a) FORCE_INLINE{
        return static_cast<const char_type*>(memchr(s, a, n));
    }
    
    inline static char_type *  move(char_type *s1, const char_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            s1[0] = s2[0];
            return s1 + 1;
        }
        return static_cast<char_type*>(memmove(s1, s2, n));
    }
    
    inline static char_type *  copy(char_type *s1, const char_type *s2, size_t n) FORCE_INLINE{
        if(!n)return s1;
        if(n == 1){
            s1[0] = s2[0];
            return s1 + 1;
        }
        return static_cast<char_type*>(memcpy(s1, s2, n));
    }
    
    inline static char_type *  assign(char_type *s, size_t n, char_type a) FORCE_INLINE{
        if(!n)return s;
        if(n == 1){
            s[0] = a;
            return s + 1;
        }
        return static_cast<char_type*>(memset(s, a, n));
    }
};

} //namespace bloom
