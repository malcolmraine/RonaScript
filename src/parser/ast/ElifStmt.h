/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_ELIFSTMT_H
#define RONASCRIPT_ELIFSTMT_H

#include <string>
#include "AstNode.h"

class ElifStmt : public AstNode
{
 public:
	ElifStmt();
	~ElifStmt() override;
	std::string ToString(bool nl) override;
	std::shared_ptr<AstNode> test = nullptr;
	std::shared_ptr<AstNode> consequent = nullptr;
	std::shared_ptr<AstNode> alternative = nullptr;
};

#endif //RONASCRIPT_ELIFSTMT_H
