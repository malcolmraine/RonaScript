/******************************************************************************
* File:
* Description:
* Author: Malcolm Hall
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
*******************************************************************************/

#ifndef VARSTACKTEST_LOG_H
#define VARSTACKTEST_LOG_H

#include <iostream>
#include <string>
#include <vector>
#include "../vm/rona_object.h"
#include "../vm/rona_class.h"
#include "../vm/rona_function.h"
#include "../vm/instruction.h"

#define ENABLE_DEBUG_LOGGING
#define ENABLE_INFO_LOGGING
#define ENABLE_ERROR_LOGGING

class Log {
public:
    static void INFO(const std::string &msg);
    static void INFO(int value);
    static void INFO(double value);
    static void INFO(bool value);
    static void DEBUG(const std::string &msg);
    static void DEBUG(int value);
    static void DEBUG(double value);
    static void DEBUG(bool value);
    static void ERROR(const std::string &msg);

};


#endif //VARSTACKTEST_LOG_H
