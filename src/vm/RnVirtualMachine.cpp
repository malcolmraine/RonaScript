/*****************************************************************************
* File: RnVirtualMachine.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/19/22
* Version: 1
*
*****************************************************************************/

#include "RnVirtualMachine.h"
#include "RnOpCode.h"
#include "RnBoolObject.h"
#include "RnSymbolTable.h"
#include "RnArrayObject.h"
#include "RnStringObject.h"
#include "RnFunctionObject.h"
#include "RnMemoryManager.h"
#include "RnFunction.h"
#include "RnClass.h"
#include "RnClassObject.h"
#include "builtins/RnBuiltins.h"
#include "builtins/RnBuiltins_Math.h"
#include "builtins/RnBuiltins_Array.h"
#include "builtins/RnBuiltins_IO.h"
#include "builtins/RnBuiltins_String.h"
#include "builtins/RnBuiltins_Time.h"
#include "builtins/RnBuiltins_Type.h"
#include <iostream>
#include <tuple>
#include <vector>
#include "../util/MLib/StopWatch.h"

/*****************************************************************************/
RnVirtualMachine::RnVirtualMachine()
{
	_scopes.reserve(16);
	_call_stack.reserve(50);
	_memory_manager = new RnMemoryManager();
	AddScope();
	_memory_manager->SetRootMemoryGroup(GetScope()->GetMemoryGroup());
	RegisterBuiltins();
	_int_holder = RnObject::Create(RnType::RN_INT);
	_float_holder = RnObject::Create(RnType::RN_FLOAT);
	_string_holder = RnObject::Create(RnType::RN_STRING);
	_bool_holder = RnObject::Create(RnType::RN_BOOLEAN);
}

/*****************************************************************************/
RnVirtualMachine::~RnVirtualMachine()
{
	for (auto instruction : _instructions)
	{
		delete instruction;
	}
	_instructions.clear();

	for (size_t i = 0; i < _scopes.size(); i++)
	{
		auto obj = _scopes.back();
		delete obj;
		_scopes.pop_back();
	}
	_scopes.clear();
	_call_stack.clear();

	delete _memory_manager;
}

/*****************************************************************************/
void RnVirtualMachine::CallFunction(RnFunctionObject* obj, uint32_t arg_cnt)
{
	std::vector<RnObject*> args;
	auto func = obj->GetData();
	args.reserve(arg_cnt);
	for (uint32_t i = 0; i < arg_cnt; i++)
	{
		args.insert(args.begin(), GetStack().back());
		GetStack().pop_back();
	}
	RnObject* ret_val = RnObject::Create(obj->GetReturnType());

	if (func->IsBuiltIn())
	{
		func->Call(args, ret_val);
		if (obj->GetReturnType() != RnType::RN_NULL)
		{
			GetStack().push_back(ret_val);
		}
	}
	else
	{
		_call_stack.push_back(func->GetScope());
		_scopes.push_back(func->GetScope());

		func->PassArguments(args);
		bool has_returned = false;
		size_t func_index = func->GetIStart();
		size_t end_index = func->GetIStart() + func->GetICnt();
		for (; func_index < end_index; func_index++)
		{
			ExecuteInstruction(_instructions[func_index], has_returned, func_index);
			if (has_returned)
			{
				break;
			}
		}

		_call_stack.pop_back();
//		auto scope = _scopes.back();
//		delete scope;
		_scopes.pop_back();


		//auto call_frame = _call_stack.back();
		//delete call_frame;


		if (func->GetName() == "construct")
		{
			GetStack().push_back(func->GetScope()->GetObject("this"));
		}
		func->Reset();
	}
}

/*****************************************************************************/
void RnVirtualMachine::AddScope()
{
	auto scope = new RnScope(nullptr);
	if (!_scopes.empty())
	{
		scope->SetParent(GetScope());
	}
	_scopes.push_back(scope);
}

