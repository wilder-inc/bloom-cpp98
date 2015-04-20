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

#include <bloom++/_bits/vector_t.h>

#ifdef AUX_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

/**
 * @brief Like std::vector.
 * 
 */
template<class vT, class Traits=traits<vT> >
class vector: public vector_t<vT, Traits>
{
public:
    typedef class vector<vT, Traits>            Self;
    typedef class vector_t<vT, Traits>          base_vector;
    typedef vT *                                iterator;
    typedef const vT *                          const_iterator;
    
    vector() throw(){}
    
    vector(size_t size, const vT &v = vT()) throw():
    base_vector(size, v){}
    
    vector(const Self &vec) throw():
    base_vector(vec){}
    
    ~vector() throw(){}
    
    Self &operator=(const Self &vec){
        /// @cond
        base_vector::assign(vec);
        return *this;
        /// @endcond
    }
    
    Self& append(const vT &c){
        /// @cond
        base_vector::push_back(c);
        return *this;
        /// @endcond
    }
    
    Self& append(const vT *vec, size_t size){
        /// @cond
        base_vector::append(vec, size);
        return *this;
        /// @endcond
    }
    
    Self& append(const Self &vec){
        /// @cond
        base_vector::append(vec);
        return *this;
        /// @endcond
    }
    
    void insert(size_t index, const vT &c) {
        /// @cond
        base_vector::insert(index, &c, 1);
        /// @endcond
    }
    
    iterator insert(iterator it, const vT &c) {
        /// @cond
        base_vector::insert(it - begin(), &c, 1);
        return ++it;
        /// @endcond
    }
    
    iterator insert(iterator it, const vT *values, size_t size) {
        /// @cond
        base_vector::insert(it - begin(), values, size);
        return it+=size;
        /// @endcond
    }
    
    iterator insert(iterator it, const Self &vec) {
        /// @cond
        base_vector::insert(it - begin(), vec);
        return it+=vec.size();
        /// @endcond
    }
    
    void replace(size_t index, const vT *values, size_t size) {
        /// @cond
        base_vector::replace(index, values, size);
        /// @endcond
    }
    
    void replace(size_t index, const Self &vec) {
        /// @cond
        base_vector::replace(index, vec);
        /// @endcond
    }
    
    iterator replace(iterator it, const vT *values, size_t size) {
        /// @cond
        base_vector::replace(it - begin(), values, size);
        return it+=size;
        /// @endcond
    }
    
    iterator replace(iterator it, const Self &vec) {
        /// @cond
        base_vector::replace(it - begin(), vec);
        return it+=vec.size();
        /// @endcond
    }
    
    void assign(size_t index, size_t size, const vT &c) {
        /// @cond
        base_vector::assign(index, size, c);
        /// @endcond
    }
    
    iterator assign(iterator it, size_t size, const vT &c) {
        /// @cond
        assign(it - begin(), size, c);
        return it+=size;
        /// @endcond
    }
    
    vT &operator[](size_t index){
        /// @cond
        return base_vector::operator[](index);
        /// @endcond
    }
    
    const vT &operator[](size_t index) const {
        /// @cond
        return base_vector::operator[](index);
        /// @endcond
    }
    
    Self &operator+=(const Self &vec){
        /// @cond
        base_vector::append(vec);
        return *this;
        /// @endcond
    }
    
    Self operator+(const Self &vec){
        /// @cond
        Self v(*this);
        v.append(vec);
        return v;
        /// @endcond
    }
    
    Self &operator+=(const vT &elem){
        /// @cond
        push_back(elem);
        return *this;
        /// @endcond
    }
    
    Self operator+(const vT &elem){
        /// @cond
        Self v(*this);
        v.push_back(elem);
        return v;
        /// @endcond
    }
    
    void resize(size_t size, vT c = vT()){
        /// @cond
        base_vector::resize(size, c);
        /// @endcond
    }
    
    iterator begin(){
        /// @cond
        return iterator(base_vector::begin());
        /// @endcond
    }
    
    const_iterator begin() const{
        /// @cond
        return const_iterator(base_vector::begin());
        /// @endcond
    }
    
    iterator end(){
        /// @cond
        return iterator(base_vector::end());
        /// @endcond
    }
    
    const_iterator end() const{
        /// @cond
        return const_iterator(base_vector::end());
        /// @endcond
    }
    
    using base_vector::push_back;
    using base_vector::pop_back;
    using base_vector::erase;
    using base_vector::clear;
    using base_vector::data;
    using base_vector::size;
    using base_vector::capacity;
    using base_vector::swap;
}; //class vector

} //namespace bloom