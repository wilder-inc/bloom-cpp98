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

#include <string>
#include <bloom++/string.h>

namespace bloom
{

#ifndef hash
/**
 * @brief Hash functions.
 */
template<class T> struct hash
{
public:
    size_t operator()(T const &key);
};
#endif

template <>
struct hash<std::string>
{
public:

    size_t operator()(const std::string &key)
    {
        size_t hash = 0;
        size_t x = 0;
        for (size_t i = 0; i < key.length(); i++)
        {
            hash = (hash << 4) + key[i];
            if ((x = hash & 0xF0000000L) != 0)
            {
                hash ^= (x >> 24);
                hash &= ~x;
            }
        }
        return (hash & 0x7FFFFFFF);
    }
};

template <>
struct hash<string>
{
public:

    size_t operator()(const string &key)
    {
        size_t hash = 0;
        size_t x = 0;
        for (size_t i = 0; i < key.length(); i++)
        {
            hash = (hash << 4) + key[i];
            if ((x = hash & 0xF0000000L) != 0)
            {
                hash ^= (x >> 24);
                hash &= ~x;
            }
        }
        return (hash & 0x7FFFFFFF);
    }
};

template<> struct hash<char>
{
public:

    size_t operator()(const char &k)
    {
        return (size_t) k;
    }
};

template<> struct hash<unsigned char>
{
public:

    size_t operator()(const unsigned char &k)
    {
        return (size_t) k;
    }
};

template<> struct hash<short>
{
public:

    size_t operator()(const short &k)
    {
        return (size_t) k;
    }
};

template<> struct hash<unsigned short>
{
public:

    size_t operator()(const unsigned short &k)
    {
        return (size_t) k;
    }
};

template<> struct hash<int>
{
public:

    size_t operator()(int const &k)
    {
        return (size_t) k;
    }
};

template<> struct hash<unsigned int>
{
public:

    size_t operator()(const unsigned int &k)
    {
        return (size_t) k;
    }
};

template<> struct hash<long>
{
public:

    size_t operator()(const long &k)
    {
        return (size_t) k;
    }
};

template<> struct hash<unsigned long>
{
public:

    size_t operator()(const unsigned long &k)
    {
        return (size_t) k;
    }
};

template<> struct hash<long long>
{
public:

    size_t operator()(const long long &k)
    {
        return (size_t) k;
    }
};

template<> struct hash<void *>
{
public:

    size_t operator()(const void * &k)
    {
        return ((size_t) k >> 8) ^ ((size_t) k >> 16);
    }
};

} //namespace bloom