/*****************************************************************************
* File: RnSymbolTable.h
* Description:
* Author: Malcolm Hall
* Date: 5/24/22
* Version: 1
*
******************************************************************************/

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "../common/RnType.h"

class RnMemoryGroup;

class RnObject;

class RnSymbolTable {
public:
    explicit RnSymbolTable(RnSymbolTable* parent_table = nullptr);
    ~RnSymbolTable();
    bool SetSymbol(RnIntNative symbol, RnObject* obj);
    RnObject* GetObject(RnIntNative symbol, bool should_throw = false);
    void RemoveSymbol(RnIntNative symbol);
    void AliasSymbol(RnIntNative base, RnIntNative alias);
    [[nodiscard]] bool SymbolExists(RnIntNative key, bool search_parent = true) const;
    void SetParent(RnSymbolTable* parent);
    [[nodiscard]] RnSymbolTable* GetParent() const;
    [[nodiscard]] std::vector<RnIntNative> GetSymbols() const;
    void Clear() {
        _table.clear();
    }

private:
    RnSymbolTable* _parent_table = nullptr;
    std::unordered_map<RnIntNative, RnObject*> _table;
};
