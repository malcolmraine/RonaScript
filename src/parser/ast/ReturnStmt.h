/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RETURNSTMT_H
#define RONASCRIPT_RETURNSTMT_H

#include <string>
#include "AstNode.h"
#include "NodeType.h"

class ReturnStmt : public AstNode {
public:
    ReturnStmt();
    ~ReturnStmt() override;
    std::string ToString(bool nl) override;
    std::shared_ptr<AstNode> expr = nullptr;
};

#endif  //RONASCRIPT_RETURNSTMT_H
