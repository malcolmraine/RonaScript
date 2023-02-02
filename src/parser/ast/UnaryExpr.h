/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_UNARYEXPR_H
#define RONASCRIPT_UNARYEXPR_H

#include <string>
#include "AstNode.h"

class UnaryExpr : public AstNode {
public:
    UnaryExpr();
    ~UnaryExpr() override;
    std::string ToString(bool nl) override;
    std::shared_ptr<AstNode> expr = nullptr;
    std::string op;
};

#endif  //RONASCRIPT_UNARYEXPR_H
