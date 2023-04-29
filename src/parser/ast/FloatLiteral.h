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

class FloatLiteral : public AstNode {
public:
    FloatLiteral();
    explicit FloatLiteral(double value);
    explicit FloatLiteral(bool value);
    ~FloatLiteral() override;
    std::string ToString(bool nl) override;

    double data = 0.0;
};

