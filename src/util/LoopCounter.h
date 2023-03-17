/*****************************************************************************
* File: LoopCounter.cpp
* Description:
* Author: Malcolm Hall
* Date: 3/11/23
* Version: 1
*
*****************************************************************************/

#ifndef RONASCRIPT_SRC_UTIL_LOOPCOUNTER_H_
#define RONASCRIPT_SRC_UTIL_LOOPCOUNTER_H_

#include <cstddef>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

class LoopCounter {
public:
    LoopCounter(size_t max_count, const char* file, const char* function, int line)
        : _max_count(max_count), _line(line) {
        std::stringstream file_ss;
        std::stringstream func_ss;
        file_ss << file;
        func_ss << function;
        _file = file_ss.str();
        _function = func_ss.str();
    }

    void Increment() {
        _count++;
    }

    void CheckCount() const {
        if (_count >= _max_count) {
            std::stringstream ss;
            ss << "Loop iteration max reached (" << _count << "): " << _file << ":"
               << _function << ":" << _line;
            throw std::runtime_error(ss.str());
        }
    }

private:
    size_t _count = 0;
    size_t _max_count = 0;
    std::string _file;
    int _line = -1;
    std::string _function;
};

#define ENABLE_LOOP_COUNTING
#define COUNTER_NAME counter##__LINE__
#define DEFAULT_ITERATION_MAX 100000

#ifndef MAKE_LOOP_COUNTER
#ifdef ENABLE_LOOP_COUNTING
#define MAKE_LOOP_COUNTER(max) \
    LoopCounter COUNTER_NAME(max, __FILE__, __FUNCTION__, __LINE__);
#else
#define MAKE_LOOP_COUNTER(max)
#endif
#endif

#ifndef INCR_LOOP_COUNTER
#ifdef ENABLE_LOOP_COUNTING
#define INCR_LOOP_COUNTER          \
    {                              \
        COUNTER_NAME.Increment();  \
        COUNTER_NAME.CheckCount(); \
    }
#else
#define INCR_LOOP_COUNTER
#endif
#endif

#endif  //RONASCRIPT_SRC_UTIL_LOOPCOUNTER_H_
