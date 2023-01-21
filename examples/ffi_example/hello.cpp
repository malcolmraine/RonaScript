// hello.cpp
#include <iostream>
#include "../../src/ffi/RnSharedLib.h"

/*****************************************************************************/
const char* LibraryName() {
	return "HelloWorld";
}

/*****************************************************************************/
const char* LibraryVersion() {
	return "1.0.0";
}

/*****************************************************************************/
void LibraryFunctions(std::vector<std::tuple<std::string, RnType::Type>>& functions) {
	functions.emplace_back("hello", RnType::RN_STRING);
}

/*****************************************************************************/
RONA_EXPORT void hello(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val);

/*****************************************************************************/
void hello(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{
	ret_val->SetData(static_cast<std::string>("This is a test of return a value"));
}