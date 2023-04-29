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

class StringLiteral : public AstNode {
public:
    StringLiteral();
    explicit StringLiteral(std::string value);
    explicit StringLiteral(bool value);
    explicit StringLiteral(double value);
    ~StringLiteral() override;
    std::string ToString(bool nl) override;
    std::string data;
};

