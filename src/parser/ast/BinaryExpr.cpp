/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "BinaryExpr.h"
#include <iostream>
#include <utility>
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
BinaryExpr::BinaryExpr() {
    node_type = AST_BINARY_EXPR;
}

/*****************************************************************************/
BinaryExpr::BinaryExpr(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right,
                       std::string op) {
    _left = std::move(left);
    _right = std::move(right);
    _op = std::move(op);
}

/*****************************************************************************/
std::string BinaryExpr::ToString(bool nl) {
    std::string s = MakeTabStr() + "BinaryExpr( " + _op + " )";
    if (nl) {
        s += "\n";
    }
    _left->nest_lvl = nest_lvl + 1;
    _right->nest_lvl = nest_lvl + 1;
    s += _left->ToString(true) + _right->ToString(true);

    return s;
}
