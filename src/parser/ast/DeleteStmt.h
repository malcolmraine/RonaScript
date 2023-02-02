/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_DELETESTMT_H
#define RONASCRIPT_DELETESTMT_H

#include <string>
#include "AstNode.h"
#include "NodeType.h"

class DeleteStmt : public AstNode {
public:
    DeleteStmt();
    ~DeleteStmt() override;
    std::string ToString(bool nl) override;
    std::shared_ptr<AstNode> name = nullptr;
};

#endif  //RONASCRIPT_DELETESTMT_H
