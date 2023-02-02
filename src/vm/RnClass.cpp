/*****************************************************************************
* File: RnClass.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/29/22
* Version: 1
*
******************************************************************************/

#include "RnClass.h"
#include "RnObject.h"
#include "RnScope.h"
#include "RnSymbolTable.h"

/*****************************************************************************/
RnClass::RnClass(RnScope* parent) : RnScope(parent) {}

/*****************************************************************************/
RnClass::~RnClass() {}

/*****************************************************************************/
void RnClass::CopySymbols(RnScope* target) {
    auto symbol_table = GetSymbolTable();
    auto target_symbol_table = target->GetSymbolTable();

    for (const auto& symbol : symbol_table->GetSymbols()) {
        auto obj = symbol_table->GetObject(symbol);

        // TODO: Fix this to allow callables to be assigned to class attributes
        if (obj->GetType() != RnType::RN_FUNCTION) {
            target_symbol_table->SetSymbol(symbol, obj);
        }
    }
}
