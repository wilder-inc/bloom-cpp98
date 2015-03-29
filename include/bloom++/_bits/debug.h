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

#include <bloom++/exception.h>
#include <iostream>

#ifndef BLOOM_LOG_OBJ
#define BLOOM_LOG_OBJ std::cout
#endif

#ifdef DEBUG_INFO
#undef DEBUG_INFO
#endif

#ifdef DEBUG_WARN
#undef DEBUG_WARN
#endif

#ifdef DEBUG_ERROR
#undef DEBUG_ERROR
#endif

#ifdef BLOOM_FAILED
#undef BLOOM_FAILED
#endif

#ifdef __BLOOM_WITH_DEBUG
#undef __BLOOM_WITH_DEBUG
#define DEBUG_INFO(STREAM) BLOOM_LOG_OBJ<<__FILE__<<":"<<__LINE__<<" ["<<__FUNCTION__<<"]: "<<STREAM
#define DEBUG_WARN(STREAM) BLOOM_LOG_OBJ<<__FILE__<<":"<<__LINE__<<" ["<<__FUNCTION__<<"] WARN: "<<STREAM
#else
#define DEBUG_INFO(STREAM)//
#define DEBUG_WARN(STREAM)//
#endif //__BLOOM_WITH_DEBUG

#define DEBUG_ERROR(STREAM) BLOOM_LOG_OBJ<<__FILE__<<":"<<__LINE__<<" ["<<__FUNCTION__<<"] ERROR: "<<STREAM

#ifdef BLOOM_WITH_EXCEPTIONS
#define BLOOM_FAILED(MSG) throw bloom::exception(MSG)
#else
#define BLOOM_FAILED(MSG) do{ DEBUG_ERROR(MSG); int a = 0; a /= a; }while(0)
#endif