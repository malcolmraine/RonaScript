/*****************************************************************************
* File: RnBuiltins.h
* Description:
* Author: Malcolm Hall
* Date: 5/30/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNBUILTINS_H
#define RONASCRIPT_RNBUILTINS_H

#include <vector>
#include <map>

class RnScope;
class RnObject;

class RnBuiltins
{
 public:
	static void rn_builtin_unpack(RnScope* scope, const std::vector<RnObject*>& args,
		RnObject* ret_val);
	static void rn_builtin_call(RnScope* scope, const std::vector<RnObject*>& args,
		RnObject* ret_val);
	static void rn_builtin_system(RnScope* scope, const std::vector<RnObject*>& args,
		RnObject* ret_val);
	static void rn_builtin_lload(RnScope* scope, const std::vector<RnObject*>& args,
		RnObject* ret_val);
	static void rn_builtin_bind(RnScope* scope, const std::vector<RnObject*>& args,
		RnObject* ret_val);
};

#endif //RONASCRIPT_RNBUILTINS_H