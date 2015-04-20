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

#include <bloom++/string.h>
#include <bloom++/stream/io.h>
#include <bloom++/stream/iostring.h>

#ifdef STREAM_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

namespace stream
{

/**
 * @brief In/out stream wrapper for strings.
 */
class stringer: public io_base
{
private:
    bloom::string &str_;
    size_t ipos_;
    
public:
    stringer(bloom::string &str):str_(str),ipos_(0){}
    virtual ~stringer(){}
    
    virtual size_t write(const char *data, size_t size);
    virtual o_base& operator<<(i_base &i);
    virtual bool oready();
    
    virtual size_t read(char *data, size_t size);
    virtual i_base &operator>>(o_base &o);
    virtual bool iready();
    
private:
    explicit stringer(const stringer &o); //< Forbid default constructor
    explicit stringer(); //< Forbid default constructor
};

} //namespace stream

/**
 * Fast string builder.
 * @param p1 String element.
 * @return string
 */
template<class P1>
string fast_ostring(P1 p1)
{
    string out;
    stream::stringer s(out);
    stream::ostring os(s);
    os<<p1;
    return out;
}

/**
 * Fast string builder.
 * @param p1 First string element.
 * @param p2 Second string element.
 * @return string
 */
template<class P1, class P2>
string fast_ostring(P1 p1, P2 p2)
{
    string out;
    stream::stringer s(out);
    stream::ostring os(s);
    os<<p1<<p2;
    return out;
}

/**
 * Fast string builder.
 * @param p1 First string element.
 * @param p2 Second string element.
 * @param p3 3th string element.
 * @return string
 */
template<class P1, class P2, class P3>
string fast_ostring(P1 p1, P2 p2, P3 p3)
{
    string out;
    stream::stringer s(out);
    stream::ostring os(s);
    os<<p1<<p2<<p3;
    return out;
}

/**
 * Fast string builder.
 * @param p1 First string element.
 * @param p2 Second string element.
 * @param p3 3th string element.
 * @param p4 4th string element.
 * @return string.
 */
template<class P1, class P2, class P3, class P4>
string fast_ostring(P1 p1, P2 p2, P3 p3, P4 p4)
{
    string out;
    stream::stringer s(out);
    stream::ostring os(s);
    os<<p1<<p2<<p3<<p4;
    return out;
}

/**
 * Fast string builder.
 * @param p1 First string element.
 * @param p2 Second string element.
 * @param p3 3th string element.
 * @param p4 4th string element.
 * @param p5 5th string element.
 * @return string.
 */
template<class P1, class P2, class P3, class P4, class P5>
string fast_ostring(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
{
    string out;
    stream::stringer s(out);
    stream::ostring os(s);
    os<<p1<<p2<<p3<<p4<<p5;
    return out;
}

/**
 * Fast string builder.
 * @param p1 First string element.
 * @param p2 Second string element.
 * @param p3 3th string element.
 * @param p4 4th string element.
 * @param p5 5th string element.
 * @param p6 6th string element.
 * @return 
 */
template<class P1, class P2, class P3, class P4, class P5, class P6>
string fast_ostring(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
{
    string out;
    stream::stringer s(out);
    stream::ostring os(s);
    os<<p1<<p2<<p3<<p4<<p5<<p6;
    return out;
}

/**
 * Fast string builder.
 * @param p1 First string element.
 * @param p2 Second string element.
 * @param p3 3th string element.
 * @param p4 4th string element.
 * @param p5 5th string element.
 * @param p6 6th string element.
 * @param p7 7th string element.
 * @return 
 */
template<class P1, class P2, class P3, class P4, class P5, class P6, class P7>
string fast_ostring(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
{
    string out;
    stream::stringer s(out);
    stream::ostring os(s);
    os<<p1<<p2<<p3<<p4<<p5<<p6<<p7;
    return out;
}

/**
 * Fast string builder.
 * @param p1 First string element.
 * @param p2 Second string element.
 * @param p3 3th string element.
 * @param p4 4th string element.
 * @param p5 5th string element.
 * @param p6 6th string element.
 * @param p7 7th string element.
 * @param p8 8th string element.
 * @return 
 */
template<class P1, class P2, class P3, class P4, class P5, class P6, 
         class P7, class P8>
string fast_ostring(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7,
                    P8 p8)
{
    string out;
    stream::stringer s(out);
    stream::ostring os(s);
    os<<p1<<p2<<p3<<p4<<p5<<p6<<p7<<p8;
    return out;
}

/**
 * Fast string builder.
 * @param p1 First string element.
 * @param p2 Second string element.
 * @param p3 3th string element.
 * @param p4 4th string element.
 * @param p5 5th string element.
 * @param p6 6th string element.
 * @param p7 7th string element.
 * @param p8 8th string element.
 * @param p9 9th string element.
 * @return 
 */
template<class P1, class P2, class P3, class P4, class P5, class P6, 
         class P7, class P8, class P9>
string fast_ostring(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7,
                    P8 p8, P9 p9)
{
    string out;
    stream::stringer s(out);
    stream::ostring os(s);
    os<<p1<<p2<<p3<<p4<<p5<<p6<<p7<<p8<<p9;
    return out;
}

/**
 * Fast string builder.
 * @param p1 First string element.
 * @param p2 Second string element.
 * @param p3 3th string element.
 * @param p4 4th string element.
 * @param p5 5th string element.
 * @param p6 6th string element.
 * @param p7 7th string element.
 * @param p8 8th string element.
 * @param p9 9th string element.
 * @param p10 10th string element.
 * @return 
 */
template<class P1, class P2, class P3, class P4, class P5, class P6, 
         class P7, class P8, class P9, class P10>
string fast_ostring(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7,
                    P8 p8, P9 p9, P10 p10)
{
    string out;
    stream::stringer s(out);
    stream::ostring os(s);
    os<<p1<<p2<<p3<<p4<<p5<<p6<<p7<<p8<<p9<<p10;
    return out;
}

} //namespace bloom