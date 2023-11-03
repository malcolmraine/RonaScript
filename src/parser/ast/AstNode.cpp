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

#include "AstNode.h"
#include "../../codegen/RnCodeGenVisitor.h"

#define TAB_CHAR "  "

/*****************************************************************************/
bool AstNode::IsLiteral() const {
    return node_type == AST_FLOAT_LITERAL || node_type == AST_STRING_LITERAL ||
           node_type == AST_BOOL_LITERAL || node_type == AST_INT_LITERAL;
}

/*****************************************************************************/
std::string AstNode::ToString(bool nl) {
    return nl ? "\n" : "";
}

/*****************************************************************************/
std::string AstNode::MakeTabStr() {
    std::string outp_str;

    for (int i = 0; i < nest_lvl; ++i) {
        outp_str.append("|");
        outp_str.append(TAB_CHAR);
    }

    return outp_str;
}

/*****************************************************************************/
void AstNode::AddChild(std::shared_ptr<AstNode> child) {
    _children.push_back(child);
}