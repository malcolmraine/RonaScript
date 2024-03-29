/*****************************************************************************
* File: RnAstSymbolTable.cpp
* Description:
* Author: Malcolm Hall
* Date: 1/27/23
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
    const AstNodePtr<AstNode>& type_decl_node) {
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

/*****************************************************************************/
bool RnAstSymbolTable::HasSymbolEntry(const std::string& symbol) const {
    auto iter = _table.find(symbol);
    if (iter != _table.end()) {
        return true;
    } else if (_parent) {
        return _parent->HasSymbolEntry(symbol);
    }
    return false;
}
