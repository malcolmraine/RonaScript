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

#include <string>
#include <unordered_map>

typedef size_t InternmentKey;

template<typename T>
class RnInternment
{
 public:
	RnInternment() = default;
	~RnInternment() = default;

	InternmentKey InternItem(T item)
	{
		if (_item_key_map.contains(item)) {
			return _item_key_map[item];
		}
		_index++;
		_item_key_map[item] = _index;
		_key_item_map[_index] = item;

		return _index;
	}

	T GetInternedItem(InternmentKey key)
	{
		return _key_item_map[key];
	}

 protected:
	size_t _index = 0;
	std::unordered_map<InternmentKey, T> _key_item_map;
	std::unordered_map<T, InternmentKey> _item_key_map;
};

#endif //RONASCRIPT_RNINTERNMENT_H