/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "DeleteStmt.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
DeleteStmt::DeleteStmt() {
    node_type = AST_DELETE_STMT;
}

/*****************************************************************************/
DeleteStmt::~DeleteStmt() = default;

/*****************************************************************************/
std::string DeleteStmt::ToString(bool nl) {
    std::string s = MakeTabStr() + "Delete( )\n";
    if (nl) {
        s += "\n";
    }
    name->nest_lvl = nest_lvl + 1;
    s += name->ToString(nl);

    return s;
}
