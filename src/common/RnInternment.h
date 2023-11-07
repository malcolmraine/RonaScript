/*****************************************************************************
* File: RnInternment.h
* Description:
* Author: Malcolm Hall
* Date: 5/19/22
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

#include <functional>
#include <string>
#include <unordered_map>
#include "RnType.h"

class RnObject;

typedef RnIntNative InternmentKey;

template <typename T, typename FUNC = std::function<bool(T a, T b)>>
class RnInternment {
public:
    explicit RnInternment(FUNC fn = [](T a, T b) { return a == b; }) {
        _compare = fn;
    }
    ~RnInternment() = default;

    InternmentKey InternItem(T item) {
        for (size_t i = 0; i < static_cast<InternmentKey>(_items.size()); i++) {
            if (_compare(item, _items[i])) {
                return i;
            }
        }
        _items.push_back(item);
        return _items.size() - 1;
    }

    T GetInternedItem(InternmentKey key) {
        return _items[key];
    }

    [[nodiscard]] InternmentKey GetIndex() const {
        return _items.size() - 1;
    }

    // This is used for loading the internment from a binary file
    void LoadObject(RnObject* object) {
        _items.push_back(object);
    }

    void Init(size_t size) {
        _items.reserve(size);
    }

protected:
    FUNC _compare;
    std::vector<T> _items;
};

class RnConstStore {
public:
    static RnStringNative GetInternedString(InternmentKey key);
    static double GetInternedFloat(InternmentKey key);
    static long GetInternedInt(InternmentKey key);
    static RnObject* GetInternedObject(InternmentKey key);
    static RnIntNative InternValue(RnFloatNative x);
    static RnIntNative InternValue(RnBoolNative x);
    static RnIntNative InternValue(const RnStringNative& x);
    static RnIntNative InternValue(RnIntNative x);
    static void LoadObject(RnObject* object);
    static void Init(size_t size);
    static RnInternment<RnObject*>* object_internment;
};