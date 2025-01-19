/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
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

#include "BinaryExpr.h"
#include <utility>

/*****************************************************************************/
BinaryExpr::BinaryExpr() {
    node_type = AST_BINARY_EXPR;
}

/*****************************************************************************/
BinaryExpr::BinaryExpr(AstNodePtr<AstNode> left, AstNodePtr<AstNode> right,
                       std::string op) {
    SetChild(AstNode::LEFT_CHILD_INDEX, left);
    SetChild(AstNode::RIGHT_CHILD_INDEX, right);
    _op = std::move(op);
}

/*****************************************************************************/
std::string BinaryExpr::ToString(bool nl) {
    std::string s = MakeTabStr() + "BinaryExpr( " + _op + " )";
    if (nl) {
        s += "\n";
    }
    auto leftChild = GetChild(AstNode::LEFT_CHILD_INDEX);
    auto rightChild = GetChild(AstNode::RIGHT_CHILD_INDEX);
    leftChild->nest_lvl = nest_lvl + 1;
    rightChild->nest_lvl = nest_lvl + 1;
    s += leftChild->ToString(true) + rightChild->ToString(true);

    return s;
}
