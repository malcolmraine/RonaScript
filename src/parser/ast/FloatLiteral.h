/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_FLOATLITERAL_H
#define RONASCRIPT_FLOATLITERAL_H

#include <string>
#include "AstNode.h"

class FloatLiteral : public AstNode
{
 public:
	FloatLiteral();
	explicit FloatLiteral(double value);
	explicit FloatLiteral(bool value);
	explicit FloatLiteral(const std::string& value);
	~FloatLiteral() override;
	std::string ToString(bool nl) override;

	double data = 0.0;
};

#endif //RONASCRIPT_FLOATLITERAL_H
