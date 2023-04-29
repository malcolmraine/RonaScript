/******************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Version: 1
*
* MIT License
*
* Copyright (c) 2021 Malcolm Hall
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
*******************************************************************************/

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "../common/RnType.h"
#include "ast/AstNode.h"

class SymbolTableEntry {
public:
    SymbolTableEntry(const std::string& name, std::shared_ptr<RnTypeComposite> type) {
        _name = name;
        _type = std::move(type);
    }

    std::shared_ptr<RnTypeComposite> GetType() {
        return _type;
    }

    std::string GetSymbol() {
        return _name;
    }

    void SetTypeDeclNode(std::shared_ptr<AstNode> reference) {
        _reference = std::move(reference);
    }

    [[nodiscard]] std::shared_ptr<AstNode> GetTypeDeclNode() {
        return _reference;
    }

private:
    std::string _name;
    std::shared_ptr<RnTypeComposite> _type;
    std::string _string_type;
    std::shared_ptr<AstNode> _reference;
};

class RnAstSymbolTable {
public:
    explicit RnAstSymbolTable(RnAstSymbolTable* parent = nullptr);
    bool SymbolExists(const std::string& symbol);
    std::shared_ptr<SymbolTableEntry> AddSymbol(
        const std::string& symbol, const std::shared_ptr<RnTypeComposite>& type,
        const std::shared_ptr<AstNode>& type_decl_node = nullptr);
    std::shared_ptr<SymbolTableEntry> GetSymbolEntry(const std::string& symbol);

    void SetParent(std::shared_ptr<RnAstSymbolTable> parent) {
        _parent = std::move(parent);
    }

private:
    std::map<std::string, std::shared_ptr<SymbolTableEntry>> _table;
    std::shared_ptr<RnAstSymbolTable> _parent;
};
