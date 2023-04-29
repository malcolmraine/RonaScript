/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#pragma once

#include <chrono>
#include "Time.h"

template <class DEFAULT = std::chrono::milliseconds>
class StopWatch {
public:
    /*************************************************************************/
    StopWatch() {
        ASSERT_IS_TIME_TYPE(DEFAULT);
    }

    /*************************************************************************/
    ~StopWatch() = default;

    /*************************************************************************/
    template <class T = DEFAULT>
    static size_t Time() {
        ASSERT_IS_TIME_TYPE(T)
        return Time::Now<T>();
    }

    /*************************************************************************/
    template <class T = DEFAULT>
    size_t Start() {
        ASSERT_IS_TIME_TYPE(T);
        _start = Time::Now<T>();
        return _start;
    }

    /*************************************************************************/
    template <class T = DEFAULT>
    size_t Stop() {
        ASSERT_IS_TIME_TYPE(T);
        _stop = Time::Now<T>();
        return _stop;
    }

    /*************************************************************************/
    template <class T = DEFAULT>
    [[nodiscard]] size_t Duration() const {
        ASSERT_IS_TIME_TYPE(T);
        if (_start > _stop) {
            return Time::Now<T>() - _start;
        }
        return _stop - _start;
    }

    /*************************************************************************/
    void Reset() {
        _start = 0;
        _stop = 0;
    }

    /*************************************************************************/
    void Split() {
        auto start = _start;
        auto stop = _stop;
        Start();
        _splits.push_back(stop - start);
    }

private:
    size_t _start = 0;
    size_t _stop = 0;
    std::vector<size_t> _splits;
};

#undef RONASCRIPT_SRC_UTIL_CLOCK
