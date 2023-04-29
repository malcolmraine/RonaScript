/******************************************************************************
* File: PtrStack.h
* Description:
* Author: Malcolm Hall
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
*******************************************************************************/

#pragma once

#include <cstdlib>

template <typename T>
class PtrStack {
public:
    explicit PtrStack(size_t reserve = 10) {
        Resize(reserve);
    }

    ~PtrStack() {
        std::free(_data);
        _data = nullptr;
    }

    void Push(T item) {
        assert(item);
        if (_size == _max_size)
            Resize(2 * _max_size);

        if (_size == 0)
            *_stack_ptr = item;
        else
            *(++_stack_ptr) = item;
        _size++;
    }

    T Pop() {
        assert(_size > 0);
        assert(_stack_ptr);
        auto item = *(_stack_ptr--);
        assert(item);
        _size--;
        return item;
    }

    T Top() {
        assert(_size > 0);
        return *_stack_ptr;
    }

    size_t Size() {
        return _size;
    }

    void Clear() {
        _stack_ptr = _data;
        _size = 0;
    }

    void push_back(T item) {
        Push(item);
    }

    T pop_back() {
        return Pop();
    }

    T back() {
        return Top();
    }

    void Resize(size_t new_size) {
        _data = static_cast<T*>(std::realloc(_data, sizeof(T) * new_size));
        _max_size = new_size;
        _stack_ptr = _data + _size;
    }

private:
    T* _data = nullptr;
    size_t _size = 0;
    size_t _max_size = 0;
    T* _stack_ptr = nullptr;
};

