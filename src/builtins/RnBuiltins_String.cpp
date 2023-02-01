/*****************************************************************************
* File: RnBuiltins_String.cpp
* Description:
* Author: Malcolm Hall
* Date: 2/1/22
* Version: 1
*
******************************************************************************/

#include "RnBuiltins_String.h"
#include "../util/MLib/String.h"
#include "../vm/RnScope.h"
#include "../vm/RnObject.h"

/*****************************************************************************/
void RnBuiltins_String::titlecase(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{

}

/*****************************************************************************/
void RnBuiltins_String::lower(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{

}

/*****************************************************************************/
void RnBuiltins_String::upper(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{

}

/*****************************************************************************/
void RnBuiltins_String::snakecase(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{

}

/*****************************************************************************/
void RnBuiltins_String::str_split(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{

}

/*****************************************************************************/
void RnBuiltins_String::str_merge(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{

}

/*****************************************************************************/
void RnBuiltins_String::str_substr(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{
	assert(args.size() == 2 || args.size() == 3);
}

/*****************************************************************************/
void RnBuiltins_String::str_startwith(RnScope* scope,
	const std::vector<RnObject*>& args, RnObject* ret_val)
{
	assert(args.size() == 2);
	ret_val->SetData(static_cast<bool>(String::StartsWith(args.front()->ToString(),
		args.back()->ToString())));
}

/*****************************************************************************/
void RnBuiltins_String::str_endswith(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{
	assert(args.size() == 2);
	ret_val->SetData(static_cast<bool>(String::EndsWith(args.front()->ToString(),
		args.back()->ToString())));
}

/*****************************************************************************/
void RnBuiltins_String::str_join(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{
	assert(args.size() == 2 || args.size() == 3);
}

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_str_merge(RnScope* scope,
	const std::vector<RnObject*>& args, RnObject* ret_val)
{
	std::string result;
	for (auto arg : args)
	{
		result += arg->ToString();
	}
	ret_val->SetData(result);
}

