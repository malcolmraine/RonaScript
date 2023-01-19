/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "ReturnStmt.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
ReturnStmt::ReturnStmt()
{
	node_type = AST_RETURN_STMT;
}

/*****************************************************************************/
ReturnStmt::~ReturnStmt()
{

}

/*****************************************************************************/
std::string ReturnStmt::ToString(bool nl)
{
	std::string s = MakeTabStr() + "Return( )";
	if (nl)
	{
		s += "\n";
	}

	if (expr != nullptr)
	{
		expr->nest_lvl = nest_lvl + 1;
		s += expr->ToString(nl);
	}

	return s;
}
