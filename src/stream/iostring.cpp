/*****************************************************************
  Author: Sergei "Wilder" Khairulin <sergei.khairulin@gmail.com>
 ****************************************************************/

#include <bloom++/stream/iostring.h>
#include <bloom++/stream/iobuffer.h>
#include <stdio.h>

#ifdef STREAM_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

namespace stream
{

unsigned char istring::get()
{   
    prev_ch = curr_ch;
    char ch;
    read((char *)&ch, sizeof(char));
    if (ch == 0x0d)
    {
        read((char *)&ch, sizeof(char));
        if (ch != 0x0a)
            iob_.write(&ch, sizeof(char));
        line_num_++;
        curr_ch = '\n';
        return '\n';
    }
    if (ch == 0x0a)
    {
        line_num_++;
        curr_ch = '\n';
        return '\n';
    }
    //cout<<"**["<<(int)ch<<"]**\n";
    curr_ch = ch;
    return ch;
}

istring & istring::operator>>(string &elem)
{
    read_element(elem);
    return *this;
}

istring & istring::operator>>(char &value)
{
    int l_val;
    bool minus;
    string elem;

    read_element(elem);
    if (elem == "-")
    {
        minus = true;
        read_element(elem);
    }
    else
        minus = false;
    l_val = atoi(elem.c_str());
    //if (l_val > 127)return false;
    value = (char) l_val;
    if (minus)value = 0 - value;
    return *this;
}

istring & istring::operator>>(unsigned char &value)
{
    int l_val;
    string elem;

    read_element(elem);
    l_val = atoi(elem.c_str());
    //if (l_val > 255)return false;
    value = (unsigned char) l_val;
    return *this;
}

istring & istring::operator>>(int &value)
{
    int l_val;
    bool minus;
    string elem;
    
    read_element(elem);
    if (elem == "-")
    {
        minus = true;
        read_element(elem);
    }
    else
        minus = false;
    l_val = atoi(elem.c_str());
    value = (int) l_val;
    if (minus)value = 0 - value;
    return *this;
}

istring & istring::operator>>(unsigned int &value)
{
    int l_val;
    string elem;
    
    read_element(elem);
    l_val = atoi(elem.c_str());
    value = (size_t) l_val;
    return *this;
}

istring & istring::operator>>(short &value)
{
    int l_val;
    bool minus;
    string elem;
    
    read_element(elem);
    if (elem == "-")
    {
        minus = true;
        read_element(elem);
    }
    else
        minus = false;
    l_val = atoi(elem.c_str());
    value = (short) l_val;
    if (minus)value = 0 - value;
    return *this;
}

istring & istring::operator>>(unsigned short &value)
{
    int l_val;
    string elem;
    
    read_element(elem);
    l_val = atoi(elem.c_str());
    value = (unsigned short) l_val;
    return *this;
}

istring & istring::operator>>(long &value)
{
    long l_val;
    bool minus;
    string elem;

    read_element(elem);
    if (elem == "-")
    {
        minus = true;
        read_element(elem);
    }
    else
        minus = false;
    l_val = atol(elem.c_str());
    value = l_val;
    if (minus)value = 0 - value;
    return *this;
}

istring & istring::operator>>(unsigned long &value)
{
    long l_val;
    string elem;

    read_element(elem);
    l_val = atol(elem.c_str());
    value = (unsigned long)l_val;
    return *this;
}

istring & istring::operator>>(float &value)
{
    float l_val;
    bool minus;
    string elem;

    read_element(elem, true);
    if (elem == "-")
    {
        minus = true;
        read_element(elem);
    }
    else
        minus = false;
    l_val = atof(elem.c_str());
    value = l_val;
    if (minus)value = 0 - value;
    return *this;
}

istring & istring::operator>>(double &value)
{
    double l_val;
    bool minus;
    string elem;

    read_element(elem, true);
    if (elem == "-")
    {
        minus = true;
        read_element(elem);
    }
    else
        minus = false;
    l_val = strtod(elem.c_str(), 0);
    value = l_val;
    if (minus)value = 0 - value;
    return *this;
}

istring & istring::operator>>(bool &value)
{
    string elem;

    read_element(elem);
    if (elem == "true")value = true;
    else value = false;
    return *this;
}

size_t istring::read(char *data, size_t size)
{
    size_t bsz = iob_.read(data, size);
    if(bsz < size){
        return bsz + i_.read(data+bsz, size-bsz);
    }
    /*
    else {
        char str[2] = {0, 0};
        str[0] = data[0];
        DEBUG_INFO("read from buffer: \'"<<str<<"\'\n");
    }
     */
    return bsz;
}

istring &istring::operator>>(o_base &o)
{
    i_.operator>>(o);
    return *this;
}

// The same as in o_base
ostring &ostring::operator<<(const string &data)
{
    o_.write((char *) data.c_str(), data.length()); // TODO: process return value
    return *this;
}

ostring &ostring::operator<<(const char *data)
{
    o_.write(data, strlen(data)); // TODO: process return value
    return *this;
}

ostring &ostring::operator <<(const char& ch)
{
    char str[5];
    snprintf(str, 5, "%d", (int)ch);
    *this<<(char *)str;
    return *this;
}

ostring &ostring::operator <<(const unsigned char& ch)
{
    char str[5];
    snprintf(str, 5, "%u", (size_t)ch);
    *this<<(char *)str;
    return *this;
}

ostring &ostring::operator <<(const int& data)
{
    char str[64];
    snprintf(str, 64, "%d", data);
    *this<<(char *)str;
    return *this;
}

ostring &ostring::operator <<(const unsigned int& data)
{
    char str[64];
    snprintf(str, 64, "%u", data);
    *this<<(char *)str;
    return *this;
}

ostring &ostring::operator <<(const short& data)
{
    char str[10];
    snprintf(str, 10, "%d", data);
    *this<<(char *)str;
    return *this;
}

ostring &ostring::operator <<(const unsigned short& data)
{
    char str[10];
    snprintf(str, 10, "%u", data);
    *this<<(char *)str;
    return *this;
}

ostring &ostring::operator <<(const long& data)
{
    char str[128];
    snprintf(str, 128, "%d", data);
    *this<<(char *)str;
    return *this;
}

ostring &ostring::operator <<(const unsigned long& data)
{
    char str[128];
    snprintf(str, 128, "%u", data);
    *this<<(char *)str;
    return *this;
}

ostring &ostring::operator <<(const double& data)
{
    char str[128];
    snprintf(str, 128, "%f", data);
    *this<<(char *)str;
    return *this;
}

ostring &ostring::operator <<(const float& data)
{
    char str[128];
    snprintf(str, 128, "%f", data);
    *this<<(char *)str;
    return *this;
}

ostring &ostring::operator <<(const bool& data)
{
    if(data){
        *this<<"true";
    }
    else {
        *this<<"false";
    }
    return *this;
}

size_t ostring::write(const char *data, size_t size)
{
    return o_.write(data, size);
}

ostring& ostring::operator<<(i_base &i)
{
    o_.operator<<(i); // TODO: process return value
    return *this;
}

/*
bool istring::operator --()
{
    //std::cout<<"**--** prev_ch = "<<std::hex<<(unsigned short)prev_ch<<std::dec<<"\n";
    bool ret = --i_;
    if (ret)
    {
        if (curr_ch == '\n')
        {
            line_num_--;
            //std::cout<<"**--**\n";
            curr_ch = prev_ch;
            prev_ch = 0;
        }
    }
    return ret;
}
 */

bool istring::read_element(string &buf, bool bNumberStr)
{
    char ch, ch2;
    buf = "";

    while (!empty())
    {
        ch = get();
        //DEBUG_INFO("get: "<<(char *)tch<<"\n");

        if (ch == '\\')continue;
        if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\\' || ch == '\t')
        {
            if (buf.length() == 0)
            {
                //if(sOut)
                //  (*sOut)+=ch;
                continue;
            }
        }
        if (bNumberStr){
            if (ch == '.')
            {
                buf += ch;
                continue;
            }
        }
        if (check_literals(ch))
        {
            if (buf.length() > 0) //if already have elem in buf
            {
                iob_.write(&ch, sizeof(char));
            }
            else {
                if (ch == '/') //verify comments
                {
                    ch2 = get();
                    if (ch2 == '/')
                    {
                        for(ch=get(); (ch!='\n')&&(!empty()); ch=get());
                        continue;
                    }
                    else if (ch2 == '*')
                    {
                        //if(sOut)(*sOut)+="/*";
                        ch = get();
                        ch2 = get();
                        while (true)
                        {
                            if ((ch == '*' && ch2 == '/') || (empty()))break;
                            //if(sOut)(*sOut)+=ch;
                            ch = ch2;
                            ch2 = get();
                        }
                        //if(sOut)(*sOut)+="*/";
                        continue;
                    }
                    else {
                        iob_.write(&ch2, sizeof(char));
                    }
                }
                buf += ch;
                if (buf == "#")continue; // for words like preprocessor operators (#define #include etc...)
            }
            return true;
        }
        
        

        buf += ch;
        //DEBUG_INFO("buf: "<<buf<<"\n");
    }
    return false;
}

bool istring::read_string(char closeElem, string &str)
{
    str = "";
    while (!empty())
    {
        char ch = get();
        if (ch == closeElem)return true;
        if (ch == '\000')return false;
        str += ch;
    }
    return false;
}

bool istring::read_line_to(char closeElem, string &str)
{
    str = "";
    while (!empty())
    {
        char ch = get();
        if ((ch == closeElem) || (ch == '\n') || (ch == '\r'))return true;
        if (ch == '\000')return false;
        //std::cout<<"ch = "<<(int)ch<<"\n";
        str += ch;
    }
    return false;
}

size_t istring::line()
{
    return line_num_;
}

bool istring::find_string_value(const string &name)
{
    string elem;
    while (!empty())
    {
        read_element(elem);
        if (elem == name)
        {
            read_element(elem);
            if (elem == "=")
            {
                return true;
            }
            else break;
        }
    }
    return false;
}

bool istring::go_to(unsigned char ch)
{
    while (!empty())
    {
        char el = get();
        if (ch == el)return true;
    }
    return false;
}

void istring::next_line()
{
    char ch;
    for (ch = get(); ch != '\n' && (!empty()); ch = get());
}

bool istring::check_literals(char ch)
{
    // TODO: User defined literals!!!
    bool result;
    result = (ch < 48) | (ch > 57 && ch < 65) | (ch > 90 && ch < 95) | \
               (ch == 96) | (ch > 122);
    return result;
}

bool istring::iready()
{
    return (i_.iready()||iob_.iready());
}

bool istring::empty()
{
    return !iready();
}

} //namespace stream

} //namespace bloom
