/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_WHILELOOP_H
#define RONASCRIPT_WHILELOOP_H

#include <string>
#include "AstNode.h"

class ScopeNode;

class WhileLoop : public AstNode {
public:
    WhileLoop();
    ~WhileLoop() override;
    std::string ToString(bool nl) override;
    std::shared_ptr<AstNode> test = nullptr;
    std::shared_ptr<ScopeNode> scope = nullptr;
};

#endif  //RONASCRIPT_WHILELOOP_H
