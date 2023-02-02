/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_ATTRIBUTEACCESS_H
#define RONASCRIPT_ATTRIBUTEACCESS_H

#include <string>
#include "AstNode.h"

class AttributeAccess : public AstNode {
public:
    AttributeAccess();
    ~AttributeAccess() override;
    std::string ToString(bool nl) override;

    std::shared_ptr<AstNode> parent = nullptr;
    std::shared_ptr<AstNode> name = nullptr;
    std::string op;
};

#endif  //RONASCRIPT_ATTRIBUTEACCESS_H
