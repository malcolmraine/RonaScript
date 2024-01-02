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

class BinaryExpr : public AstNode {
public:
    BinaryExpr();
    BinaryExpr(AstNodePtr<AstNode> left, AstNodePtr<AstNode> right, std::string op);
    ~BinaryExpr() override = default;
    std::string ToString(bool nl) override;

    AstNodePtr<AstNode> _left = nullptr;
    AstNodePtr<AstNode> _right = nullptr;
    std::string _op;
};
