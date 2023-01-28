//
// Created by Malcolm Hall on 1/27/23.
//

#ifndef RONASCRIPT_SRC_PARSER_RNASTSYMBOLTABLE_H_
#define RONASCRIPT_SRC_PARSER_RNASTSYMBOLTABLE_H_

#include <string>
#include <unordered_map>
#include <vector>
#include "ast/AstNode.h"
#include "RnTypeComposite.h"
#include <memory>

struct SymbolTableEntry {
	std::string name;
	RnTypeComposite type;
};

class RnAstSymbolTable
{
 public:
	RnAstSymbolTable(RnAstSymbolTable* parent = nullptr);

	bool SymbolExists(const std::string& symbol);
	std::shared_ptr<SymbolTableEntry> AddSymbol(const std::string& symbol);
	std::shared_ptr<SymbolTableEntry> GetSymbolEntry(const std::string& symbol);

 private:
	std::map<std::string, std::shared_ptr<SymbolTableEntry>> _table;
	RnAstSymbolTable* _parent = nullptr;
};

#endif //RONASCRIPT_SRC_PARSER_RNASTSYMBOLTABLE_H_