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
#include "NodeType.h"

class IntLiteral;

class ExitStmt : public AstNode {
public:
    ExitStmt();
    ~ExitStmt() override;
    std::string ToString(bool nl) override;

    std::shared_ptr<IntLiteral> exit_code = nullptr;
};

