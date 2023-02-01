/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "IfStmt.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
IfStmt::IfStmt()
{
	node_type = AST_IF_STMT;
}

/*****************************************************************************/
IfStmt::~IfStmt() = default;

/*****************************************************************************/
std::string IfStmt::ToString(bool nl)
{
	std::string newline = nl ? "\n" : "";
	std::string output = MakeTabStr() + "IfStmt( )" + newline;
	output += MakeTabStr() + "\tTest( )" + newline;
	test->nest_lvl = nest_lvl + 1;
	output += test->ToString(true);

	output += MakeTabStr() + "\tConsequent( )" + newline;
	consequent->nest_lvl = nest_lvl + 1;
	output += consequent->ToString(true);

	if (alternative != nullptr)
	{
		output += MakeTabStr() + "\tAlternative( )" + newline;
		alternative->nest_lvl = nest_lvl + 1;
		output += alternative->ToString(true);
	}

	return output;
}

