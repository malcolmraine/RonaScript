/*****************************************************************************
* File: RnBuiltins_IO.cpp
* Description:
* Author: Malcolm Hall
* Date: 2/1/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2020 - 2023 Malcolm Hall
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
#include <fcntl.h>
#include <unistd.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "../vm/RnClassObject.h"
#include "../vm/RnObject.h"
#include "../vm/RnScope.h"
#include "../vm/RnStringObject.h"

#undef BUILTIN_CLASS
#define BUILTIN_CLASS RnBuiltins_IO

#undef RN_BUILTIN_FUNC
#define RN_BUILTIN_FUNC RN_BUILTIN_FUNC_DEFINE

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(file_size, RnType::RN_INT, 1) {
    BUILTIN_ASSERTS
    std::string path = args.front()->ToString();
    ret_val->SetData(static_cast<RnIntNative>(std::filesystem::file_size(path)));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(file_write, RnType::RN_INT, 2){BUILTIN_ASSERTS}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(print, RnType::RN_VOID, 1) {
    std::string s;
    for (auto arg : args) {
        assert(arg);
        if (arg->GetActiveType() == RnType::RN_OBJECT) {
            auto instance = dynamic_cast<RnClassObject*>(arg);
            RnStringObject string_repr;
            if (instance->TryMagicMethod(RnClassObject::MAGIC_METHOD_KEY_STR, {arg},
                                          &string_repr)) {
                s.append(string_repr.ToString());
            }
        } else {
            s.append(arg->ToString());
        }

        if (arg != args.back()) {
            s.append(" ");
        }
    }
    printf("%s\n", s.c_str());
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(file_read, RnType::RN_STRING, 1) {
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
RN_BUILTIN_FUNC_DEFINE(prompt, RnType::RN_STRING, 1) {
    std::string input;
    std::cout << args.front()->ToString();
    std::cin >> input;
    ret_val->SetData(input);
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(_file_open, RnType::RN_INT, 1) {
    BUILTIN_ASSERTS
    auto fd =
        open(args.front()->ToString().c_str(), static_cast<int>(args[1]->ToInt()));
    ret_val->SetData(static_cast<RnIntNative>(fd));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(_file_fd_close, RnType::RN_BOOLEAN, 1) {
    BUILTIN_ASSERTS
    if (close(static_cast<int>(args[0]->ToInt())) == 0) {
        ret_val->SetData(true);
    } else {
        ret_val->SetData(false);
    }
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(_file_fd_write, RnType::RN_INT, 1) {
    BUILTIN_ASSERTS
    std::string content = args[1]->ToString();
    ssize_t result =
        write(static_cast<int>(args[0]->ToInt()), content.c_str(), content.length());
    ret_val->SetData(static_cast<RnIntNative>(result));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(_file_fd_read, RnType::RN_STRING, 1) {
    BUILTIN_ASSERTS

    auto size = static_cast<ssize_t>(args[1]->ToInt());
    if (size == 0) {
        ret_val->SetData(static_cast<RnIntNative>(0));
    } else {
        int fd = static_cast<int>(args[0]->ToInt());
        std::string content(size, 0);
        read(fd, (void*)content.c_str(), size);
        ret_val->SetData(content);
    }
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(_file_fd_truncate, RnType::RN_BOOLEAN, 1) {
    BUILTIN_ASSERTS
}