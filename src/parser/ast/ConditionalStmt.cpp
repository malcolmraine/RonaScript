/*****************************************************************************
* File: ConditionalStmt.cpp
* Description:
* Author: Malcolm Hall
* Date:
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

#include "ConditionalStmt.h"

/*****************************************************************************/
ConditionalStmt::ConditionalStmt() = default;

/*****************************************************************************/
ConditionalStmt::~ConditionalStmt() = default;

/*****************************************************************************/
std::string ConditionalStmt::ToString(bool nl) {
    std::string newline = nl ? "\n" : "";
    std::string output;
    if (node_type == AST_IF_STMT) {
        output += MakeTabStr() + "IfStmt( )" + newline;
    } else if (node_type == AST_ELIF_STMT) {
        output += MakeTabStr() + "ElifStmt( )" + newline;
    } else {
        output += MakeTabStr() + "ElseStmt( )" + newline;
    }

    if (test) {
        test->nest_lvl = nest_lvl + 1;
        output += test->ToString(true);
    }

    nest_lvl++;
    output += MakeTabStr() + "Consequent( )" + newline;
    consequent->nest_lvl = nest_lvl + 1;
    output += consequent->ToString(true);

    if (alternative) {
        output += MakeTabStr() + "Alternative( )" + newline;
        alternative->nest_lvl = nest_lvl + 1;
        output += alternative->ToString(true);
    }
    nest_lvl--;

    return output;
}
