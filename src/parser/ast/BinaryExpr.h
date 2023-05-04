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
    BinaryExpr(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right,
               std::string op);
    ~BinaryExpr() override = default;
    std::string ToString(bool nl) override;

    std::shared_ptr<AstNode> _left = nullptr;
    std::shared_ptr<AstNode> _right = nullptr;
    std::string _op;
};
