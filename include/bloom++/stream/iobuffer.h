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

#include <bloom++/list.h>
#include <bloom++/shared_ptr.h>
#include <bloom++/stream/io.h>
#include <bloom++/string.h>

#include <list>

namespace bloom
{

namespace stream
{

#define DEFAULT_SB_ALLOC_SIZE 1024

/**
 * @brief Stream buffer.
 * 
 * A storage buffer consisting of sub-buffers(blocks).
 * Each block can be external data, new allocated data
 * or external string.
 */
class iobuffer : public io_base
{
public:
    /**
     * @brief Sub-buffer.
     */
    class ioblock
    {
        friend class iobuffer;
    public:
        ioblock(size_t ds = 0): i_line(1), i_buf_Pos(0), i_data_Size(ds) {}
        
        virtual ~ioblock(){}
        virtual char *data() = 0;
        virtual size_t size() = 0;
        size_t data_size(){
            return i_data_Size;
        }
        bool set_data_size(size_t sz){
            if(sz<=size()){
                i_data_Size = sz;
                return true;
            }
            return false;
        }
    protected:
        size_t i_line;
        size_t i_buf_Pos;
        size_t i_data_Size;
    };

    /**
     * @brief External data sub-buffer.
     */
    class ioblock_mem : public ioblock
    {
    private:
        char *sBuf_;
        size_t size_;
    public:

        ioblock_mem(char *s_buf, size_t sz) : ioblock(sz), sBuf_(s_buf), size_(sz)
        {}
        
        virtual ~ioblock_mem(){}

        virtual char *data()
        {
            return sBuf_;
        }

        virtual size_t size()
        {
            return size_;
        }
    };

    /**
     * @brief New allocation data sub-buffer.
     */
    class ioblock_new : public ioblock
    {
    private:
        char *sBuf_;
        size_t size_;
    public:

        ioblock_new(size_t sz) : sBuf_((char*)malloc(sz)), size_(sz)
        {
            if(!sBuf_)size_ = 0;
        }

        virtual ~ioblock_new()
        {
            if(sBuf_)
                free(sBuf_);
        }

        virtual char *data()
        {
            return sBuf_;
        }

        virtual size_t size()
        {
            return size_;
        }
    };

    /**
     * @brief External string sub-buffer.
     */
    class ioblock_str : public ioblock
    {
    private:
        string &sBuf_;
    public:

        ioblock_str(string &sb) : ioblock(sb.length()), sBuf_(sb)
        {
        }

        virtual char *data()
        {
            return (char *) sBuf_.c_str();
        }

        virtual size_t size()
        {
            return sBuf_.length();
        }
    };
    
private:
    /// @cond
    list<shared_ptr<ioblock> > blocks_;
    size_t sb_alloc_size_;
    /// @endcond

public:

    /**
     * @brief Constructor with defining sub-buffer allocation size.
     * 
     * @param sb_alloc_size default sub-buffer allocation size.
     */
    explicit iobuffer(size_t sb_alloc_size = DEFAULT_SB_ALLOC_SIZE):
    sb_alloc_size_(sb_alloc_size)
    {
        push_back_new(sb_alloc_size);
    }

    /**
     * @brief Constructor with external string as input sub-buffer.
     * 
     * @param buf external string.
     * @param sb_alloc_size default sub-buffer allocation size.
     */
    iobuffer(string &buf, size_t sb_alloc_size = DEFAULT_SB_ALLOC_SIZE):
    sb_alloc_size_(sb_alloc_size)
    {
        push_back_str(buf);
    }

    /**
     * @brief Constructor with external data as input sub-buffer.
     * 
     * @param buf external data.
     * @param sz external data size.
     * @param sb_alloc_size default sub-buffer allocation size.
     */
    iobuffer(char *buf, size_t sz, size_t sb_alloc_size = DEFAULT_SB_ALLOC_SIZE):
    sb_alloc_size_(sb_alloc_size)
    {
        push_back_mem(buf, sz);
    }

    virtual ~iobuffer();

    void swap(const iobuffer &b); //swap
    
    /**
     * @brief Put buffer b to back of list.
     * @param b iobuffer.
     * @return 
     */
    iobuffer &transmit_back(iobuffer &b);
    /**
     * @brief Put buffer b to front of list.
     * @param b iobuffer.
     * @return 
     */
    iobuffer &transmit_front(iobuffer &b);

    /**
     * @brieg Adding block at back of list.
     * @param block data block.
     */
    void push_back(shared_ptr<ioblock> block);
    /**
     * @brief Creating new allocated data block
     * and adding at back of list.
     * @param sz size of allocation data.
     */
    void push_back_new(size_t sz);
    /**
     * @brief Creating existing data block
     * and adding at back of list.
     * @param buf external data.
     * @param sz size of data.
     */
    void push_back_mem(char *buf, size_t sz);
    /**
     * @brief Creating external string block
     * and adding at back of list.
     * @param buf string contain data.
     */
    void push_back_str(string &buf);
    
    /**
     * @brief Adding block at front of list.
     * @param block ioblock.
     */
    void push_front(shared_ptr<ioblock> block);
    /**
     * @brief Create new allocated data block
     * and adding at front of list.
     * @param sz size of allocation data.
     */
    void push_front_new(size_t sz);
    /**
     * @brief Creating existing data block
     * and adding at front of list.
     * @param buf external data.
     * @param sz size of data.
     */
    void push_front_mem(char *buf, size_t sz);
    /**
     * @brief Creating external string block
     * and adding at front of list.
     * @param buf string contain data.
     */
    void push_front_str(string &buf);

    /**
     * @brief Getting front block of list.
     * @return front block.
     */
    shared_ptr<ioblock> front()
    {
        return blocks_.front();
    }
    
    /**
     * @brief Getting backend block of list.
     * @return backend block.
     */
    shared_ptr<ioblock> back()
    {
        return blocks_.back();
    }

    ///Out stream to abstract
    //using i_base::operator >>;
    virtual i_base & operator>>(o_base &o);
    virtual size_t read(char *data, size_t size);
    virtual bool iready();

    ///In stream from abstract
    //using o_base::operator <<;
    virtual o_base &operator<<(i_base &i);
    virtual size_t write(const char *data, size_t size);
    virtual bool oready();

    /*
    ///getting 1 byte/char
    virtual unsigned char get();
    
    ///going back for 1 byte/char
    virtual bool operator--();
    
    ///check data for read
    virtual bool empty()
    {
        return blocks_.size() == 0;
    }
     */
private:
    explicit iobuffer(const iobuffer &o); //< Forbid default constructor
};

string getStringValue(string name, string &buffer, unsigned char endSymbol);
int getIntValue(string name, string &buffer);
bool getBoolValue(string name, string &buffer);

} //namespace stream

} //namespace bloom
