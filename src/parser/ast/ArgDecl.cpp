/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "ArgDecl.h"
#include "../../codegen/RnCodeGenVisitor.h"
#include "../RnTypeComposite.h"
#include "Name.h"

/*****************************************************************************/
ArgDecl::ArgDecl() {
    node_type = AST_ARG_DECL;
}

/*****************************************************************************/
ArgDecl::~ArgDecl() = default;

/*****************************************************************************/
std::string ArgDecl::ToString(bool nl) {
    std::string result =
        MakeTabStr() + "ArgDecl( " + _id->value + ", " + _type->ToString() + " )";
    if (nl) {
        result += "\n";
    }
    return result;
}
