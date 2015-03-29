/* 
 * File:   stringer.cpp
 * Author: Sergei "Wilder" Khairulin <sergei.khairulin@gmail.com>
 *
 * Created on 28 אוקטובר 2014, 15:50
 */

#include <bloom++/stream/stringer.h>

#define STREAM_STEP_SIZE 1024

namespace bloom {

namespace stream {

size_t stringer::write(const char* data, size_t size)
{
    str_.append(data, size);
    return size;
}

size_t stringer::read(char* data, size_t size)
{
    size_t sz = str_.length() - ipos_;
    if(sz >= size){
        memcpy(data, str_.c_str()+ipos_, size);
        ipos_ += size;
        return size;
    }
    
    memcpy(data, str_.c_str()+ipos_, sz);
    ipos_ += sz;
    return sz;
}

o_base &stringer::operator <<(i_base& i)
{
    char buf[STREAM_STEP_SIZE];
    size_t size = i.read(buf, STREAM_STEP_SIZE);
    size_t sz = write(buf, size);
    if(sz!=size)return *this;
    if(size!=STREAM_STEP_SIZE)return *this;
    return this->operator <<(i);
}

i_base &stringer::operator >>(o_base& o)
{
    o.write(str_.c_str(), str_.length());
    return *this;
}

bool stringer::oready()
{
    return true;
}

bool stringer::iready()
{
    return (ipos_ < str_.length());
}

} //namespace stream

} //namespace bloom
