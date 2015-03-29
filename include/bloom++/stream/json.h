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

namespace bloom {

namespace stream {

/**
 * @brief Output JSON class.
 */
class o_json
{
protected:
    ostring & out_;
    bool b_first_;
public:

    o_json(ostring &o) : out_(o), b_first_(true)
    {
    }

    o_json(o_json &oj) : out_(oj.out_), b_first_(true)
    {
    }
};

class jarray;

/**
 * @brief JSON Object.
 */
class json : public o_json
{
protected:
    void out_str(const char *data, size_t size);

    void out_str(const string &data)
    {
        out_str(data.c_str(), data.length());
    }

public:

    json(ostring &o) : o_json(o)
    {
        out_ << "{";
    }

    json(json &js) : o_json(js)
    {
        out_ << "{";
    }

    json(json &js, string name) : o_json(js)
    {
        js.out(name, "{");
    }

    json(json &js, size_t id) : o_json(js)
    {
        js.out(id, "{");
    }
    json(jarray &ja);

    ~json()
    {
        out_ << "}";
    }

    ostring & o()
    {
        return out_;
    }
    
    template<class vT>
    void out(const string &name, const vT &value){
        if (b_first_)b_first_ = false;
        else out_ << ",";
        out_ << "\"";
        out_str(name.c_str(), name.length());
        out_ << "\":";
        out_ << "\"" << value << "\"";
    }
    
    template<class vT>
    void out(const string &name, vT *value){
        if (b_first_)b_first_ = false;
        else out_ << ",";
        out_ << "\"";
        out_str(name.c_str(), name.length());
        out_ << "\":";
        out_ << "\"" << value << "\"";
    }
    
    void out(const string &name, const string &value);
    void out(const string &name, const char *value);
    void out(size_t id, const char *value);
    void out(const string &name, const char *value, size_t val_sz);
    void out(size_t id, const char *value, size_t val_sz);
    void out(const string &name, i_base &buf);
    void out_begin(const string &name);
    void out_end(){}

    //void                          out(const string &name, dynamic::CObjectContainer *obj);
    //void                          out(const string &name, dynamic::CValue &value);
};

/**
 * @brief JSON Array.
 */
class jarray : public o_json
{
public:

    jarray(ostring &o) : o_json(o)
    {
        out_ << "[";
    }

    jarray(jarray &ja) : o_json(ja)
    {
        ja.out();
        out_ << "[";
    }

    jarray(json &js) : o_json(js)
    {
        out_ << "[";
    }

    jarray(json &js, string name) : o_json(js)
    {
        js.out(name, "[");
    }

    jarray(json &js, size_t id) : o_json(js)
    {
        js.out(id, "[");
    }

    ~jarray()
    {
        out_ << "]";
    }

    void out();
};

} // namespace stream

} // namespace bloom
