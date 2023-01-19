/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "ArrayLiteral.h"
#include "../../codegen/RnCodeGenVisitor.h"
#include <iostream>

/*****************************************************************************/
ArrayLiteral::ArrayLiteral()
{
	node_type = AST_LIST_LITERAL;
}

/*****************************************************************************/
ArrayLiteral::~ArrayLiteral()
{
	items.clear();
}

/*****************************************************************************/
std::string ArrayLiteral::ToString(bool nl)
{
	std::string s = MakeTabStr() + "ArrayLiteral( )";
	if (nl)
	{
		s += "\n";
	}

	for (auto& item : items)
	{
		item->nest_lvl = nest_lvl + 1;
		s += item->ToString(nl);
	}

	return s;
}
