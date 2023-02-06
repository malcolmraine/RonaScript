/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "AliasDecl.h"
#include "../../codegen/RnCodeGenVisitor.h"
#include "Name.h"

/*****************************************************************************/
AliasDecl::AliasDecl() {
    node_type = AST_ALIAS_DECL;
}

/*****************************************************************************/
AliasDecl::~AliasDecl() = default;

/*****************************************************************************/
std::string AliasDecl::ToString(bool nl) {
    std::string s = MakeTabStr() + "AliasDecl( " + alias_name->value + " -> ";
    if (alias_type == NAME_ALIAS) {
        s += base_name->value + " )";
    } else {
        s += base_type->ToString() + " )";
    }

    if (nl) {
        s += "\n";
    }

    return s;
}
