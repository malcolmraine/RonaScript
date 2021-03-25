/******************************************************************************
* File:
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

#ifndef VARSTACKTEST_RNSTACK_H
#define VARSTACKTEST_RNSTACK_H

#include <vector>

template<class T>
class RnStack {
public:
    RnStack();
    ~RnStack();
    T pop();
    void push(T value);
    long size();
    bool empty();
    T top();
    T bottom();
    void clear();

    bool is_ptr_stack = false;
protected:
    void delete_all();

    std::vector<T> _data;
};

template<class T>
RnStack<T>::RnStack() {

}

template<class T>
RnStack<T>::~RnStack() {
    delete_all();
}

template<class T>
T RnStack<T>::pop() {
    T item = _data.back();
    _data.pop_back();

    return item;
}

template<class T>
void RnStack<T>::push(T value) {
    _data.template emplace_back(value);
}

template<class T>
long RnStack<T>::size() {
    return _data.size();
}

template<class T>
bool RnStack<T>::empty() {
    return _data.empty();
}

template<class T>
T RnStack<T>::top() {
    return _data.back();
}

template<class T>
T RnStack<T>::bottom() {
    if (empty()) {
        throw std::exception();
    } else {
        return _data[0];
    }
}

template<class T>
void RnStack<T>::clear() {
    delete_all();
}

template<class T>
void RnStack<T>::delete_all() {
    if (is_ptr_stack) {
        for (auto &item : _data) {
            delete item;
        }
    }
    _data.clear();
}


#endif //VARSTACKTEST_RNSTACK_H
