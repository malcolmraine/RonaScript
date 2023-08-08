/*****************************************************************************
* File: Time.h
* Description:
* Author: Malcolm Hall
* Date: 6/23/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2021 Malcolm Hall
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
#include <type_traits>

#ifndef ASSERT_IS_TIME_TYPE
#define ASSERT_IS_TIME_TYPE(T)                                        \
    static_assert(std::is_same_v<T, std::chrono::hours> ||            \
                      std::is_same_v<T, std::chrono::minutes> ||      \
                      std::is_same_v<T, std::chrono::seconds> ||      \
                      std::is_same_v<T, std::chrono::milliseconds> || \
                      std::is_same_v<T, std::chrono::microseconds> || \
                      std::is_same_v<T, std::chrono::nanoseconds>,    \
                  "Type should be an std::chrono time type.");
#endif

#define NS_PER_MS (1000000)
#define MS_PER_SECOND (1000)
#define SECONDS_PER_MINUTE (60)
#define MINUTES_PER_HOUR (60)

#ifndef RN_TIME_CLOCK
#define RN_TIME_CLOCK std::chrono::high_resolution_clock
#endif

/*****************************************************************************/
class Time {
public:
    /*************************************************************************/
    template <class T = std::chrono::milliseconds>
    static size_t Now() {
        ASSERT_IS_TIME_TYPE(T)
        auto duration = RN_TIME_CLOCK::now().time_since_epoch();
        return std::chrono::duration_cast<T>(duration).count();
    }

    /*************************************************************************/
    static size_t Seconds() {
        return Now<std::chrono::seconds>();
    }

    /*************************************************************************/
    static size_t Milliseconds() {
        return Now<std::chrono::milliseconds>();
    }

    /*************************************************************************/
    static size_t Microseconds() {
        return Now<std::chrono::microseconds>();
    }

    /*************************************************************************/
    static size_t Nanoseconds() {
        return Now<std::chrono::nanoseconds>();
    }

    /*************************************************************************/
    static std::string Stamp() {
        return "";
    }
};
