/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_EXITSTMT_H
#define RONASCRIPT_EXITSTMT_H

#include <string>
#include "NodeType.h"
#include "AstNode.h"

class IntLiteral;

class ExitStmt : public AstNode
{
 public:
	ExitStmt();
	~ExitStmt();
	std::string ToString(bool nl) override;

	std::shared_ptr<IntLiteral> exit_code = nullptr;
};

#endif //RONASCRIPT_EXITSTMT_H
