/*****************************************************************************
* File: RnSymbolTable.cpp
* Description:
* Author: Malcolm Hall
* Date: 5/24/22
* Version: 1
*
******************************************************************************/

#include "RnSymbolTable.h"
#include "RnObject.h"
#include "RnMemoryGroup.h"

/*****************************************************************************/
RnSymbolTable::RnSymbolTable(RnSymbolTable* parent_table)
{
	SetParent(parent_table);
}

/*****************************************************************************/
RnSymbolTable::~RnSymbolTable()
{
	_parent_table = nullptr;
}

/*****************************************************************************/
bool RnSymbolTable::SetSymbol(const std::string& symbol, RnObject* obj)
{
	if (_table.find(symbol) != _table.end())
	{
		_table[symbol] = obj;
		return true;
	}
	else if (_parent_table && _parent_table->SymbolExists(symbol))
	{
		if (!_parent_table->SetSymbol(symbol, obj))
		{
			return true;
		}
	}
	_table[symbol] = obj;
	return true;

}

/*****************************************************************************/
RnObject* RnSymbolTable::GetObject(const std::string& symbol)
{
	auto iter = _table.find(symbol);
	if (iter != _table.end())
	{
		return iter->second;
	}
	else if (_parent_table)
	{
		return _parent_table->GetObject(symbol);
	}
	else
	{
		throw std::runtime_error("Symbol not found: " + symbol);
	}
}

/*****************************************************************************/
void RnSymbolTable::RemoveSymbol(const std::string& symbol)
{
	auto iter = _table.find(symbol);
	if (iter != _table.end())
	{
		_table.erase(iter);
	}
	else if (_parent_table)
	{
		_parent_table->RemoveSymbol(symbol);
	}
	else
	{
		throw std::runtime_error(
			"Attempting to delete unknown symbol '" + symbol + "'");
	}
}

/*****************************************************************************/
void RnSymbolTable::AliasSymbol(const std::string& base, const std::string& alias)
{
	auto obj = GetObject(base);
	_table[alias] = obj;
}

/*****************************************************************************/
bool RnSymbolTable::SymbolExists(const std::string& symbol, bool search_parent) const
{
	if (_table.contains(symbol))
	{
		return true;
	}
	else if (_parent_table && search_parent)
	{
		return _parent_table->SymbolExists(symbol);
	}
	return false;
}

/*****************************************************************************/
void RnSymbolTable::SetMemoryGroup(RnMemoryGroup* group)
{
	_memory_group = group;
}

/*****************************************************************************/
void RnSymbolTable::SetParent(RnSymbolTable* parent)
{
	assert(parent != this);
	_parent_table = parent;
}

/*****************************************************************************/
RnSymbolTable* RnSymbolTable::GetParent() const
{
	return _parent_table;
}

/*****************************************************************************/
std::vector<std::string> RnSymbolTable::GetSymbols() const
{
	std::vector<std::string> result;
	result.reserve(_table.size());
	for (const auto& pair : _table)
	{
		result.push_back(pair.first);
	}
	return result;
}
