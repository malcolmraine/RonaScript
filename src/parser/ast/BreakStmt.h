/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_BREAKSTMT_H
#define RONASCRIPT_BREAKSTMT_H

#include <string>
#include "AstNode.h"

class BreakStmt : public AstNode {
public:
    BreakStmt();
    ~BreakStmt() override;
    std::string ToString(bool nl) override;
};

#endif  //RONASCRIPT_BREAKSTMT_H
