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

/*****************************************************************************/
void RnBuiltins_IO::rn_builtin_file_size(RnScope* scope, const RnArrayNative& args,
                                         RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    std::string path = args.front()->ToString();
    ret_val->SetData(static_cast<RnIntNative>(std::filesystem::file_size(path)));
}

/*****************************************************************************/
void RnBuiltins_IO::rn_builtin_file_write(RnScope* scope, const RnArrayNative& args,
                                          RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
}

/*****************************************************************************/
void RnBuiltins_IO::rn_builtin_print(RnScope* scope, const RnArrayNative& args,
                                     RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
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
void RnBuiltins_IO::rn_builtin_read(RnScope* scope, const RnArrayNative& args,
                                    RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
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
void RnBuiltins_IO::rn_builtin_prompt(RnScope* scope, const RnArrayNative& args,
                                      RnObject* ret_val) {
    std::string input;
    std::cout << args.front()->ToString();
    std::cin >> input;
    ret_val->SetData(input);
}