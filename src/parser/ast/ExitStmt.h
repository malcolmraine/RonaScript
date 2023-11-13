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

class LiteralValue;

class ExitStmt : public AstNode {
public:
    ExitStmt();
    ~ExitStmt() override;
    std::string ToString(bool nl) override;

    AstNodePtr<LiteralValue> exit_code = nullptr;
};
