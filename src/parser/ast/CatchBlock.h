/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef VARSTACKTEST_CATCH_BLOCK_H
#define VARSTACKTEST_CATCH_BLOCK_H

#include "AstNode.h"

class Name;

class ScopeNode;

class CatchBlock : public AstNode
{
 public:
	CatchBlock();
	~CatchBlock();
	std::string ToString(bool nl) override;
	std::vector<std::shared_ptr<Name>> exception_ids;
	std::shared_ptr<ScopeNode> scope = nullptr;
};

#endif //VARSTACKTEST_CATCH_BLOCK_H
