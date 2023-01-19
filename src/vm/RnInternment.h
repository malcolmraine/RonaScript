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

//	InternmentKey GetInternId(T item)
//	{
//		auto iter =  _item_key_map.find(item);
//		if (iter != _item_key_map.end())
//		{
//			return _item_key_map[item];
//		}
//		else
//		{
//			throw std::runtime_error("Item not found for internment key.");
//		}
//	}

	InternmentKey InternItem(T item)
	{
		_items.push_back(item);
		return _items.size() - 1;
//		auto iter = _item_key_map.find(item);
//		if (iter != _item_key_map.end())
//		{
//			return iter->second;
//		}
//
//		InternmentKey next_key = _last_key + 1;
//		_key_item_map.insert(std::pair<InternmentKey, T>(next_key, item));
//		_item_key_map.insert(std::pair<T, InternmentKey>(item, next_key));
//		_last_key = next_key;

//		return next_key;
	}

	T GetInternedItem(InternmentKey key)
	{
		try
		{
			return _items.at(key);
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error(
				"Key not found for interned item ( " + std::to_string(key) + " )");
		}
//		auto iter = _key_item_map.find(key);
//		if (iter != _key_item_map.end())
//		{
//			return iter->second;
//		}
//		else
//		{
//			throw std::runtime_error(
//				"Key not found for interned item ( " + std::to_string(key) + " )");
//		}
	};

 protected:
	std::vector<T> _items;
	std::unordered_map<InternmentKey, T> _key_item_map;
	std::unordered_map<T, InternmentKey> _item_key_map;
	InternmentKey _last_key = 0;
};

#endif //RONASCRIPT_RNINTERNMENT_H