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

class RnString {
public:
    RnString() {
        Resize(1);
    }

    RnString(const std::string& str) {
        SetData(str.c_str(), str.length());
    }

    RnString(char* cstr, size_t cnt = std::string::npos) {
        if (cnt == std::string::npos) {
            SetData(cstr, strlen(cstr));
        } else
        {
            if (cnt > std::strlen(cstr))
            {
                throw std::runtime_error("Invalid substring length");
            }
            SetData(cstr, cnt);
        }
    }

    RnString(const char* cstr, size_t cnt = std::string::npos) {
        if (cnt == std::string::npos) {
            SetData(cstr, strlen(cstr));
        } else
        {
            if (cnt > std::strlen(cstr))
            {
                throw std::runtime_error("Invalid substring length");
            }
            SetData(cstr, cnt);
        }
    }

    RnString(size_t n, char c) {
        Resize(n + 1, c);
    }

    ~RnString() {
        RnLinearAllocator::Instance()->Free(_data);
    }

    [[nodiscard]] const char* c_str() const {
        return const_cast<const char*>(_data);
    }

    void SetData(const char* cstr, size_t n) {
        Resize(n + 1);
        std::memcpy(_data, cstr, n);
        _length = n;
    }

    RnString& append(const char* cstr) {
        size_t len = strlen(cstr);
        Resize(_length + len + 1);
        std::memcpy(_data + _length, cstr, len);
        return *this;
    }

    RnString& append(RnString str) {
        return append(str.c_str());
    }

    RnString& append(std::string& str) {
        return append(str.c_str());
    }

    RnString operator+(RnString& str) const {
        return RnString(c_str()).append(str.c_str());
    }

    RnString operator+(const char* str) const {
        return RnString(c_str()).append(str);
    }

    RnString operator+(std::string& str) const {
        return RnString(c_str()).append(str.c_str());
    }

    RnString& operator+=(std::string& str) {
        append(str.c_str());
        return *this;
    }

    RnString& operator=(const RnString& str) {
        if (&str != this) {
            SetData(str.c_str(), str.length());
        }
        return *this;
    }

    [[nodiscard]] size_t length() const {
        return _length;
    }

    [[nodiscard]] RnString substr(size_t pos = 0, size_t len = std::string::npos) const;
    [[nodiscard]] size_t find(const RnString& str) const;

private:
    void Resize(size_t n, char c = 0) {
        _data = static_cast<char*>(RnLinearAllocator::Instance()->Realloc(_data, n));
        _capacity = n;
        std::memset(_data + _length, c, _capacity - _length);
    }

private:
    char* _data = nullptr;
    size_t _length = 0;
    size_t _capacity = 0;
};
