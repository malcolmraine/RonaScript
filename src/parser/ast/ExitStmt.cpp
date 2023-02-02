/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "ExitStmt.h"
#include "../../codegen/RnCodeGenVisitor.h"
#include "IntLiteral.h"

/*****************************************************************************/
ExitStmt::ExitStmt() {
    node_type = AST_EXIT_STMT;
}

/*****************************************************************************/
ExitStmt::~ExitStmt() = default;

/*****************************************************************************/
std::string ExitStmt::ToString(bool nl) {
    std::string s = MakeTabStr() + "Exit( " + exit_code->ToString(false) + " )";
    if (nl) {
        s += "\n";
    }
    return s;
}
