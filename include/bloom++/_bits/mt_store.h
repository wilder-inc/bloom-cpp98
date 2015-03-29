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

#include <bloom++/mutex.h>
#include <iostream>
#include <assert.h>

namespace bloom
{

class mt_store
{
protected:
    mutable mutex m_;
    
private:
    mutable unsigned int it_count_;
    
public:
    mt_store():m_(mutex::errorcheck), it_count_(0){}
    void incIterator() const{
        ++it_count_;
        //std::cout<<"mt_store "<<(unsigned long)this<<": incIterator, count "<<it_count_<<"\n";
    }
    void decIterator() const{
        --it_count_;
        //std::cout<<"mt_store "<<(unsigned long)this<<": decIterator, count "<<it_count_<<"\n";
        assert(it_count_ != -1);
        if(!it_count_)
            m_.unlock();
    }
};

} //namespace bloom