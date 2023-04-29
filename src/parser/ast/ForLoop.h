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

class VarDecl;
class ScopeNode;

class ForLoop : public AstNode {
public:
    ForLoop();
    ~ForLoop() override;
    std::string ToString(bool nl) override;

    std::shared_ptr<AstNode> init = nullptr;
    std::shared_ptr<AstNode> test = nullptr;
    std::shared_ptr<AstNode> update = nullptr;
    std::shared_ptr<ScopeNode> scope = nullptr;
};

