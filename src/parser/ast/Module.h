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
#include "Name.h"
#include "NodeType.h"
#include "ScopeNode.h"

class Module : public AstNode {
public:
    Module();
    ~Module() override;
    std::string ToString(bool nl) override;
    std::shared_ptr<Name> name = nullptr;
    std::shared_ptr<ScopeNode> scope = nullptr;
};
