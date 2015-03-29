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

template<class vT, class rvT>
struct vector_iterator_t
{
    /// @cond
    vT *current_;
    /// @endcond

    vector_iterator_t():current_(){}
    vector_iterator_t(vT *p): current_(p){}
    vector_iterator_t(const vector_iterator_t &it):current_(it.current_){}
    
    vector_iterator_t operator+(size_t step){
        /// @cond
        return vector_iterator_t(current_ + step);
        /// @endcond
    }
    vector_iterator_t &operator+=(size_t step){
        /// @cond
        current_+=step;
        return *this;
        /// @endcond
    }
    vector_iterator_t operator-(size_t step){
        /// @cond
        return vector_iterator_t(current_ - step);
        /// @endcond
    }
    size_t operator-(const vector_iterator_t &it){
        return current_ - it.current_;
    }
    vector_iterator_t &operator-=(size_t step){
        /// @cond
        current_-=step;
        return *this;
        /// @endcond
    }
    bool operator==(const vector_iterator_t &it) const{
        /// @cond
        if(current_ == it.current_)return true;
        return false;
        /// @endcond
    }
    bool operator!=(const vector_iterator_t &it) const{
        /// @cond
        if(current_ != it.current_)return true;
        return false;
        /// @endcond
    }
    rvT &operator *(){
        /// @cond
        return *current_;
        /// @endcond
    }
    vector_iterator_t &operator++(){
        /// @cond
        ++current_;
        return *this;
        /// @endcond
    }
    vector_iterator_t operator++(int){
        /// @cond
        vector_iterator_t it(*this);
        ++current_;
        return it;
        /// @endcond
    }
    vector_iterator_t &operator--(){
        /// @cond
        --current_;
        return *this;
        /// @endcond
    }
    vector_iterator_t operator--(int){
        /// @cond
        vector_iterator_t it(*this);
        --current_;
        return it;
        /// @endcond
    }
};