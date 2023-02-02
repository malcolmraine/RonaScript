/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "UnaryExpr.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
UnaryExpr::UnaryExpr() {
    node_type = AST_UNARY_EXPR;
}

/*****************************************************************************/
UnaryExpr::~UnaryExpr() = default;

/*****************************************************************************/
std::string UnaryExpr::ToString(bool nl) {
    std::string s = MakeTabStr() + "UnaryExpr( " + op + " )";
    if (nl) {
        s += "\n";
    }
    expr->nest_lvl = nest_lvl + 1;
    s += expr->ToString(nl);

    return s;
}
