/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
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

#include "IntLiteral.h"
#include <iostream>
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
IntLiteral::IntLiteral() {
    node_type = AST_INT_LITERAL;
    data = 0;
}

/*****************************************************************************/
IntLiteral::IntLiteral(long value) {
    node_type = AST_INT_LITERAL;
    data = value;
}

/*****************************************************************************/
IntLiteral::~IntLiteral() = default;

/*****************************************************************************/
std::string IntLiteral::ToString(bool nl) {
    return MakeTabStr() + "IntLiteral( " + std::to_string(data) + " )" +
           (nl ? "\n" : "");
}

/*****************************************************************************/
IntLiteral::IntLiteral(bool value) {
    data = value ? 1 : 0;
}

/*****************************************************************************/
IntLiteral::IntLiteral(double value) {
    data = static_cast<RnIntNative>(value);
}

///*****************************************************************************/
//IntLiteral::IntLiteral(const std::string& value)
//{
//	std::cout << value;
//}
