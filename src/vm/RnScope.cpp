/*****************************************************************************
* File: RnScope.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/20/22
* Version: 1
*
******************************************************************************/

#include <iostream>
#include "RnScope.h"
#include "RnSymbolTable.h"
#include "RnMemoryManager.h"
#include "RnObject.h"
#include "RnFunctionObject.h"
#include "RnFunction.h"
#include <dlfcn.h>

std::map<std::string, void*> RnScope::_handles;

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

/*****************************************************************************/
void RnScope::LoadLibraryIntoScope(RnScope* scope, const std::string& library)
{
	// This function should really be moved somewhere more appropriate and
	// should do something other than just load the names into the parent scope.
	// It would be ideal to return an object containing the scope
	auto CastToBuiltin = [](auto f)
	{
	  return reinterpret_cast<BuiltinFunction>(f);
	};

	void* handle = dlopen(library.c_str(), RTLD_LOCAL);
	_handles[library] = handle;

	if (handle)
	{
		void (* ListExports)(
			std::vector<std::tuple<std::string, RnType::Type>>&) = nullptr;
		ListExports =
			(void (*)(std::vector<std::tuple<std::string, RnType::Type>>&))dlsym(handle,
				"LibraryFunctions");
		std::vector<std::tuple<std::string, RnType::Type>> functions;
		ListExports(functions);
		for (const auto& info : functions)
		{
			auto name = std::get<0>(info);
			std::cout << "Loading external function: " << std::get<0>(info)
					  << std::endl;

			auto func =
				new RnBuiltinFunction(name, CastToBuiltin(dlsym(handle, name.c_str())));
			func->SetScope(scope);
			auto obj =
				dynamic_cast<RnFunctionObject*>(RnObject::Create(RnType::RN_FUNCTION));
			obj->SetReturnType(std::get<1>(info));
			obj->SetData(func);
			scope->StoreObject(name, obj);
		}
	}
	else
	{
		std::cout << "Unable to load external library: " << library << std::endl;
	}
}


