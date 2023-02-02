/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef VARSTACKTEST_MODULE_H
#define VARSTACKTEST_MODULE_H

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

#endif  //VARSTACKTEST_MODULE_H
