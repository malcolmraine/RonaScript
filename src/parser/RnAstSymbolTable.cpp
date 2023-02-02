/*****************************************************************************
* File: RnAstSymbolTable.cpp
* Description:
* Author: Malcolm Hall
* Date: 1/27/23
* Version: 1
*
*****************************************************************************/

#include "RnAstSymbolTable.h"

/*****************************************************************************/
RnAstSymbolTable::RnAstSymbolTable(RnAstSymbolTable* parent) : _parent(parent) {}

/*****************************************************************************/
bool RnAstSymbolTable::SymbolExists(const std::string& symbol) {
    if (_table.contains(symbol)) {
        return true;
    } else if (_parent && _parent->SymbolExists(symbol)) {
        return true;
    } else {
        return false;
    }
}

/*****************************************************************************/
std::shared_ptr<SymbolTableEntry> RnAstSymbolTable::AddSymbol(
    const std::string& symbol, std::shared_ptr<RnTypeComposite> type) {
    auto entry = std::make_shared<SymbolTableEntry>(symbol, type);
    _table[symbol] = entry;
    return entry;
}

/*****************************************************************************/
std::shared_ptr<SymbolTableEntry> RnAstSymbolTable::GetSymbolEntry(
    const std::string& symbol) {
    return _table[symbol];
}
