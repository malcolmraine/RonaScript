/*****************************************************************************
* File: RnBuiltins_IO.cpp
* Description:
* Author: Malcolm Hall
* Date: 2/1/22
* Version: 1
*
******************************************************************************/

#include "RnBuiltins_IO.h"
#include "../vm/RnScope.h"
#include "../vm/RnObject.h"
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iostream>

/*****************************************************************************/
void RnBuiltins_IO::file_size(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{
	std::string path = args.front()->ToString();
	ret_val->SetData(static_cast<RnIntNative>(std::filesystem::file_size(path)));
}

/*****************************************************************************/
void RnBuiltins_IO::file_write(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{
}

/*****************************************************************************/
void RnBuiltins_IO::rn_builtin_print(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{
	std::string s;
	for (auto arg : args)
	{
		s.append(arg->ToString());
	}
	std::cout << s << "\n";
}

/*****************************************************************************/
void RnBuiltins_IO::read(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{
	std::ifstream file;
	file.open(args.front()->ToString());

	std::string contents;
	file.seekg(0, std::ios::end);
	contents.reserve(file.tellg());
	file.seekg(0, std::ios::beg);

	contents.assign((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());
	file.close();
	ret_val->SetData(contents);
}

/*****************************************************************************/
void RnBuiltins_IO::rn_builtin_prompt(RnScope* scope,
	const std::vector<RnObject*>& args, RnObject* ret_val)
{
	std::string input;
	std::cout << args.front()->ToString();
	std::cin >> input;
	ret_val->SetData(input);
}