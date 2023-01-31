/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "RnClassObject.h"
#include <iomanip>
#include <sstream>

/*****************************************************************************/
auto RnClassObject::ToString() const -> RnStringNative
{
	std::stringstream s;
	s << "{class object @ " << std::hex << _data << "}";
	return s.str();
}

/*****************************************************************************/
void RnClassObject::CopySymbols(RnScope* target)
{
	auto symbol_table = _data->GetSymbolTable();
	auto target_symbol_table = target->GetSymbolTable();

	for (const auto& symbol : symbol_table->GetSymbols())
	{
		auto obj = symbol_table->GetObject(symbol);

		// TODO: Fix this to allow callables to be assigned to class attributes
		if (obj->GetType() != RnType::RN_FUNCTION)
		{
			target_symbol_table->SetSymbol(symbol, obj);
		}
	}
}
