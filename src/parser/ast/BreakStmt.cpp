/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "BreakStmt.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
BreakStmt::BreakStmt()
{
	node_type = AST_BREAK_STMT;
}

/*****************************************************************************/
BreakStmt::~BreakStmt()
{

}

/*****************************************************************************/
std::string BreakStmt::ToString(bool nl)
{
	std::string s = MakeTabStr() + "BreakStmt( )";
	if (nl)
	{
		s += "\n";
	}
	return s;
}

