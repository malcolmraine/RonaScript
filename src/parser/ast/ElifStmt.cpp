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

#include "ElifStmt.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
ElifStmt::ElifStmt() {
    node_type = AST_ELIF_STMT;
}

/*****************************************************************************/
ElifStmt::~ElifStmt() = default;

/*****************************************************************************/
std::string ElifStmt::ToString(bool nl) {
    std::string newline = nl ? "\n" : "";
    std::string output = MakeTabStr() + "ElifStmt( )" + newline;
    output += MakeTabStr() + "\tTest( )" + newline;
    test->nest_lvl = nest_lvl + 2;
    output += test->ToString(true);

    output += MakeTabStr() + "\tConsequent( )" + newline;
    consequent->nest_lvl = nest_lvl + 2;
    output += consequent->ToString(true);

    if (alternative != nullptr) {
        output += MakeTabStr() + "\tAlternative( )" + newline;
        alternative->nest_lvl = nest_lvl + 2;
        output += alternative->ToString(true);
    }

    return output;
}
