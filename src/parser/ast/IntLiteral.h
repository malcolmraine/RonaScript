/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_INTLITERAL_H
#define RONASCRIPT_INTLITERAL_H

#include "../../common/RnType.h"
#include "AstNode.h"

class IntLiteral : public AstNode {
public:
    IntLiteral();
    explicit IntLiteral(long value);
    explicit IntLiteral(bool value);
    explicit IntLiteral(double value);
    ~IntLiteral();
    std::string ToString(bool nl) override;
    RnIntNative data;
};

#endif  //RONASCRIPT_INTLITERAL_H
