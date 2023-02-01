/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_STRINGLITERAL_H
#define RONASCRIPT_STRINGLITERAL_H

#include <string>
#include "AstNode.h"

class StringLiteral : public AstNode
{
 public:
	StringLiteral();
	explicit StringLiteral(std::string value);
	explicit StringLiteral(bool value);
	explicit StringLiteral(double value);
	~StringLiteral() override;
	std::string ToString(bool nl) override;
	std::string data;
};

#endif //RONASCRIPT_STRINGLITERAL_H
