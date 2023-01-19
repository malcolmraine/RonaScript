/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "CatchBlock.h"
#include "Name.h"
#include "ScopeNode.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
CatchBlock::CatchBlock() = default;

/*****************************************************************************/
CatchBlock::~CatchBlock() = default;

/*****************************************************************************/
std::string CatchBlock::ToString(bool nl)
{
	std::string s = MakeTabStr() + "Catch( ";
	for (auto& exception_id : exception_ids)
	{
		s += exception_id->value + " ";
	}

	s += ")";
	if (nl)
	{
		s += "\n";
	}
	scope->nest_lvl = nest_lvl + 1;
	s += scope->ToString(true);

	return s;
}
