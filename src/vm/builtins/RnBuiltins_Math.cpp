/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "RnBuiltins_Math.h"
#include "../RnScope.h"
#include "../RnObject.h"
#include <random>

/*****************************************************************************/
void RnBuiltins_Math::pow(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{
	ret_val->SetData(std::pow(args.front()->ToFloat(), args.back()->ToFloat()));
}

/*****************************************************************************/
void RnBuiltins_Math::mod(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{
}

/*****************************************************************************/
void RnBuiltins_Math::sqrt(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{
	ret_val->SetData(std::sqrt(args.front()->ToFloat()));
}

/*****************************************************************************/
void RnBuiltins_Math::cbrt(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{
	ret_val->SetData(std::cbrt(args.front()->ToFloat()));
}

/*****************************************************************************/
void RnBuiltins_Math::randf(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{

}

/*****************************************************************************/
void RnBuiltins_Math::randint(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{
}

/*****************************************************************************/
void RnBuiltins_Math::normal(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{

}

/*****************************************************************************/
void RnBuiltins_Math::rn_builtin_sum(RnScope* scope, const std::vector<RnObject*>& args,
	RnObject* ret_val)
{
	RnFloatNative result = 0.0;
	auto operands = args.front()->ToList();
	for (auto operand : operands)
	{
		result += operand->ToFloat();
	}

	ret_val->SetData(result);
}