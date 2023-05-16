/*****************************************************************************
* File: RnBuiltins_String.cpp
* Description:
* Author: Malcolm Hall
* Date: 2/1/22
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

#include "RnBuiltins_String.h"
#include "../util/String.h"
#include "../vm/RnObject.h"
#include "../vm/RnScope.h"


#undef BUILTIN_CLASS
#define BUILTIN_CLASS RnBuiltins_String

#undef RN_BUILTIN_FUNC
#define RN_BUILTIN_FUNC RN_BUILTIN_FUNC_DEFINE

/*****************************************************************************/
RN_BUILTIN_FUNC(RnBuiltins_String, str_titlecase, RnType::RN_STRING, 1)  {
    BUILTIN_ASSERTS
    ret_val->SetData(String::TitleCase(args.front()->ToString()));
}

/*****************************************************************************/
RN_BUILTIN_FUNC(RnBuiltins_String, str_lower, RnType::RN_STRING, 1)    {
    BUILTIN_ASSERTS
    ret_val->SetData(String::Lower(args.front()->ToString()));
}

/*****************************************************************************/
RN_BUILTIN_FUNC(RnBuiltins_String, str_upper, RnType::RN_STRING, 1)       {
    BUILTIN_ASSERTS
    ret_val->SetData(String::Upper(args.front()->ToString()));
}

/*****************************************************************************/
RN_BUILTIN_FUNC(RnBuiltins_String, str_snakecase, RnType::RN_STRING, 1)  {
    BUILTIN_ASSERTS
    ret_val->SetData(String::SnakeCase(args.front()->ToString()));
}

/*****************************************************************************/
RN_BUILTIN_FUNC(RnBuiltins_String, str_split, RnType::RN_ARRAY, 1)   {
    BUILTIN_ASSERTS

    RnArrayNative result;

    if (args.size() == 1 || args.size() == 2 && args[1]->ToString().empty()) {
        auto original_string = args[0]->ToString();
        result.reserve(original_string.size());
        for (char c : original_string) {
            result.push_back(RnObject::Create(std::string(1, c)));
        }
    } else {
        auto strings = String::Split(args[0]->ToString(), args[1]->ToString());
        result.reserve(strings.size());
        for (const auto& s : strings) {
            result.push_back(RnObject::Create(s));
        }
    }
    ret_val->SetData(result);
}

/*****************************************************************************/
RN_BUILTIN_FUNC(RnBuiltins_String, str_substr, RnType::RN_STRING, 1)    {
    BUILTIN_ASSERTS

    assert(args.size() == 2 || args.size() == 3);
    // original string, position, count
    if (args.size() == 2) {
        ret_val->SetData(args[0]->ToString().substr(args[1]->ToInt()));
    } else if (args.size() == 3) {
        ret_val->SetData(
            args[0]->ToString().substr(args[1]->ToInt(), args[2]->ToInt()));
    }
}

/*****************************************************************************/
RN_BUILTIN_FUNC(RnBuiltins_String, str_startswith, RnType::RN_BOOLEAN, 1) {
    BUILTIN_ASSERTS

    assert(args.size() == 2);

    // subject, search
    ret_val->SetData(static_cast<bool>(
        String::StartsWith(args.front()->ToString(), args.back()->ToString())));
}

/*****************************************************************************/
RN_BUILTIN_FUNC(RnBuiltins_String, str_endswith, RnType::RN_BOOLEAN, 1)    {
    BUILTIN_ASSERTS

    assert(args.size() == 2);

    // subject, search
    ret_val->SetData(static_cast<bool>(
        String::EndsWith(args.front()->ToString(), args.back()->ToString())));
}

/*****************************************************************************/
RN_BUILTIN_FUNC(RnBuiltins_String, str_join, RnType::RN_STRING, 1) {
    BUILTIN_ASSERTS

    assert(args.size() == 1 || args.size() == 2 || args.size() == 3);
    std::string result;
    auto strings = args[0]->ToArray();
    std::string join_str;

    if (args.size() == 2) {
        join_str = args[1]->ToString();
    }

    if (strings.empty()) {
        ret_val->SetData(std::string());
    } else if (strings.size() == 1) {
        ret_val->SetData(strings[0]->ToString());
    } else {
        for (size_t i = 0; i < strings.size() - 1; ++i) {
            result.append(strings[i]->ToString());
            result.append(join_str);
        }

        if (strings.size() > 1) {
            result.append(strings.back()->ToString());
        }
        ret_val->SetData(result);
    }
}
