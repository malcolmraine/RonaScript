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
#include "Expr.h"

class IndexedExpr : public Expr {
public:
    IndexedExpr();
    ~IndexedExpr() override;
    std::string ToString(bool nl) override;
    std::shared_ptr<AstNode> idx = nullptr;
};
