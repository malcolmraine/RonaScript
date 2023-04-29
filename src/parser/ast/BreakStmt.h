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

class BreakStmt : public AstNode {
public:
    BreakStmt();
    ~BreakStmt() override;
    std::string ToString(bool nl) override;
};

