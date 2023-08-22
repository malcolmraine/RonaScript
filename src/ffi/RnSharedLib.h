/*****************************************************************************
* File: RnSharedLib.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/23/22
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
******************************************************************************/

#pragma once

#include <string>
#include <vector>
#include "../vm/RnArrayObject.h"
#include "../vm/RnBoolObject.h"
#include "../vm/RnFloatObject.h"
#include "../vm/RnFunctionObject.h"
#include "../vm/RnIntObject.h"
#include "../vm/RnNullObject.h"
#include "../vm/RnObject.h"
#include "../vm/RnStringObject.h"

#if defined(_WIN32)
#define LIBRARY_API __declspec(dllexport)
#else
#define LIBRARY_API __attribute__((visibility("default")))
#endif

#define RONA_EXPORT extern "C" LIBRARY_API

RONA_EXPORT const char* LibraryName();
RONA_EXPORT const char* LibraryVersion();
RONA_EXPORT void LibraryFunctions(
    std::vector<std::tuple<std::string, RnType::Type>>& functions);

#define DEFINE_LIBRARY(name, version) \
    const char* LibraryName() {       \
        return #name;                 \
    }                                 \
    const char* LibraryVersion() {    \
        return #version;              \
    }                                 \
    void LibraryFunctions(std::vector<std::tuple<std::string, RnType::Type>>& functions)

#define REGISTER_LIBRARY_FUNCTION(name, type) functions.emplace_back(#name, type);
