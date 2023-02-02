/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "ContinueStmt.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
ContinueStmt::ContinueStmt() {
    node_type = AST_CONTINUE_STMT;
}

/*****************************************************************************/
ContinueStmt::~ContinueStmt() = default;

/*****************************************************************************/
std::string ContinueStmt::ToString(bool nl) {
    return AstNode::ToString(nl);
}
