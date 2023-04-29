/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#pragma once

#include <string>
#include "AstNode.h"

class ElifStmt : public AstNode {
public:
    ElifStmt();
    ~ElifStmt() override;
    std::string ToString(bool nl) override;
    std::shared_ptr<AstNode> test = nullptr;
    std::shared_ptr<AstNode> consequent = nullptr;
    std::shared_ptr<AstNode> alternative = nullptr;
};

