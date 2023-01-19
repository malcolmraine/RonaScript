/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_ASSIGNMENTSTMT_H
#define RONASCRIPT_ASSIGNMENTSTMT_H

#include <string>
#include "AstNode.h"

class AssignmentStmt : public AstNode
{
 public:
	AssignmentStmt();
	~AssignmentStmt();
	std::string ToString(bool nl) override;
	std::shared_ptr<AstNode> rexpr{};
	std::shared_ptr<AstNode> lexpr = nullptr;
	std::shared_ptr<AstNode> offset = nullptr;
	std::shared_ptr<AstNode> idx = nullptr;
};

#endif //RONASCRIPT_ASSIGNMENTSTMT_H
