/*****************************************************************************
* File: RnSymbolTable.h
* Description:
* Author: Malcolm Hall
* Date: 6/20/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2020 - 2023 Malcolm Hall
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
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
