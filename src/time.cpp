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

#include <string.h>
#include <bloom++/time.h>
#include <bloom++/_bits/c++config.h>
#include <bloom++/exception.h>

#ifdef LINUX
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#else
#include <time.h>
//#include <mmsystem.h>
#include <stdint.h>
#endif

#ifdef DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom {

string timeToStr(std::tm tm, const string &format)
{
  string result;
  char buf[100];
  strftime(buf, 100, format.c_str(), &tm);
  result = buf;
  return result;
}

std::tm strToTime(const string &stime, const string &format)
{
  std::tm ttm;
  memset(&ttm, 0, sizeof(struct std::tm));
  strptime(stime.c_str(), format.c_str(), &ttm);
  return ttm;
}

void swapbytes(void *object, size_t size)
{
   unsigned char *start, *end;
   for ( start = (unsigned char *)object, end = start + size - 1; start < end; ++start, --end )
   {
      unsigned char swap = *start;
      *start = *end;
      *end = swap;
   }
}

long get_milli_sec() throw()
{
    struct timeval tv;
    long ret;
#ifdef LINUX
    if (gettimeofday(&tv, 0))
    {
        throw exception("get_milli_sec: gettimeofday failed...");
    }
    ret = tv.tv_sec * 1000l;
    ret += tv.tv_usec / 1000l;
#else
    ret = GetTickCount();
#endif                                                                                                                                                                                 
    return ret;
}

} //namespace bloom
