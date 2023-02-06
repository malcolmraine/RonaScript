//
// Created by Malcolm Hall on 1/27/23.
//

#ifndef RONASCRIPT_SRC_PARSER_RNASTSYMBOLTABLE_H_
#define RONASCRIPT_SRC_PARSER_RNASTSYMBOLTABLE_H_

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

#endif  //RONASCRIPT_SRC_PARSER_RNASTSYMBOLTABLE_H_