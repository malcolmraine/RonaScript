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

#ifndef RONASCRIPT_RONA_FUNCTION_H
#define RONASCRIPT_RONA_FUNCTION_H

#include <vector>
#include <string>
#include "scope.h"
#include "instruction.h"
#include "rona_object.h"
#include "rona_type.h"

class Scope;
class RonaObject;

class RonaFunction {
public:
    RonaFunction();
    ~RonaFunction();

    std::vector<std::pair<RonaObject *, RonaType_t>> args;
    Scope *scope = nullptr;
    long i_start = -1;
    long i_cnt = -1;
    RonaType_t ret_type = RN_TYPE_NULL;
};


#endif //RONASCRIPT_RONA_FUNCTION_H
