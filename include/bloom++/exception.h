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
#include <exception>

namespace bloom {

/**
 * Base exception class.
 */
class exception: public std::exception
{
protected:
    string message_;
        
public:
    /**
     * Constructor.
     * @param msg Exception message.
     */
    exception(string msg);
    
    virtual ~exception() throw(){}
    virtual const char *what() throw();
};


#define BLOOM_DBGMSG(msg) fast_ostring(__FILE__,\
            ": ", __LINE__, " [", __FUNCTION__, "] :", msg)

} //namespace bloom