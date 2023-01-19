/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "ElseStmt.h"
#include "ScopeNode.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
ElseStmt::ElseStmt()
{
	node_type = AST_ELSE_STMT;
}

/*****************************************************************************/
ElseStmt::~ElseStmt() = default;

/*****************************************************************************/
std::string ElseStmt::ToString(bool nl)
{
	std::string s = MakeTabStr() + "Else( )";

	if (nl)
	{
		s += "\n";
	}
	s += MakeTabStr() + "\tConsequent( )";

	if (nl)
	{
		s += "\n";
	}
	consequent->nest_lvl = nest_lvl + 2;
	s += consequent->ToString(nl);

	return s;
}

