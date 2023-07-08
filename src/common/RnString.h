/*****************************************************************************
* File: RnString.h
* Description:
* Author: Malcolm Hall
* Date: 6/22/23
* Version: 1
*
*****************************************************************************/

#pragma once

#include <cstdlib>
#include <cstring>
#include <string>
#include "../memory_mgmt/RnLinearAllocator.h"

class RnString
{
 public:
	RnString(){

	}

	RnString(const std::string& str) {
		size_t len = str.length();
		Resize(len + 1);
		std::memcpy(_data, str.c_str(), len);
	}

	RnString(char* cstr, size_t cnt = std::string::npos){
		if (cnt > std::strlen(cstr)) {
			throw std::runtime_error("Invalid substring length");
		}
		Resize(cnt + 1);
		std::memcpy(_data, cstr, cnt);
	}

	RnString(const char* cstr, size_t cnt = std::string::npos){
		if (cnt > std::strlen(cstr)) {
			throw std::runtime_error("Invalid substring length");
		}
		Resize(cnt + 1);
		std::memcpy(_data, cstr, cnt);
	}

	~RnString() {
		RnLinearAllocator::Instance()->Free(_data);
	}

	[[nodiscard]] const char* c_str() const {
		return const_cast<const char*>(_data);
	}

	RnString& append(const char* cstr) {
		size_t len = strlen(cstr);
		Resize(_length + len + 1);
		std::memcpy(_data + _length, cstr, len);
		return *this;
	}

	RnString operator+(RnString& str) const {
		return RnString(c_str()).append(str.c_str());
	}

	RnString substr(size_t pos = 0, size_t len = std::string::npos) const;
	size_t find(const RnString& str) const;

 private:
	void Resize(size_t n) {
		_data = static_cast<char*>(RnLinearAllocator::Instance()->Realloc(_data, n));
		_capacity = n;
		std::memset(_data + _length, 0, _capacity - _length);
	}

 private:
	char* _data = nullptr;
	size_t _length = 0;
	size_t _capacity = 0;
};