/*****************************************************************************/
void RnVirtualMachine::ExecuteInstruction(RnInstruction* instruction, bool& break_scope,
	size_t& index)
{
	_gc_count++;
	if (_gc_count > 1000)
	{
//		_memory_manager->GCMark();
//		_memory_manager->GCSweep();
		_gc_count = 0;
	}

	switch (instruction->_opcode)
	{
	case OP_BINARY_ADD:
	{
		auto b = GetStack().back();
		GetStack().pop_back();
		auto a = GetStack().back();
		GetStack().pop_back();
		auto result = *a + b;
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), result);
		GetStack().push_back(result);
		break;
	}
	case OP_BINARY_SUB:
	{
		auto b = GetStack().back();
		GetStack().pop_back();
		auto a = GetStack().back();
		GetStack().pop_back();
		auto result = *a - b;
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), result);
		GetStack().push_back(result);
		break;
	}
	case OP_BINARY_MUL:
	{
		auto b = GetStack().back();
		GetStack().pop_back();
		auto a = GetStack().back();
		GetStack().pop_back();
		auto result = *a * b;
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), result);
		GetStack().push_back(result);
		break;
	}
	case OP_BINARY_DIV:
	{
		auto b = GetStack().back();
		GetStack().pop_back();
		auto a = GetStack().back();
		GetStack().pop_back();
		auto result = *a / b;
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), result);
		GetStack().push_back(result);
		break;
	}
	case OP_BINARY_MOD:
	{
		auto b = GetStack().back();
		GetStack().pop_back();
		auto a = GetStack().back();
		GetStack().pop_back();
		auto result = *a % b;
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), result);
		GetStack().push_back(result);
		break;
	}
	case OP_BINARY_GTE:
	{
		auto b = GetStack().back();
		GetStack().pop_back();
		auto a = GetStack().back();
		GetStack().pop_back();
		auto result = *a >= b;
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), result);
		GetStack().push_back(result);
		break;
	}
	case OP_BINARY_LTE:
	{
		auto b = GetStack().back();
		GetStack().pop_back();
		auto a = GetStack().back();
		GetStack().pop_back();
		auto result = *a <= b;
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), result);
		GetStack().push_back(result);
		break;
	}
	case OP_BINARY_GT:
	{
		auto b = GetStack().back();
		GetStack().pop_back();
		auto a = GetStack().back();
		GetStack().pop_back();
		auto result = *a > b;
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), result);
		GetStack().push_back(result);
		break;
	}
	case OP_BINARY_LT:
	{
		auto b = GetStack().back();
		GetStack().pop_back();
		auto a = GetStack().back();
		GetStack().pop_back();
		auto result = *a < b;
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), result);
		GetStack().push_back(result);
		break;
	}
	case OP_BINARY_EQ:
	{
		auto b = GetStack().back();
		GetStack().pop_back();
		auto a = GetStack().back();
		GetStack().pop_back();
		auto result = *a == b;
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), result);
		GetStack().push_back(result);
		break;
	}
	case OP_BINARY_POWER:
	{
		break;
	}
	case OP_BINARY_RSH:
	{
		auto b = GetStack().back();
		GetStack().pop_back();
		auto a = GetStack().back();
		GetStack().pop_back();
		auto result = *a << b;
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), result);
		GetStack().push_back(result);
		break;
	}
	case OP_BINARY_LSH:
	{
		auto b = GetStack().back();
		GetStack().pop_back();
		auto a = GetStack().back();
		GetStack().pop_back();
		auto result = *a >> b;
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), result);
		GetStack().push_back(result);
		break;
	}
	case OP_BINARY_OR:
	{
		auto b = GetStack().back();
		GetStack().pop_back();
		auto a = GetStack().back();
		GetStack().pop_back();
		auto result = *a | b;
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), result);
		GetStack().push_back(result);
		break;
	}
	case OP_BINARY_XOR:
	{
		auto b = GetStack().back();
		GetStack().pop_back();
		auto a = GetStack().back();
		GetStack().pop_back();
		auto result = *a ^ b;
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), result);
		GetStack().push_back(result);
		break;
	}
	case OP_BINARY_AND:
	{
		auto b = GetStack().back();
		GetStack().pop_back();
		auto a = GetStack().back();
		GetStack().pop_back();
		auto result = *a & b;
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), result);
		GetStack().push_back(result);
		break;
	}
	case OP_LOGICAL_OR:
	{
		auto b = GetStack().back();
		GetStack().pop_back();
		auto a = GetStack().back();
		GetStack().pop_back();
		auto result = *a || b;
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), result);
		GetStack().push_back(result);
		break;
	}
	case OP_LOGICAL_AND:
	{
		auto b = GetStack().back();
		GetStack().pop_back();
		auto a = GetStack().back();
		GetStack().pop_back();
		auto result = *a && b;
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), result);
		GetStack().push_back(result);
		break;
	}
	case OP_STORE:
	{
		auto obj = GetStack().back();
		GetStack().pop_back();
		auto value = GetStack().back();
		GetStack().pop_back();
		obj->CopyDataFromObject(value);
//		GetScope()->StoreObject(name->ToString(), value);
		break;
	}
	case OP_POP:
	{
		GetStack().pop_back();
		break;
	}
	case OP_UNARY_NOT:
	{
		auto name = RnObject::GetInternedString(instruction->_arg1);
		auto obj = GetScope()->GetObject(name);
		obj->SetData(!obj->ToBool());
		break;
	}
	case OP_BREAK:
	{
		break;
	}
	case OP_CONTINUE:
	{
		break;
	}
	case OP_TRY_CONTEXT:
	{
		break;
	}
	case OP_UNARY_INVERT:
	{
		break;
	}
	case OP_UNARY_DECREMENT:
	{
		auto name = RnObject::GetInternedString(instruction->_arg1);
		auto obj = GetScope()->GetObject(name);
		obj->SetData(obj->ToFloat() - 1);
		break;
	}
	case OP_UNARY_INCREMENT:
	{
		auto name = RnObject::GetInternedString(instruction->_arg1);
		auto obj = GetScope()->GetObject(name);
		obj->SetData(obj->ToFloat() + 1);
		break;
	}
	case OP_UNARY_POWER:
	{
		break;
	}
	case OP_UNARY_NEGATION:
	{
		break;
	}
	case OP_RETURN:
	{
		auto function_scope = _call_stack.back();
		if (!function_scope)
		{
			throw std::runtime_error("Cannot return outside of a function");
		}
		// Have to get the parent of the working scope, not the argument scope
		auto ret_scope = function_scope->GetParent()->GetParent();
		ret_scope->GetStack().push_back(GetStack().back());
		GetStack().pop_back();
		break_scope = true;
		break;
	}
	case OP_LOAD_INT:
	{
		auto value = RnObject::GetInternedInt(instruction->_arg1);
		_int_holder->SetData(value);
//		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), obj);
		GetStack().push_back(_int_holder);
		break;
	}
	case OP_LOAD_FLOAT:
	{
		auto value = RnObject::GetInternedFloat(instruction->_arg1);
//		auto obj = RnObject::Create(value);
//		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), obj);
		_float_holder->SetData(value);
		GetStack().push_back(_float_holder);
		break;
	}
	case OP_LOAD_STRING:
	{
		auto value = RnObject::GetInternedString(instruction->_arg1);
//		auto obj = RnObject::Create(value);
//		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), obj);
		_string_holder->SetData(value);
		GetStack().push_back(_string_holder);
		break;
	}
	case OP_LOAD_NAME:
