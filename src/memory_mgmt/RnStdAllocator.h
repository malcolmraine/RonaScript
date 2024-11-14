/*****************************************************************************
* File: RnObjectList.cpp
* Description:
* Author: Malcolm Hall
* Date: 11/9/24
* Version: 1
*
* MIT License
*
* Copyright (c) 2020 - 2024 Malcolm Hall
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

#include <iostream>
#include <numeric>
#include "RnLinearAllocator.h"

template <class T>
struct RnStdAllocator {
    typedef T value_type;

    RnStdAllocator() = default;

    template <class U>
    constexpr explicit RnStdAllocator(const RnStdAllocator<U>&) noexcept {}

    [[nodiscard]] T* allocate(std::size_t n) {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
            throw std::bad_array_new_length();
        }

        if (auto p =
                static_cast<T*>(RnLinearAllocator::Instance()->Malloc(n * sizeof(T)))) {
            //            report(p, n);
            return p;
        }

        throw std::bad_alloc();
    }

    void deallocate(T* p, std::size_t n) noexcept {
        //        report(p, n, 0);
        RnLinearAllocator::Instance()->Free(p);
    }

private:
    void report(T* p, std::size_t n, bool alloc = true) const {
        std::cout << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T) * n << " bytes at "
                  << std::hex << std::showbase << reinterpret_cast<void*>(p) << std::dec
                  << '\n';
    }
};

template<class T>
using RnStdVector = std::vector<T, RnStdAllocator<T>>;