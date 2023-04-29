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
#include <vector>
#include "AstNode.h"

class ArrayLiteral : public AstNode {
public:
    ArrayLiteral();
    ~ArrayLiteral();
    std::string ToString(bool nl) override;

public:
    std::vector<std::shared_ptr<AstNode>> items;
};

