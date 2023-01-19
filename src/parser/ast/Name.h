/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef VARSTACKTEST_NAME_H
#define VARSTACKTEST_NAME_H

#include <string>
#include "AstNode.h"
#include "NodeType.h"

class RnCodeGenVisitor;

class RnCodeGenVisitor;

class Name : public AstNode
{
 public:
	Name();
	explicit Name(std::string name);
	~Name() override;
	std::string ToString(bool nl) override;

	std::string value;
};

#endif //VARSTACKTEST_NAME_H
