/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "Name.h"
#include "../../codegen/RnCodeGenVisitor.h"
#include <utility>

/*****************************************************************************/
Name::Name()
{
	node_type = AST_NAME;
	value = "";
}

/*****************************************************************************/
Name::Name(std::string name)
{
	value = std::move(name);
}

/*****************************************************************************/
Name::~Name() = default;

/*****************************************************************************/
std::string Name::ToString(bool nl)
{
	std::string output = MakeTabStr() + "Name( " + value + " )";

	if (nl)
	{
		output += "\n";
	}

	return output;
}

