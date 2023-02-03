/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "AttributeAccess.h"
#include <iostream>
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
AttributeAccess::AttributeAccess() {
    node_type = AST_BINARY_EXPR;
}

/*****************************************************************************/
AttributeAccess::~AttributeAccess() = default;

/*****************************************************************************/
std::string AttributeAccess::ToString(bool nl) {
    std::string s = MakeTabStr() + "AttributeAccess( )";
    if (nl) {
        s += "\n";
    }
    _parent->nest_lvl = nest_lvl + 1;
    _name->nest_lvl = nest_lvl + 1;
    s += _parent->ToString(nl) + _name->ToString(nl);

    return s;
}
