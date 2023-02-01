/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "AttributeAccess.h"
#include "../../codegen/RnCodeGenVisitor.h"
#include <iostream>

/*****************************************************************************/
AttributeAccess::AttributeAccess()
{
	node_type = AST_BINARY_EXPR;
}

/*****************************************************************************/
AttributeAccess::~AttributeAccess() = default;

/*****************************************************************************/
std::string AttributeAccess::ToString(bool nl)
{
	std::string s = MakeTabStr() + "AttributeAccess( )";
	if (nl)
	{
		s += "\n";
	}
	parent->nest_lvl = nest_lvl + 1;
	name->nest_lvl = nest_lvl + 1;
	s += parent->ToString(nl) + name->ToString(nl);

	return s;
}
