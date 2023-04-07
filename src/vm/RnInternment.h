/*****************************************************************************
* File: RnInternment.h
* Description:
* Author: Malcolm Hall
* Date: 5/19/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNINTERNMENT_H
#define RONASCRIPT_RNINTERNMENT_H

#include <functional>
#include <string>
#include <unordered_map>
#include "../common/RnType.h"

typedef RnIntNative InternmentKey;

template <typename T, typename FUNC = std::function<bool(T a, T b)>>
class RnInternment {
public:
    explicit RnInternment(FUNC fn = [](T a, T b) { return a == b; }) {
        _compare = fn;
    }
    ~RnInternment() = default;

    InternmentKey InternItem(T item) {
        for (const auto& [key, value] : _key_item_map) {
            if (_compare(item, value)) {
                return key;
            }
        }
        _index++;
        _item_key_map[item] = _index;
        _key_item_map[_index] = item;

        return _index;
    }

    T GetInternedItem(InternmentKey key) {
        return _key_item_map[key];
    }

    [[nodiscard]] InternmentKey GetIndex() const {
        return _index;
    }

protected:
    FUNC _compare;
    InternmentKey _index = 0;
    std::unordered_map<InternmentKey, T> _key_item_map;
    std::unordered_map<T, InternmentKey> _item_key_map;
};

#endif  //RONASCRIPT_RNINTERNMENT_H