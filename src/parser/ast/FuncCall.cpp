/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "FuncCall.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
FuncCall::FuncCall() {
    node_type = AST_FUNC_CALL;
}

/*****************************************************************************/
FuncCall::~FuncCall() {
    for (auto& arg : args) {
        arg.reset();
    }
}

/*****************************************************************************/
std::string FuncCall::ToString(bool nl) {
    std::string s = MakeTabStr() + "FuncCall( )";
    if (nl) {
        s += "\n";
    }
    expr->nest_lvl = nest_lvl + 1;
    s += expr->ToString(true);

    for (auto& arg : args) {
        arg->nest_lvl = nest_lvl + 1;
        s += arg->ToString(true);
    }

    return s;
}
