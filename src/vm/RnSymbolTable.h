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
#include "RnType.h"

class RnMemoryGroup;

class RnObject;

class RnSymbolTable
{
 public:
	explicit RnSymbolTable(RnSymbolTable* parent_table = nullptr);
	~RnSymbolTable();
	bool SetSymbol(RnIntNative symbol, RnObject* obj);
	RnObject* GetObject(RnIntNative symbol);
	void RemoveSymbol(RnIntNative symbol);
	void AliasSymbol(RnIntNative base, RnIntNative alias);
	[[nodiscard]] bool SymbolExists(RnIntNative key,
		bool search_parent = true) const;
	void SetMemoryGroup(RnMemoryGroup* group);
	void SetParent(RnSymbolTable* parent);
	[[nodiscard]] RnSymbolTable* GetParent() const;
	[[nodiscard]] std::vector<RnIntNative> GetSymbols() const;

 private:
	RnSymbolTable* _parent_table = nullptr;
	RnMemoryGroup* _memory_group = nullptr;
	std::unordered_map<RnIntNative, RnObject*> _table;

};

#endif //RONASCRIPT_RNSYMBOLTABLE_H