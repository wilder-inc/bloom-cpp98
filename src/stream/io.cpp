/*****************************************************************
  Author: Sergei "Wilder" Khairulin <sergei.khairulin@gmail.com>
 ****************************************************************/

#include <bloom++/stream/io.h>
#include <bloom++/string.h>

namespace bloom 
{

namespace stream 
{

/*
o_base &o_base::operator <<(const string &str){
    write(str.c_str(), str.length());
    return *this;
}

o_base &o_base::operator <<(const char *str){
    write(str, strlen(str));
    return *this;
}
 

o_base &o_base::put(unsigned char byte)
{
    write((const char *)&byte, sizeof(unsigned char));
    return *this;
}

bool o_base::full()
{
    return false;
}

//---------------------------


unsigned char i_base::get()
{
    unsigned char ch;
    read((char *)&ch, sizeof(char));
    return ch;
}

bool i_base::empty()
{
    return false;
}
 */
        
} //namespace stream

} //namespace bloom
