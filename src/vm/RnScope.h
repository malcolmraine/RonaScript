/*****************************************************************************
* File: RnScope.h
* Description:
* Author: Malcolm Hall
* Date: 6/20/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNSCOPE_H
#define RONASCRIPT_RNSCOPE_H

#include <vector>
#include "../util/RnStack.h"
#include "RnScope.h"
#include "RnMemoryGroup.h"
#include "RnSymbolTable.h"

class RnScope
{
 public:
	explicit RnScope(RnScope* parent);
	virtual ~RnScope();
	void StoreObject(const std::string& name, RnObject* value);
	[[nodiscard]] RnObject* GetObject(const std::string& name) ;
	void RemoveObject(const std::string& name);
	[[nodiscard]] RnSymbolTable* GetSymbolTable();
	[[nodiscard]] std::vector<RnObject*>& GetStack();
	void SetParent(RnScope* scope);
	[[nodiscard]] RnScope* GetParent() const;
	[[nodiscard]] RnMemoryGroup* GetMemoryGroup() const;

 protected:
	RnScope* _parent = nullptr;
	std::vector<RnObject*> _stack;
	RnSymbolTable _symbolTable;
	RnMemoryGroup* _memory_group = nullptr;
};

#endif //RONASCRIPT_RNSCOPE_H