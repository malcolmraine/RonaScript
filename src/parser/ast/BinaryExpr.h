/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_BINARYEXPR_H
#define RONASCRIPT_BINARYEXPR_H

#include <string>
#include "AstNode.h"

class RnCodeGenVisitor;

class BinaryExpr : public AstNode
{
 public:
	BinaryExpr();
	BinaryExpr(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right,
		std::string op);
	~BinaryExpr() override = default;
	std::string ToString(bool nl) override;

	std::shared_ptr<AstNode> _left = nullptr;
	std::shared_ptr<AstNode> _right = nullptr;
	std::string _op;
};

#endif //RONASCRIPT_BINARYEXPR_H
