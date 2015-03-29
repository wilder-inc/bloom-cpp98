/* 
 * File:   ostream.cpp
 * Author: Sergei "Wilder" Khairulin <sergei.khairulin@gmail.com>
 *
 * Created on 25 אוקטובר 2014, 17:12
 */

#include <bloom++/stream/iostream.h>

namespace bloom
{

std::ostream &operator<<(std::ostream &o, stream::i_base &ib)
{
    char data[BLOOM_STREAM_READ_STEP];
    size_t sz = ib.read(data, BLOOM_STREAM_READ_STEP);
    if(sz){
        o.write(data, sz);
        if(sz == BLOOM_STREAM_READ_STEP){
            return o<<ib;
        }
    }
    return o;
}

} //namespace bloom