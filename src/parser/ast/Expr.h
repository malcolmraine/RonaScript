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

class Expr : public AstNode {
public:
    Expr();
    ~Expr() override;
    std::string ToString(bool nl) override;

    AstNodePtr<AstNode> expr = nullptr;
};
