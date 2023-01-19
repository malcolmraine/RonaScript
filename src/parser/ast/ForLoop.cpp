/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "ForLoop.h"
#include "VarDecl.h"
#include "../../codegen/RnCodeGenVisitor.h"
#include "ScopeNode.h"

/*****************************************************************************/
ForLoop::ForLoop()
{
	node_type = AST_FOR_LOOP;
}

/*****************************************************************************/
ForLoop::~ForLoop()
{
}

/*****************************************************************************/
std::string ForLoop::ToString(bool nl)
{
	std::string output = MakeTabStr() + "ForLoop( )";
	if (nl)
	{
		output += "\n";
	}
	init->nest_lvl = nest_lvl + 1;
	test->nest_lvl = nest_lvl + 1;
	update->nest_lvl = nest_lvl + 1;
	scope->nest_lvl = nest_lvl + 1;
	output += init->ToString(nl);
	output += test->ToString(nl);
	output += update->ToString(nl);
	output += scope->ToString(nl);

	return output;
}

