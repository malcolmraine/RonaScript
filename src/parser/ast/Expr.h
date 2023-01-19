/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_EXPR_H
#define RONASCRIPT_EXPR_H

#include <string>
#include "AstNode.h"

class Expr : public AstNode
{
 public:
	Expr();
	~Expr() override;
	std::string ToString(bool nl) override;

	std::shared_ptr<AstNode> expr = nullptr;
};

#endif //RONASCRIPT_EXPR_H
