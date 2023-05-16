/*****************************************************************************
* File: RnBuiltins_IO.cpp
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

#include "RnBuiltins_IO.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include "../vm/RnObject.h"
#include "../vm/RnScope.h"

#undef BUILTIN_CLASS
#define BUILTIN_CLASS RnBuiltins_IO

#undef RN_BUILTIN_FUNC
#define RN_BUILTIN_FUNC RN_BUILTIN_FUNC_DEFINE

/*****************************************************************************/
RN_BUILTIN_FUNC(RnBuiltins_IO, file_size, RnType::RN_INT, 1)    {
    BUILTIN_ASSERTS
    std::string path = args.front()->ToString();
    ret_val->SetData(static_cast<RnIntNative>(std::filesystem::file_size(path)));
}

/*****************************************************************************/
RN_BUILTIN_FUNC(RnBuiltins_IO, file_write, RnType::RN_INT, 2)   {
    BUILTIN_ASSERTS
}

/*****************************************************************************/
RN_BUILTIN_FUNC(RnBuiltins_IO, print, RnType::RN_VOID, 1)        {
    std::string s;
    for (auto arg : args) {
        assert(arg);
        s.append(arg->ToString());
        if (arg != args.back()) {
            s.append(" ");
        }
    }
    printf("%s\n", s.c_str());
}

/*****************************************************************************/
RN_BUILTIN_FUNC(RnBuiltins_IO, file_read, RnType::RN_STRING, 1) {
    BUILTIN_ASSERTS
    std::ifstream file;
    file.open(args.front()->ToString());

    std::string contents;
    file.seekg(0, std::ios::end);
    contents.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    contents.assign((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    file.close();
    ret_val->SetData(contents);
}

/*****************************************************************************/
RN_BUILTIN_FUNC(RnBuiltins_IO, prompt, RnType::RN_STRING, 1) {
    std::string input;
    std::cout << args.front()->ToString();
    std::cin >> input;
    ret_val->SetData(input);
}