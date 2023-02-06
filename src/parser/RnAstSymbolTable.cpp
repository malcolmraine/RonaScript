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
    if (_table.contains(symbol) || (_parent && _parent->SymbolExists(symbol))) {
        return true;
    } else {
        return false;
    }
}

/*****************************************************************************/
std::shared_ptr<SymbolTableEntry> RnAstSymbolTable::AddSymbol(
    const std::string& symbol, const std::shared_ptr<RnTypeComposite>& type,
    const std::shared_ptr<AstNode>& type_decl_node) {
    auto entry = std::make_shared<SymbolTableEntry>(symbol, type);
    entry->SetTypeDeclNode(type_decl_node);
    _table[symbol] = entry;
    return entry;
}

/*****************************************************************************/
std::shared_ptr<SymbolTableEntry> RnAstSymbolTable::GetSymbolEntry(
    const std::string& symbol) {
    auto iter = _table.find(symbol);
    if (iter != _table.end()) {
        return iter->second;
    } else if (_parent) {
        return _parent->GetSymbolEntry(symbol);
    }
    throw std::runtime_error("Fatal: could not find symbol '" + symbol + "'");
}
