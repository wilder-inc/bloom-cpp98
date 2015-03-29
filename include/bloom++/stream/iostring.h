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
#include <bloom++/stream/iobuffer.h>

namespace bloom
{

namespace stream
{

/**
 * @brief Output text stream.
 */
class ostring: public o_base
{
protected:
    o_base &o_;

public:

    ostring(io_base &io) : o_(io){}
    ostring(o_base &o) : o_(o){}
    virtual ~ostring(){}
    
    //Writing
    ostring &operator<<(const char *data);
    ostring &operator<<(const string &data); //the same as in o_base
    ostring &operator<<(const char &); //(as integer value)
    ostring &operator<<(const unsigned char &); //(as integer value)
    ostring &operator<<(const int &);
    ostring &operator<<(const unsigned int &);
    ostring &operator<<(const short &);
    ostring &operator<<(const unsigned short &);
    ostring &operator<<(const long &);
    ostring &operator<<(const unsigned long &);
    ostring &operator<<(const long long&);
    ostring &operator<<(const unsigned long long &);
    ostring &operator<<(const double &);
    ostring &operator<<(const float &);
    ostring &operator<<(const bool &);
    virtual size_t write(const char *data, size_t size);
    virtual ostring& operator<<(i_base &i);
    virtual bool oready(){return o_.oready();}
    
private:
    explicit ostring(const ostring &o); //< Forbid default constructor
    explicit ostring(); //< Forbid default constructor
};

/**
 * @brief Input text stream.
 */
class istring: public i_base
{
protected:
    i_base &i_;
    iobuffer iob_;
    unsigned char curr_ch;
    unsigned char prev_ch;
    size_t line_num_;

    //fixme: user defined literals!!!
    bool check_literals(char ch);

public:
    istring(io_base &io) : i_(io), iob_(16), curr_ch(0), prev_ch(0), line_num_(1){}
    istring(i_base &o) : i_(o), curr_ch(0), prev_ch(0), line_num_(1){}
    virtual ~istring(){}
    
    //Reading
    virtual size_t read(char *data, size_t size);
    virtual istring &operator>>(o_base &o);
    virtual bool iready();
    
    istring & operator>>(string &data);
    istring & operator>>(char &);
    istring & operator>>(unsigned char &);
    istring & operator>>(int &);
    istring & operator>>(unsigned int &);
    istring & operator>>(short &);
    istring & operator>>(unsigned short &);
    istring & operator>>(long &);
    istring & operator>>(unsigned long &);
    istring & operator>>(double &);
    istring & operator>>(float &);
    istring & operator>>(bool &);

    bool read_element(string &elem, bool bNumberStr = false);
    bool go_to(unsigned char ch);
    size_t line();
    void next_line();
    bool read_string(char closeElem, string &str);
    bool read_line_to(char closeElem, string &str);
    bool find_string_value(const string &name);

    unsigned char get();
    bool empty();
    
private:
    explicit istring(const istring &o); //< Forbid default constructor
    explicit istring(); //< Forbid default constructor
};

/**
 * @brief Input/Output text stream.
 */
class iostring: public istring, public ostring
{
public:
    iostring(i_base &i, o_base &o) : istring(i), ostring(o) {}
    iostring(io_base &io) : istring(io), ostring(io) {}
    virtual ~iostring(){}
    
    using istring::operator >>;
    using istring::read;
    using istring::read_element;
    using istring::find_string_value;
    using istring::iready;
    using istring::get;
    using istring::line;
    using istring::next_line;
    using istring::read_line_to;
    using istring::read_string;
    
    using ostring::operator <<;
    using ostring::write;
    using ostring::oready;
    
private:
    explicit iostring(const iostring &o); //< Forbid default constructor
    explicit iostring(); //< Forbid default constructor
};

} //namespace stream

} //namespace bloom
