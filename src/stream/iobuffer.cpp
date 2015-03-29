/*****************************************************************
  Author: Sergei "Wilder" Khairulin <sergei.khairulin@gmail.com>
 ****************************************************************/

#include <stdlib.h>
#include <string.h>
#include <bloom++/stream/iobuffer.h>
#include <bloom++/stream/iostring.h>
#include <bloom++/_bits/debug.h>

namespace bloom
{

namespace stream
{

void iobuffer::swap(const iobuffer &b)
{
    blocks_.swap(const_cast<iobuffer&> (b).blocks_);
}

iobuffer::~iobuffer()
{
}

bool iobuffer::iready()
{
    size_t bs = blocks_.size();
    if(!bs)return false;
    if(bs == 1){
        ioblock *buf = blocks_.front().get();
        if(buf->i_buf_Pos < buf->i_data_Size)
            return true;
        return false;
    }
    return true;
}

bool iobuffer::oready()
{
    return true;
}

/*
unsigned char iobuffer::get()
{
    unsigned char ch = 0;
    if (blocks_.size())
    {
        iobuffer::ioblock *buf = blocks_.front().get();

        if (buf)
        {
            if (buf->i_buf_Pos >= buf->i_data_Size)
            {
                blocks_.pop_front();
                ch = get();
            }
            else ch = buf->data()[buf->i_buf_Pos++];
        }
        else return 0;
    }
    return ch;
}
 */

/*
void iobuffer::incLineNum()
{
    iobuffer::ioblock *buf = blocks_.front().get();
    buf->i_line++;
}

void iobuffer::decLineNum()
{
    iobuffer::ioblock *buf = blocks_.front().get();
    if (buf->i_line > 1)
        buf->i_line--;
}
 

size_t iobuffer::getLineNum()
{
    iobuffer::ioblock *buf = blocks_.front().get();
    return buf->i_line;
}
 */
//

/*
bool iobuffer::operator--()
{
    if (!blocks_.size())return false;
    ioblock *buf = blocks_.front().get();

    if (buf)
    {
        if (buf->i_buf_Pos > 0)
        {
            buf->i_buf_Pos--;
            return true;
        }
        else return false;
    }
    else return false;
}
 */

//

i_base &iobuffer::operator>>(o_base &o)
{
    iobuffer *iobuf = dynamic_cast<iobuffer *> (&o);
    if (iobuf)
    {
        //DEBUG_INFO(">> transmiting ioblocks!");
        iobuf->blocks_.transmit_back(blocks_);
        return *this;
    }

    while (blocks_.size())
    {
        ioblock *buf = blocks_.front().get();
        size_t sz = write(buf->data() + buf->i_buf_Pos, buf->i_data_Size - buf->i_buf_Pos);
        if (sz < (buf->i_data_Size - buf->i_buf_Pos))
        {
            buf->i_buf_Pos += sz;
            return *this;
        }
        blocks_.pop_front();
    }
    return *this;
}

iobuffer &iobuffer::transmit_back(iobuffer &b)
{
    if (this == &b)return *this;
    blocks_.transmit_back(b.blocks_);
    return *this;
}

iobuffer &iobuffer::transmit_front(iobuffer &b)
{
    if (this == &b)return *this;
    blocks_.transmit_front(b.blocks_);
    return *this;
}

size_t iobuffer::write(const char *data, size_t size)
{
    if (!blocks_.size())
        push_back_new(sb_alloc_size_);
    ioblock *buf = blocks_.back().get();
    size_t piece_sz = 0;
    if ((buf->i_data_Size + size) > buf->size())
    {
        if (buf->i_data_Size < buf->size())
        {
            piece_sz = buf->size() - buf->i_data_Size;
            memcpy(buf->data() + buf->i_data_Size, data, piece_sz);
            buf->i_data_Size += piece_sz;
        }
        push_back_new(sb_alloc_size_);
        return write(data + piece_sz, size - piece_sz);
    }
    memcpy(buf->data() + buf->i_data_Size, data, size);
    buf->i_data_Size += size;
    return size;
}

//

size_t iobuffer::read(char *data, size_t size)
{
    if ((!blocks_.size()) || (!size))return 0;
    ioblock *buf = blocks_.front().get();
    size_t piece_sz = buf->i_data_Size - buf->i_buf_Pos;
    if (piece_sz <= size)
    {
        if(piece_sz){
            memcpy(data, buf->data() + buf->i_buf_Pos, piece_sz);
        }
        blocks_.pop_front();
        return piece_sz + read(data + piece_sz, size - piece_sz);
    }
    memcpy(data, buf->data() + buf->i_buf_Pos, size);
    buf->i_buf_Pos += size;
    return size;
}

o_base &iobuffer::operator<<(i_base &i)
{
    iobuffer *iobuf = dynamic_cast<iobuffer *> (&i);
    if (iobuf)
    {
        //DEBUG_INFO("<< transmiting ioblock!...");
        blocks_.transmit_back(iobuf->blocks_);
        return *this;
    }
    ioblock *buf = blocks_.front().get();
    if (!buf)
    {
        push_back_new(sb_alloc_size_);
        buf = blocks_.front().get();
    }
    size_t size = buf->size() - buf->i_data_Size;
    size_t piece_sz = i.read(buf->data() + buf->i_data_Size, size);
    buf->i_data_Size += piece_sz;
    if (piece_sz == size)
    {
        push_back_new(sb_alloc_size_);
        *this << i;
    }
    return *this;
}

void iobuffer::push_back(shared_ptr<ioblock> block)
{
    blocks_.push_back(block);
}

void iobuffer::push_back_mem(char *buf, size_t sz)
{
    shared_ptr<ioblock> newer(new ioblock_mem(buf, sz));
    blocks_.push_back(newer);
}


void iobuffer::push_back_new(size_t sz)
{
    shared_ptr<ioblock> newer(new ioblock_new(sz));
    blocks_.push_back(newer);
}

void iobuffer::push_back_str(string &buf)
{
    shared_ptr<ioblock> newer(new ioblock_str(buf));
    blocks_.push_back(newer);
}

void iobuffer::push_front(shared_ptr<ioblock> block)
{
    blocks_.push_front(block);
}

void iobuffer::push_front_mem(char *buf, size_t sz)
{
    shared_ptr<ioblock> newer(new ioblock_mem(buf, sz));
    blocks_.push_front(newer);
}

void iobuffer::push_front_new(size_t sz)
{
    shared_ptr<ioblock> newer(new ioblock_new(sz));
    blocks_.push_front(newer);
}

void iobuffer::push_front_str(string &buf)
{
    shared_ptr<ioblock> newer(new ioblock_str(buf));
    blocks_.push_front(newer);
}

//

string getStringValue(string name, string &buffer, unsigned char endSymbol)
{
    string value;
    iobuffer buf(buffer);
    istring str(buf);
    while (!str.empty())
    {
        string elem;
        str.read_element(elem);
        if (elem == name)
        {
            str.read_element(elem);
            if (elem == "=")
            {
                if (endSymbol == '\"')
                {
                    str.read_element(elem);
                    if (elem != "\"")
                    {
                        break;
                    }
                }
                str.read_string(endSymbol, value);
                break;
            }
            else break;
        }
    }
    return value;
}

int getIntValue(string name, string &buffer)
{
    int value = 0;
    iobuffer buf(buffer);
    istring str(buf);
    while (!str.empty())
    {
        string elem;
        str.read_element(elem);
        if (elem == name)
        {
            str.read_element(elem);
            if (elem == "=")
            {
                str >> value;
                break;
            }
            else break;
        }
    }
    return value;
}

bool getBoolValue(string name, string &buffer)
{
    bool value = 0;
    iobuffer buf(buffer);
    istring str(buf);
    while (!str.empty())
    {
        string elem;
        str.read_element(elem);
        if (elem == name)
        {
            str.read_element(elem);
            if (elem == "=")
            {
                str >> value;
                break;
            }
            else break;
        }
    }
    return value;
}

} //namespace stream

} //namespace bloom]
