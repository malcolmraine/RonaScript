/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_IFSTMT_H
#define RONASCRIPT_IFSTMT_H

#include "AstNode.h"

class IfStmt : public AstNode {
public:
    IfStmt();
    ~IfStmt() override;
    std::string ToString(bool nl) override;
    std::shared_ptr<AstNode> test = nullptr;
    std::shared_ptr<AstNode> consequent = nullptr;
    std::shared_ptr<AstNode> alternative = nullptr;
};

#endif  //RONASCRIPT_IFSTMT_H
