//
// Created by Malcolm Hall on 1/27/23.
//

#ifndef RONASCRIPT_SRC_PARSER_RNASTSYMBOLTABLE_H_
#define RONASCRIPT_SRC_PARSER_RNASTSYMBOLTABLE_H_

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "ast/AstNode.h"
#include "RnTypeComposite.h"
#include <memory>

class SymbolTableEntry
{
 public:
	SymbolTableEntry(const std::string& name, RnType::Type type) {
		_name = name;
		_type = std::make_unique<RnTypeComposite>(type);
	}

	RnType::Type GetType() {
		return _type->GetType();
	}

	std::string GetSymbol() {
		return _name;
	}

 private:
	std::string _name;
	std::unique_ptr<RnTypeComposite> _type;
};

class RnAstSymbolTable
{
 public:
	RnAstSymbolTable(RnAstSymbolTable* parent = nullptr);

	bool SymbolExists(const std::string& symbol);
	std::shared_ptr<SymbolTableEntry> AddSymbol(const std::string& symbol, RnType::Type type);
	std::shared_ptr<SymbolTableEntry> GetSymbolEntry(const std::string& symbol);
	void SetParent(std::shared_ptr<RnAstSymbolTable> parent) {
		_parent = std::move(parent);
	}


 private:
	std::map<std::string, std::shared_ptr<SymbolTableEntry>> _table;
	std::shared_ptr<RnAstSymbolTable> _parent = nullptr;
};

#endif //RONASCRIPT_SRC_PARSER_RNASTSYMBOLTABLE_H_