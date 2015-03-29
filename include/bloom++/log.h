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

#include <fstream>
#include <bloom++/list.h>
#include <bloom++/string.h>
#include <bloom++/thread.h>
#include <bloom++/mutex.h>
#include <bloom++/condition_variable.h>
#include <bloom++/shared_ptr.h>
#include <bloom++/stream/iobuffer.h>

#include "stream/iostring.h"
#include "stream/stringer.h"

#ifdef LOG_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom {

/**
 * @brief Logger.
 */
class log
{
public:
    log();
    virtual ~log();
    
    /**
     * Open log file.
     * @param filename Filename.
     * @param max_size Max size of log (kb).
     * @return true if successful.
     */
    bool open(string filename, size_t max_size=4);
    
    log &operator<<(string iob); 
    
    template<class T> log &operator<<(const T &v){
        /// @cond
        string str;
        stream::stringer sstr(str);
        stream::ostring ios(sstr);
        ios<<v;
        return operator<<(str);
        /// @endcond
    }
    
    void clear();
    size_t queue_size();
    void close();
    
    static string pf(const char *format, ...); //printf
    
private:
    log(const log &); //Disable
    log &operator=(const log &); //Disable
    
    /// @cond
    string filename_;
    std::fstream file_;
    list<string> logs_;
    bool bStopping_;
    size_t maxSize_;
    size_t minSize_;
    thread<log> logThread_;
    mutex listMutex_;
    condition_variable cv_;
    mutex fileMutex_;
    void logThread();
    void deleteFirstLines(size_t removeSize);
    bool cv_check();
    /// @endcond
};

} //namespace bloom
