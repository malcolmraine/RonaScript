/*****************************************************************************
* File: ConditionalStmt.cpp
* Description:
* Author: Malcolm Hall
* Date: 4/29/23
* Version: 1
*
*****************************************************************************/
//
// Created by Malcolm Hall on 4/29/23.
//

#include "ConditionalStmt.h"

/*****************************************************************************/
ConditionalStmt::ConditionalStmt() = default;

/*****************************************************************************/
ConditionalStmt::~ConditionalStmt() = default;

/*****************************************************************************/
std::string ConditionalStmt::ToString(bool nl) {
    std::string newline = nl ? "\n" : "";
    std::string output;
    if (node_type == AST_IF_STMT) {
        output += MakeTabStr() + "IfStmt( )" + newline;
    } else if (node_type == AST_ELIF_STMT) {
        output += MakeTabStr() + "ElifStmt( )" + newline;
    } else {
        output += MakeTabStr() + "ElseStmt( )" + newline;
    }

    if (test) {
        test->nest_lvl = nest_lvl + 1;
        output += test->ToString(true);
    }

    output += MakeTabStr() + "\tConsequent( )" + newline;
    consequent->nest_lvl = nest_lvl + 1;
    output += consequent->ToString(true);

    if (alternative) {
        output += MakeTabStr() + "\tAlternative( )" + newline;
        alternative->nest_lvl = nest_lvl + 1;
        output += alternative->ToString(true);
    }

    return output;
}
