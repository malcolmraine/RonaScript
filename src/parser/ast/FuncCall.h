/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef VARSTACKTEST_FUNCCALL_H
#define VARSTACKTEST_FUNCCALL_H

#include <vector>
#include <string>
#include "AstNode.h"

class FuncCall : public AstNode
{
 public:
	FuncCall();
	~FuncCall() override;
	std::string ToString(bool nl) override;

	std::shared_ptr<AstNode> expr = nullptr;
	std::vector<std::shared_ptr<AstNode>> args;
};

#endif //VARSTACKTEST_FUNCCALL_H
