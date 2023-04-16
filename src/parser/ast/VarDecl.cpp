/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "VarDecl.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
VarDecl::VarDecl() : type(std::make_shared<RnTypeComposite>(RnType::RN_NULL)) {
    node_type = AST_VAR_DECL;
}

/*****************************************************************************/
VarDecl::~VarDecl() = default;

/*****************************************************************************/
std::string VarDecl::ToString(bool nl) {
    std::string s = MakeTabStr();

    if (is_const) {
        s.append("ConstDecl");
    } else if (is_local) {
        s.append("LocalDecl");
    } else if (is_global) {
        s.append("GlobalDecl");
    } else if (is_literal) {
        s.append("LiteralDecl");
    } else {
        s.append("VarDecl");
    }

    s.append("( ").append(id).append(", ").append(type->ToString());

    for (auto& qualifier : qualifiers) {
        s.append(", ").append(qualifier->lexeme);
    }

    s.append(" )");
    if (nl) {
        s.append("\n");
    }

    if (init_value != nullptr) {
        init_value->nest_lvl = nest_lvl + 1;
        s.append(MakeTabStr()).append("AssignmentStmt( ").append(id).append(" )");
        if (nl) {
            s.append("\n");
        }
        s.append(init_value->ToString(nl));
    }

    return s;
}
