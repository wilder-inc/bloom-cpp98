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

#include <ctime>
#include <bloom++/string.h>

namespace bloom {

string            timeToStr(std::tm tm, const string &format);
std::tm           strToTime(const string &stime, const string &format);
//std::tm           dtToTm(CWDateTime dt);

void              swapbytes(void *object, size_t size);
long              get_milli_sec();

template<class T>
T                 swapValue(T &value)
{
  T res = value;
  swapbytes(&res, sizeof(T));
  return res;
}

} //namespace bloom
