/*****************************************************************
  Author: Sergei "Wilder" Khairulin <sergei.khairulin@gmail.com>
 ****************************************************************/

#include <string.h>
#include <bloom++/stream/json.h>
#include <bloom++/stream/iostring.h>
//#include "dynamic/types.h"

namespace bloom {

namespace stream {

json::json(jarray &ja) : o_json(ja)
{
    ja.out();
    out_ << "{";
}

void json::out_str(const char *data, size_t size)
{
    char *d_to = (char *) memchr(data, '\"', size);
    if (d_to)
    {
        out_.write((char *) data, d_to - data);
        out_ << "\\\"";
        d_to++;
        out_str(d_to, size - (d_to - data));
    }
    else
    {
        out_.write((char *) data, size);
    }
}

void json::out(const string &name, const string &value)
{
    out(name, value.c_str(), value.length());
}

void json::out(const string &name, const char *value)
{
    out(name, value, strlen(value));
}

void json::out(size_t id, const char *value)
{
    out(id, value, strlen(value));
}

void json::out(const string &name, const char *value, size_t val_sz)
{
    if (b_first_)b_first_ = false;
    else out_ << ",";
    out_ << "\"";
    out_str(name.c_str(), name.length());
    out_ << "\":";
    if ((!val_sz) || ((value[0] != '{')&&(value[0] != '[')))
    {
        out_ << "\"";
        out_str(value, val_sz);
        out_ << "\"";
    }
    else
    {
        out_.write((char *) value, val_sz);
    }
}

void json::out(size_t id, const char *value, size_t val_sz)
{
    if (b_first_)b_first_ = false;
    else out_ << ",";
    out_ << "\"" << id << "\":";
    if ((!val_sz) || ((value[0] != '{')&&(value[0] != '[')))
    {
        out_ << "\"";
        out_str(value, val_sz);
        out_ << "\"";
    }
    else
    {
        out_.write((char *) value, val_sz);
    }
}

void json::out(const string &name, i_base &buf)
{
    if (b_first_)b_first_ = false;
    else out_ << ",";
    out_ << "\"";
    out_str(name.c_str(), name.length());
    out_ << "\":";
    out_ << buf;
}

void json::out_begin(const string &name){
        if (b_first_)b_first_ = false;
        else out_ << ",";
        out_ << "\"";
        out_str(name.c_str(), name.length());
        out_ << "\":";
}

//-------------------------
// for dynamic::types
//-------------------------
/*
void json::out(const string &name, dynamic::CObjectContainer *obj)
{
  if(b_first_)b_first_=false;
  else out_ << ",";
  out_ << "\"";
  out_str(name.c_str(), name.length());
  out_ << "\":";
  obj->toJson(out_);
}

void json::out(const string &name, dynamic::CValue &value)
{
  if(b_first_)b_first_=false;
  else out_ << ",";
  out_ << "\"";
  out_str(name.c_str(), name.length());
  out_ << "\":";
  //if(value.isString()){
      out_<< "\"";
      out_str(value.toString());
      out_<< "\"";
  //  }
  //else {
  //    out_<< value.toString();
  //  }
}
 */
//----------------

void jarray::out()
{
    if (b_first_)b_first_ = false;
    else out_ << ",";
}


} //namespace sstream

} //namespace bloom
