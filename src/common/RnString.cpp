/*****************************************************************************
* File: RnString.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/22/23
* Version: 1
*
*****************************************************************************/

#include "RnString.h"

/*****************************************************************************/
RnString RnString::substr(size_t pos, size_t len) const {
	if (pos + len > _length) {
		throw std::runtime_error("Out of bounds substring");
	}
	return {_data + pos, len};
}

/*****************************************************************************/
size_t RnString::find(const RnString& str) const {
	// Simple hack to get this working while a full implementation is done
	std::string s(_data);
	return s.find(str.c_str());
}
