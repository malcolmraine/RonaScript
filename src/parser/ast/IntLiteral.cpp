/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "IntLiteral.h"
#include <iostream>
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
IntLiteral::IntLiteral()
{
	node_type = AST_INT_LITERAL;
	data = 0;
}

/*****************************************************************************/
IntLiteral::IntLiteral(long value)
{
	node_type = AST_INT_LITERAL;
	data = value;
}

/*****************************************************************************/
IntLiteral::~IntLiteral() = default;

/*****************************************************************************/
std::string IntLiteral::ToString(bool nl)
{
	return MakeTabStr() + "IntLiteral( " + std::to_string(data) + " )"
		+ (nl ? "\n" : "");
}

/*****************************************************************************/
IntLiteral::IntLiteral(bool value)
{
	data = value ? 1 : 0;
}

/*****************************************************************************/
IntLiteral::IntLiteral(double value)
{
	data = static_cast<RnIntNative>(value);
}

/*****************************************************************************/
IntLiteral::IntLiteral(const std::string& value)
{
	std::cout << value;
}
