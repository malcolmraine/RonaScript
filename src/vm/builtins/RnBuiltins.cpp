/*****************************************************************************
* File: RnBuiltins.cpp
* Description:
* Author: Malcolm Hall
* Date: 5/30/22
* Version: 1
*
******************************************************************************/

#include "RnBuiltins.h"
#include <string>
#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <string>
#include <array>
#include <sstream>
#include <iostream>
#include "../../util/MLib/String.h"
#include "../RnScope.h"
#include "../RnObject.h"
#include "../RnFunctionObject.h"
#include "../RnFunction.h"
#include <dlfcn.h>

/*****************************************************************************/
void RnBuiltins::rn_builtin_unpack(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{
	auto parent_scope = scope->GetParent();
	RnScope* unpack_scope = parent_scope;
	if (!unpack_scope)
	{
		unpack_scope = scope;
	}

	for (auto obj : args.front()->ToArray())
	{
		scope->GetStack().push_back(obj);
	}
}

/*****************************************************************************/
void RnBuiltins::rn_builtin_call(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{
	auto obj = scope->GetObject(args.front()->ToString());
	auto func_obj = dynamic_cast<RnFunctionObject*>(obj);
	auto func = func_obj->GetData();
	std::vector<RnObject*> callArgs(args.begin() + 1, args.end());
	RnObject* call_ret_val = RnObject::Create(func_obj->GetReturnType());
	func->Call(callArgs, call_ret_val);
	std::vector<RnObject*> ret_vals = { call_ret_val };
	ret_val->SetData(ret_vals);
}

/*****************************************************************************/
void RnBuiltins::rn_builtin_system(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{
	std::array<char, 128> buffer{};
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)>
		pipe(popen(args.front()->ToString().c_str(), "r"), pclose);
	if (!pipe)
	{
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
	{
		result += buffer.data();
	}
	result = String::Replace(result, "\n", "\\n");
	std::vector<RnObject*> output = { RnObject::Create(result) };
	ret_val->SetData(output);
}

/*****************************************************************************/
void RnBuiltins::lload(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{
	// TODO: Add file existence check for library
	// TODO: Add check to make sure library actually loaded and return status accordingly
	RnScope::LoadLibraryIntoScope(scope->GetParent(), args.at(0)->ToString());
}
