/*****************************************************************************
* File: LiteralValue.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/26/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2023 Malcolm Hall
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

#include "LiteralValue.h"

/*****************************************************************************/
LiteralValue::LiteralValue() = default;

/*****************************************************************************/
LiteralValue::~LiteralValue() = default;

/*****************************************************************************/
std::string LiteralValue::ToString(bool nl) {
    std::string s = MakeTabStr() + "LiteralValue( ";
    switch (node_type) {
        case AST_FLOAT_LITERAL:
            s += "float, " + std::to_string(std::get<RnFloatNative>(data)) + " )";
            break;
        case AST_INT_LITERAL:
            s += "int, " + std::to_string(std::get<RnIntNative>(data)) + " )";
            break;
        case AST_BOOL_LITERAL:
            s += "bool, " +
                 std::string((std::get<RnBoolNative>(data) ? "true" : "false")) + " )";
            break;
        case AST_STRING_LITERAL:
            s += "string, " + std::get<RnStringNative>(data) + " )";
            break;
        case AST_NULL_LITERAL:
            s += "null )";
            break;
        default:
            assert(false);
    }
    if (nl) {
        s += "\n";
    }
    return s;
}