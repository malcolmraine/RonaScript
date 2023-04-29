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

class FlowStmt : public AstNode {
public:
    FlowStmt();
    ~FlowStmt() override;
    std::string ToString(bool nl) override;
};

