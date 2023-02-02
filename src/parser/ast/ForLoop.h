/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_FORLOOP_H
#define RONASCRIPT_FORLOOP_H

#include "AstNode.h"

class VarDecl;

class ScopeNode;

class ForLoop : public AstNode {
public:
    ForLoop();
    ~ForLoop();
    std::string ToString(bool nl) override;

    std::shared_ptr<AstNode> init = nullptr;
    std::shared_ptr<AstNode> test = nullptr;
    std::shared_ptr<AstNode> update = nullptr;
    std::shared_ptr<ScopeNode> scope = nullptr;
};

#endif  //RONASCRIPT_FORLOOP_H
