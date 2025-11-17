/*****************************************************************************
* File: StopWatch.h
* Description:
* Author: Malcolm Hall
* Date: 6/23/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2020 - 2023 Malcolm Hall
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
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
