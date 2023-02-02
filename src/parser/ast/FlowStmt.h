/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_FLOWSTMT_H
#define RONASCRIPT_FLOWSTMT_H

#include "AstNode.h"

class FlowStmt : public AstNode {
public:
    FlowStmt();
    ~FlowStmt() override;
    std::string ToString(bool nl) override;
};

#endif  //RONASCRIPT_FLOWSTMT_H