//	{
//		auto name = RnObject::GetInternedString(instruction->_arg1);
//		auto obj = new RnStringObject(name);
//		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), obj);
//		GetStack().push_back(obj);
//		break;
//	}
	case OP_LOAD_VALUE:
	{
		auto name = RnObject::GetInternedString(instruction->_arg1);
		if (GetScope()->GetSymbolTable()->SymbolExists(name))
		{
			auto object = GetScope()->GetObject(name);
			GetStack().push_back(object);
		}
		else if (_namespaces.contains(name))
		{
			auto class_obj = dynamic_cast<RnClass*>(_namespaces[name]);
			auto instance =
				dynamic_cast<RnClassObject*>(RnObject::Create(RnType::RN_CLASS_INSTANCE));
			_memory_manager->AddObject(GetScope()->GetMemoryGroup(), instance);
			instance->GetScope()->SetParent(class_obj);
			class_obj->CopySymbols(instance->GetScope());
			instance->GetScope()->StoreObject("this", instance);
			auto func_obj =
				dynamic_cast<RnFunctionObject*>(class_obj->GetObject("construct"));
			auto func = func_obj->ToFunction();

			func->SetScope(new RnScope(instance->GetScope()));
			GetStack().push_back(func_obj);
		}
		else
		{
			throw std::runtime_error("Symbol does not exist: " + name);
		}

		break;
	}
	case OP_LOAD_NULL:
	{
		GetStack().push_back(RnObject::Create(RnType::RN_NULL));
		break;
	}
	case OP_LOAD_BOOL:
	{
		auto value = static_cast<bool>(instruction->_arg1);
		auto obj = new RnBoolObject(value);
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), obj);
		GetStack().push_back(obj);
		break;
	}
	case OP_CALL:
	{
		auto func_obj = dynamic_cast<RnFunctionObject*>(GetStack().back());
		GetStack().pop_back();
		CallFunction(func_obj, instruction->_arg1);
		break;
	}
	case OP_MAKE_CONST:
	{
		auto type = static_cast<RnType::Type>(instruction->_arg1);
		auto name = RnObject::GetInternedString(instruction->_arg2);

		if (GetScope()->GetSymbolTable()->SymbolExists(name))
		{
			throw std::runtime_error("Redeclaration of symbol '" + name + "'");
		}

		auto obj = RnObject::Create(type);
		obj->SetConstFlag(true);
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), obj);
		GetScope()->StoreObject(name, obj);
		break;
	}
	case OP_MAKE_VAR:
	{
		auto type = static_cast<RnType::Type>(instruction->_arg1);
		auto name = RnObject::GetInternedString(instruction->_arg2);

		if (GetScope()->GetSymbolTable()->SymbolExists(name, false))
		{
			throw std::runtime_error("Redeclaration of symbol '" + name + "'");
		}

		auto obj = RnObject::Create(type);
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), obj);
		GetScope()->StoreObject(name, obj);
		break;
	}
	case OP_MAKE_CLASS:
	{
		auto name = RnObject::GetInternedString(instruction->_arg1);
		auto obj = new RnClass(GetScope());
		obj->SetName(name);
		_namespaces[name] = obj;
		_scopes.push_back(obj);
		index++;
		size_t stop_index = index + instruction->_arg2;
		for (; index < stop_index; index++)
		{
			ExecuteInstruction(_instructions[index], break_scope, index);
		}
		index--;

//		auto scope = _scopes.back();
//		delete scope;
		_scopes.pop_back();
//		index += instruction->_arg2;
		break;
	}
	case OP_MAKE_FUNC:
	{
		auto obj =
			dynamic_cast<RnFunctionObject*>(RnObject::Create(RnType::RN_FUNCTION));
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), obj);
		auto name = RnObject::GetInternedString(instruction->_arg1);
		if (GetScope()->GetSymbolTable()->SymbolExists(name))
		{
			throw std::runtime_error("Redeclaration of symbol '" + name + "'");
		}

		auto func =
			new RnFunction(name, index + 1 + instruction->_arg2, instruction->_arg3);
		obj->SetData(func);
		func->SetScope(new RnScope(GetScope()));

		for (uint32_t i = 0; i < instruction->_arg2; i++)
		{
			auto arg_instruction = _instructions[i + index + 1];
			func->CreateArgument(RnObject::GetInternedString(arg_instruction->_arg2),
				static_cast<RnType::Type>(arg_instruction->_arg1),
				i);
		}
		index += instruction->_arg2 + func->GetICnt();
		GetScope()->StoreObject(func->GetName(), obj);
		break;
	}
	case OP_MAKE_MODULE:
	{

		break;
	}
	case OP_CREATE_CONTEXT:
	{
		AddScope();
		break;
	}
	case OP_DESTROY_CONTEXT:
	{
		auto scope = _scopes.back();
		_scopes.pop_back();
		delete scope;
		break;
	}
	case OP_DELETE:
	{
		auto name = RnObject::GetInternedString(instruction->_arg1);
		GetScope()->GetSymbolTable()->RemoveSymbol(name);
		break;
	}
	case OP_NOP:
	{
		break;
	}
	case OP_JUMPF:
	{
		index += instruction->_arg1;
		break;
	}
	case OP_JUMPB:
	{
		index -= instruction->_arg1 + 1;
		break;
	}
	case OP_JUMPF_IF:
	{
		auto steps = instruction->_arg1;
		auto condition = GetStack().back()->ToInt();
		GetStack().pop_back();
		if (!condition)
		{
			index += steps;
		}
		break;
	}
	case OP_JUMPB_IF:
	{
		auto steps = instruction->_arg1;
		auto condition = GetStack().back()->ToInt();
		GetStack().pop_back();
		if (!condition)
		{
			index -= steps;
		}
		break;
	}
	case OP_COMPARE:
	{
		break;
	}
	case OP_EXIT:
	{
		std::cout << "Exit " << instruction->_arg1;
		break_scope = true;
		break;
	}
	case OP_ARRAY_STORE:
	{
		// TODO: Remove OP_ARRAY_STORE
		break;
	}
	case OP_INDEX_ACCESS:
	{
		auto idx_value = GetStack().back()->ToInt();
		GetStack().pop_back();
		auto obj = dynamic_cast<RnArrayObject*>(GetStack().back());
		GetStack().pop_back();
		auto result = obj->ToList().at(idx_value);
		GetStack().push_back(result);
		break;
	}
	case OP_MAKE_ARG:
	{
		// Do nothing. These instructions are consumed automatically
		// by the MAKE_FUNC handler
		break;
	}
	case OP_MAKE_ALIAS:
	{
		auto alias = RnObject::GetInternedString(instruction->_arg2);
		auto base = RnObject::GetInternedString(instruction->_arg1);
		GetScope()->GetSymbolTable()->AliasSymbol(base, alias);
		break;
	}
	case OP_MAKE_ARRAY:
	{
		auto obj = new RnArrayObject();
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), obj);

		for (RnIntNative i = 0; i < instruction->_arg1; i++)
		{
			obj->Append(GetStack().back());
			GetStack().pop_back();
		}
		GetStack().push_back(obj);
		break;
	}
	case OP_ATTR_ACCESS :
	{
		auto object = dynamic_cast<RnClassObject*>(GetStack().back());
		GetStack().pop_back();
		auto name = RnObject::GetInternedString(instruction->_arg1);
		auto scope = object->GetScope();
		if (scope->GetSymbolTable()->SymbolExists(name, false))
		{
			auto result = scope->GetObject(name);
			GetStack().push_back(result);
		} else if (scope->GetParent()->GetSymbolTable()->SymbolExists(name, false)) {
			auto result = scope->GetParent()->GetObject(name);
			GetStack().push_back(result);
		}
		else
		{
			throw std::runtime_error("Object has no attribute '" + name + "'");
		}
		break;
	}
	case OP_RESOLVE_NAMESPACE:
		break;
	}
}

