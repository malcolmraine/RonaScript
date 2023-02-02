/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "ElifStmt.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
ElifStmt::ElifStmt() {
    node_type = AST_ELIF_STMT;
}

/*****************************************************************************/
ElifStmt::~ElifStmt() = default;

/*****************************************************************************/
std::string ElifStmt::ToString(bool nl) {
    std::string newline = nl ? "\n" : "";
    std::string output = MakeTabStr() + "ElifStmt( )" + newline;
    output += MakeTabStr() + "\tTest( )" + newline;
    test->nest_lvl = nest_lvl + 2;
    output += test->ToString(true);

    output += MakeTabStr() + "\tConsequent( )" + newline;
    consequent->nest_lvl = nest_lvl + 2;
    output += consequent->ToString(true);

    if (alternative != nullptr) {
        output += MakeTabStr() + "\tAlternative( )" + newline;
        alternative->nest_lvl = nest_lvl + 2;
        output += alternative->ToString(true);
    }

    return output;
}
