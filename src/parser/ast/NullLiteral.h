/*****************************************************************************
* File: NullLiteral.h
* Description:
* Author: Malcolm Hall
* Date: 6/20/22
* Version: 1
*
******************************************************************************/

#pragma once

#include "AstNode.h"

class RnCodeGenVisitor;

class NullLiteral : public AstNode {
public:
    NullLiteral() = default;
    ~NullLiteral() override = default;
    std::string ToString(bool nl) override;
};
