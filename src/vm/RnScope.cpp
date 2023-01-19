/*****************************************************************************
* File: RnScope.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/20/22
* Version: 1
*
******************************************************************************/

#include "RnScope.h"
#include "RnSymbolTable.h"
#include "RnMemoryManager.h"

/*****************************************************************************/
RnScope::RnScope(RnScope* parent)
{
	_parent = parent;
//	RnSymbolTable parent_table;
	RnMemoryGroup* parent_memory_group = nullptr;
	if (parent)
	{
		_symbolTable.SetParent(&parent->_symbolTable);
		parent_memory_group = parent->GetMemoryGroup();
	}
//	_symbolTable = RnSymbolTable(parent_table);
	_memory_group = new RnMemoryGroup(parent_memory_group);
	_symbolTable.SetMemoryGroup(_memory_group);
//	_stack = new std::vector<RnObject*>();
	_stack.reserve(10);
}

/*****************************************************************************/
RnScope::~RnScope()
{
	if (_memory_group->GetParent())
	{
		_memory_group->GetParent()->RemoveChildGroup(_memory_group);
	}
	delete _memory_group;
//	delete _symbolTable;
//	_stack.clear();
//	delete _stack;
}

/*****************************************************************************/
void RnScope::StoreObject(const std::string& name, RnObject* value)
{
	_symbolTable.SetSymbol(name, value);
}

/*****************************************************************************/
RnObject* RnScope::GetObject(const std::string& name)
{
	return _symbolTable.GetObject(name);
}

/*****************************************************************************/
void RnScope::RemoveObject(const std::string& name)
{
	_symbolTable.RemoveSymbol(name);
}

/*****************************************************************************/
RnSymbolTable* RnScope::GetSymbolTable()
{
	return &_symbolTable;
}

/*****************************************************************************/
std::vector<RnObject*>& RnScope::GetStack()
{
	return _stack;
}

/*****************************************************************************/
void RnScope::SetParent(RnScope* scope)
{
	_parent = scope;
	if (_parent)
	{
		_symbolTable.SetParent(_parent->GetSymbolTable());
		_memory_group->SetParent(_parent->GetMemoryGroup());
	}
}

/*****************************************************************************/
RnScope* RnScope::GetParent() const
{
	return _parent;
}

/*****************************************************************************/
RnMemoryGroup* RnScope::GetMemoryGroup() const
{
	return _memory_group;
}


