#include <iostream>
#include "../../src/ffi/RnSharedLib.h"

DEFINE_LIBRARY("HelloWorld", "1.0.0")
{
	REGISTER_LIBRARY_FUNCTION(hello, RnType::RN_STRING)
}

/*****************************************************************************/
RONA_EXPORT void hello(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val);

/*****************************************************************************/
void hello(RnScope* scope, const std::vector<RnObject*>& args, RnObject* ret_val)
{
	ret_val->SetData(static_cast<std::string>("This is a test of return a value"));
}