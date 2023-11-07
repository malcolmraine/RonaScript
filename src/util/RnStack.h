/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
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

#include <stack>
#include <vector>

template <class T>
class RnStack {
public:
    explicit RnStack() = default;

    /*************************************************************************/
    ~RnStack() {
        Clear();
    }

    /*************************************************************************/
    T Pop() {
        T item = Top();
        _data.pop_back();

        return item;
    }

    /*************************************************************************/
    void pop_back() {
        Pop();
    }

    /*************************************************************************/
    void Push(T value) {
        _data.push_back(value);
    }

    /*************************************************************************/
    void emplace_back(T value) {
        Push(value);
    }

    /*************************************************************************/
    void push_back(T value) {
        Push(value);
    }

    /*************************************************************************/
    long Size() {
        return _data.size();
    }

    /*************************************************************************/
    bool IsEmpty() {
        return _data.empty();
    }

    /*************************************************************************/
    T Top() {
        return _data.back();
    }

    /*************************************************************************/
    T back() {
        return Top();
    }

    /*************************************************************************/
    void Clear() {
        while (!_data.empty()) {
            Pop();
        }
    }

    /*************************************************************************/
    std::vector<T> GetData() {
        return _data;
    }

    /*************************************************************************/
    void Reserve(size_t n) {
        _data.reserve(n);
    }

private:
    std::vector<T> _data;
};
