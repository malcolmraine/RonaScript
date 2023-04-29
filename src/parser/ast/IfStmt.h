/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#pragma once

#include "AstNode.h"

class IfStmt : public AstNode {
public:
    IfStmt();
    ~IfStmt() override;
    std::string ToString(bool nl) override;
    std::shared_ptr<AstNode> test = nullptr;
    std::shared_ptr<AstNode> consequent = nullptr;
    std::shared_ptr<AstNode> alternative = nullptr;
};