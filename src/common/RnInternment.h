/*****************************************************************************
* File: RnInternment.h
* Description:
* Author: Malcolm Hall
* Date: 5/19/22
* Version: 1
*
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

protected:
    FUNC _compare;
    std::vector<T> _items;
};

class RnConstStore {
public:
    static std::string GetInternedString(InternmentKey key);
    static double GetInternedFloat(InternmentKey key);
    static long GetInternedInt(InternmentKey key);
    static RnObject* GetInternedObject(InternmentKey key);
    static RnIntNative InternValue(RnFloatNative x);
    static RnIntNative InternValue(RnBoolNative x);
    static RnIntNative InternValue(const RnStringNative& x);
    static RnIntNative InternValue(RnIntNative x);
    static void LoadObject(RnObject* object);

    static RnInternment<RnObject*>* object_internment;
};