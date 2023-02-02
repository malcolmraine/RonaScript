/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "Module.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
Module::Module() {
    node_type = AST_MODULE;
}

/*****************************************************************************/
Module::~Module() {}

/*****************************************************************************/
std::string Module::ToString(bool nl) {
    std::string s = MakeTabStr() + "Module( " + name->value + " )";
    if (nl) {
        s += "\n";
    }

    if (scope != nullptr) {
        scope->nest_lvl = nest_lvl + 1;
        s += scope->ToString(true);
    }

    return s;
}
