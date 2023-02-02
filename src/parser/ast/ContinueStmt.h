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

class ContinueStmt : public AstNode {
public:
    ContinueStmt();
    ~ContinueStmt() override;
    std::string ToString(bool nl) override;
};

#endif  //RONASCRIPT_FLOWSTMT_H
