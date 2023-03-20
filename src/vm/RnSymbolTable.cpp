/*****************************************************************************
* File: RnSymbolTable.cpp
* Description:
* Author: Malcolm Hall
* Date: 5/24/22
* Version: 1
*
******************************************************************************/

#include "RnSymbolTable.h"
#include "RnMemoryGroup.h"
#include "RnObject.h"

/*****************************************************************************/
RnSymbolTable::RnSymbolTable(RnSymbolTable* parent_table) {
    SetParent(parent_table);
}

/*****************************************************************************/
RnSymbolTable::~RnSymbolTable() {
    _parent_table = nullptr;
}

/*****************************************************************************/
bool RnSymbolTable::SetSymbol(RnIntNative symbol, RnObject* obj) {
    if (_table.contains(symbol)) {
        _table[symbol] = obj;
        return true;
    } else if (_parent_table && _parent_table->SymbolExists(symbol)) {
        if (!_parent_table->SetSymbol(symbol, obj)) {
            return true;
        }
    }
    _table[symbol] = obj;
    return true;
}

/*****************************************************************************/
RnObject* RnSymbolTable::GetObject(RnIntNative symbol, bool should_throw) {
    auto iter = _table.find(symbol);
    if (iter != _table.end()) {
        return iter->second;
    } else if (_parent_table) {
        return _parent_table->GetObject(symbol, should_throw);
    } else {
        if (should_throw) {
            throw std::runtime_error("Symbol not found: " +
                                     RnObject::GetInternedString(symbol));
        }
        return nullptr;
    }
}

/*****************************************************************************/
void RnSymbolTable::RemoveSymbol(RnIntNative symbol) {
    auto iter = _table.find(symbol);
    if (iter != _table.end()) {
        _table.erase(iter);
    } else if (_parent_table) {
        _parent_table->RemoveSymbol(symbol);
    } else {
        throw std::runtime_error("Attempting to delete unknown symbol '" +
                                 RnObject::GetInternedString(symbol) + "'");
    }
}

/*****************************************************************************/
void RnSymbolTable::AliasSymbol(RnIntNative base, RnIntNative alias) {
    auto obj = GetObject(base);
    _table[alias] = obj;
}

/*****************************************************************************/
bool RnSymbolTable::SymbolExists(RnIntNative key, bool search_parent) const {
    if (_table.find(key) != _table.end()) {
        return true;
    } else if (_parent_table && search_parent) {
        return _parent_table->SymbolExists(key);
    }
    return false;
}

/*****************************************************************************/
void RnSymbolTable::SetParent(RnSymbolTable* parent) {
    assert(parent != this);
    _parent_table = parent;
}

/*****************************************************************************/
RnSymbolTable* RnSymbolTable::GetParent() const {
    return _parent_table;
}

/*****************************************************************************/
std::vector<RnIntNative> RnSymbolTable::GetSymbols() const {
    std::vector<RnIntNative> result;
    result.reserve(_table.size());
    for (const auto& pair : _table) {
        result.push_back(pair.first);
    }
    return result;
}