/*****************************************************************************/
void RnVirtualMachine::Run()
{
	bool has_returned = false; // Placeholder
	auto stopwatch = StopWatch();
	stopwatch.Start();
	while (i_idx < _instructions.size())
	{
		ExecuteInstruction(_instructions[i_idx], has_returned, i_idx);
		if (has_returned) {
			break;
		}
		i_idx++;
	}
	stopwatch.Stop();
	std::cout << "\nRuntime duration: " << stopwatch.Duration() << std::endl;
	std::cout << "Heap size: " << _memory_manager->GetHeapCount() * sizeof(RnObject) * 2;
}

/*****************************************************************************/
void RnVirtualMachine::RegisterBuiltins()
{
	auto CastToBuiltin = [](auto f)
	{
	  return reinterpret_cast<BuiltinFunction>(f);
	};
	// Would be nice to have each of these in separate namespaces based on their
	// category
	std::vector<std::tuple<std::string, BuiltinFunction, RnType::Type>> functions =
		{{ "print", CastToBuiltin(&RnBuiltins_IO::rn_builtin_print), RnType::RN_NULL },
		 { "read", CastToBuiltin(&RnBuiltins_IO::read), RnType::RN_STRING },
		 { "file_read", CastToBuiltin(&RnBuiltins_IO::read), RnType::RN_STRING },
		 { "file_write", CastToBuiltin(&RnBuiltins_IO::file_write),
		   RnType::RN_BOOLEAN },
		 { "prompt", CastToBuiltin(&RnBuiltins_IO::rn_builtin_prompt),
		   RnType::RN_STRING },
		 { "file_size", CastToBuiltin(&RnBuiltins_IO::file_size), RnType::RN_INT },
		 { "sum", CastToBuiltin(&RnBuiltins_Math::rn_builtin_sum), RnType::RN_FLOAT },
		 { "pow", CastToBuiltin(&RnBuiltins_Math::pow), RnType::RN_FLOAT },
		 { "is_int", CastToBuiltin(&RnBuiltins_Type::rn_builtin_is_int),
		   RnType::RN_BOOLEAN },
		 { "is_float", CastToBuiltin(&RnBuiltins_Type::rn_builtin_is_float),
		   RnType::RN_BOOLEAN },
		 { "is_array", CastToBuiltin(&RnBuiltins_Type::rn_builtin_is_array),
		   RnType::RN_BOOLEAN },
		 { "is_object", CastToBuiltin(&RnBuiltins_Type::rn_builtin_is_object),
		   RnType::RN_BOOLEAN },
		 { "is_bool", CastToBuiltin(&RnBuiltins_Type::rn_builtin_is_bool),
		   RnType::RN_BOOLEAN },
		 { "is_callable", CastToBuiltin(&RnBuiltins_Type::rn_builtin_is_callable),
		   RnType::RN_BOOLEAN },
		 { "str_merge", CastToBuiltin(&RnBuiltins_String::rn_builtin_str_merge),
		   RnType::RN_STRING },
		 { "array_merge", CastToBuiltin(&RnBuiltins_Array::rn_builtin_array_merge),
		   RnType::RN_ARRAY },
		 { "count", CastToBuiltin(&RnBuiltins_Array::rn_builtin_count),
		   RnType::RN_INT },
		 { "system", CastToBuiltin(&RnBuiltins::rn_builtin_system), RnType::RN_ARRAY },
		 { "lload", CastToBuiltin(&RnBuiltins::lload), RnType::RN_NULL },
		 { "count", CastToBuiltin(&RnBuiltins_Array::rn_builtin_count),
		   RnType::RN_INT },
		 { "array_push", CastToBuiltin(&RnBuiltins_Array::rn_builtin_array_push),
		   RnType::RN_NULL }, };

	for (auto parts : functions)
	{
		auto func = new RnBuiltinFunction(std::get<0>(parts), std::get<1>(parts));
		func->SetScope(GetScope());
		auto obj =
			dynamic_cast<RnFunctionObject*>(RnObject::Create(RnType::RN_FUNCTION));
		_memory_manager->AddObject(GetScope()->GetMemoryGroup(), obj);
		obj->SetReturnType(std::get<2>(parts));
		obj->SetData(func);
		GetScope()->StoreObject(std::get<0>(parts), obj);
	}

}

