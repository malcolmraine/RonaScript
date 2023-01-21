/*****************************************************************************
* File: RnSymbolTable.h
* Description:
* Author: Malcolm Hall
* Date: 5/24/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNSYMBOLTABLE_H
#define RONASCRIPT_RNSYMBOLTABLE_H

#include <vector>
#include <unordered_map>
#include <string>

class RnMemoryGroup;

class RnObject;

class RnSymbolTable
{
 public:
	explicit RnSymbolTable(RnSymbolTable* parent_table = nullptr);
	~RnSymbolTable();
	bool SetSymbol(const std::string& symbol, RnObject* obj);
	RnObject* GetObject(const std::string& symbol);
	void RemoveSymbol(const std::string& symbol);
	void AliasSymbol(const std::string& base, const std::string& alias);
	[[nodiscard]] bool SymbolExists(const std::string& symbol,
		bool search_parent = true) const;
	void SetMemoryGroup(RnMemoryGroup* group);
	void SetParent(RnSymbolTable* parent);
	[[nodiscard]] RnSymbolTable* GetParent() const;
	std::vector<std::string> GetSymbols() const;

 private:
	RnSymbolTable* _parent_table = nullptr;
	RnMemoryGroup* _memory_group = nullptr;
	std::unordered_map<std::string, RnObject*> _table;

};

#endif //RONASCRIPT_RNSYMBOLTABLE_H