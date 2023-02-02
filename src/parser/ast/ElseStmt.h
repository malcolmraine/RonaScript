/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef VARSTACKTEST_ELSESTMT_H
#define VARSTACKTEST_ELSESTMT_H

#include <string>
#include "AstNode.h"

class ScopeNode;

class ElseStmt : public AstNode {
public:
    ElseStmt();
    ~ElseStmt() override;
    std::string ToString(bool nl) override;
    std::shared_ptr<ScopeNode> consequent = nullptr;
};

#endif  //VARSTACKTEST_ELSESTMT_H
