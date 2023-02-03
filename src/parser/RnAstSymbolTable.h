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

private:
    std::string _name;
    std::shared_ptr<RnTypeComposite> _type;
};

class RnAstSymbolTable {
public:
    explicit RnAstSymbolTable(RnAstSymbolTable* parent = nullptr);
    bool SymbolExists(const std::string& symbol);
    std::shared_ptr<SymbolTableEntry> AddSymbol(const std::string& symbol,
                                                std::shared_ptr<RnTypeComposite> type);
    std::shared_ptr<SymbolTableEntry> GetSymbolEntry(const std::string& symbol);

    void SetParent(std::shared_ptr<RnAstSymbolTable> parent) {
        _parent = std::move(parent);
    }

private:
    std::map<std::string, std::shared_ptr<SymbolTableEntry>> _table;
    std::shared_ptr<RnAstSymbolTable> _parent = nullptr;
};

#endif  //RONASCRIPT_SRC_PARSER_RNASTSYMBOLTABLE_H_