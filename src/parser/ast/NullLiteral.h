/*****************************************************************************
* File: NullLiteral.h
* Description:
* Author: Malcolm Hall
* Date: 6/20/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_NULLLITERAL_H
#define RONASCRIPT_NULLLITERAL_H

#include "AstNode.h"

class RnCodeGenVisitor;

class NullLiteral : public AstNode {
public:
    NullLiteral() = default;
    ~NullLiteral() override = default;
    std::string ToString(bool nl) override;
};

#endif  //RONASCRIPT_NULLLITERAL_H