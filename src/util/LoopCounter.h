//
// Created by Malcolm Hall on 3/11/23.
//

#ifndef RONASCRIPT_SRC_UTIL_LOOPCOUNTER_H_
#define RONASCRIPT_SRC_UTIL_LOOPCOUNTER_H_

#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string>
#include <sstream>

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
            ss << "Loop iteration max reached ("
               << _count << "): " << _file
               << ":" << _function << ":" << _line;
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

#ifndef MAKE_LOOP_COUNTER
#define MAKE_LOOP_COUNTER(max) \
    LoopCounter counter(max, __FILE__, __FUNCTION__, __LINE__);
#endif

#ifndef INCR_LOOP_COUNTER
#define INCR_LOOP_COUNTER       \
    {                           \
        counter.Increment();    \
        counter.CheckCount();   \
    }
#endif

#endif  //RONASCRIPT_SRC_UTIL_LOOPCOUNTER_H_
