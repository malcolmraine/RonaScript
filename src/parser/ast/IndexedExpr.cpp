/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "IndexedExpr.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
IndexedExpr::IndexedExpr() {
    node_type = AST_INDEXED_EXPR;
}

/*****************************************************************************/
IndexedExpr::~IndexedExpr() {
    //delete idx;
}

/*****************************************************************************/
std::string IndexedExpr::ToString(bool nl) {
    std::string s = MakeTabStr() + "IndexedExpr( )";
    if (nl) {
        s += "\n";
    }
    idx->nest_lvl = nest_lvl + 1;
    expr->nest_lvl = nest_lvl + 1;
    s += expr->ToString(nl) + idx->ToString(nl);

    return s;
}
