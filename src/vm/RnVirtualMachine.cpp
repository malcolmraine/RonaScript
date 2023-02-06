/*****************************************************************************
 * File: RnVirtualMachine.cpp
 * Description:
 * Author: Malcolm Hall
 * Date: 6/19/22
 * Version: 1
 *
 *****************************************************************************/

#include "RnVirtualMachine.h"

#include <tuple>
#include <utility>
#include <vector>

#include "../builtins/RnBuiltins.h"
#include "../builtins/RnBuiltins_Array.h"
#include "../builtins/RnBuiltins_IO.h"
#include "../builtins/RnBuiltins_Math.h"
#include "../builtins/RnBuiltins_String.h"
#include "../builtins/RnBuiltins_Type.h"
#include "../util/MLib/StopWatch.h"
#include "../util/log.h"
#include "RnArrayObject.h"
#include "RnClassObject.h"
#include "RnFunction.h"
#include "RnFunctionObject.h"
#include "RnMemoryManager.h"
#include "RnOpCode.h"
#include "RnSymbolTable.h"

RnVirtualMachine* RnVirtualMachine::_instance = nullptr;
RnIntNative RnVirtualMachine::_object_this_key = 0;
RnIntNative RnVirtualMachine::_object_construct_key = 0;

/*****************************************************************************/
RnVirtualMachine::RnVirtualMachine() {
    _scopes.reserve(16);
    _call_stack.reserve(50);
    _memory_manager = new RnMemoryManager();

    _object_this_key = RnObject::InternValue("this");
    _object_construct_key = RnObject::InternValue("construct");
}

/*****************************************************************************/
void RnVirtualMachine::Init() {
    auto obj = RnObject::Create(RnType::RN_OBJECT);
    auto scope = obj->ToObject();
    scope->GetMemoryGroup()->AddObject(obj);
    if (!_scopes.empty()) {
        scope->SetParent(GetScope());
    }
    _scopes.push_back(scope);
    _memory_manager->SetRootMemoryGroup(GetScope()->GetMemoryGroup());
    RegisterBuiltins();
}

/*****************************************************************************/
RnVirtualMachine::~RnVirtualMachine() {
    for (auto instruction : _instructions) {
        delete instruction;
    }
    _instructions.clear();
    _scopes.clear();
    _call_stack.clear();

    delete _memory_manager;
}

/*****************************************************************************/
void RnVirtualMachine::CallFunction(RnFunctionObject* obj, uint32_t arg_cnt) {
    std::vector<RnObject*> args;
    auto func = obj->GetData();
    args.reserve(arg_cnt);
    for (uint32_t i = 0; i < arg_cnt; i++) {
        args.insert(args.begin(), GetStack().back());
        GetStack().pop_back();
    }
    RnObject* ret_val = _memory_manager->CreateObject(obj->GetReturnType());

    if (func->IsBuiltIn()) {
        func->Call(args, ret_val);
        if (obj->GetReturnType() != RnType::RN_NULL) {
            GetStack().push_back(ret_val);
        }
    } else {
        _call_stack.push_back(func->GetScope());
        _scopes.push_back(func->GetScope());

        func->PassArguments(args);
        bool has_returned = false;
        size_t func_index = func->GetIStart();
        size_t end_index = func->GetIStart() + func->GetICnt();
        for (; func_index < end_index; func_index++) {
            ExecuteInstruction(has_returned, func_index);
            if (has_returned) {
                break;
            }
        }

        _call_stack.pop_back();
        _scopes.pop_back();

        if (func->GetName() == "construct") {
            GetStack().push_back(func->GetScope()->GetObject(_object_this_key));
        }
        func->Reset();
    }
}

/*****************************************************************************/
void RnVirtualMachine::AddScope() {
    auto scope = _memory_manager->CreateScope();
    if (!_scopes.empty()) {
        scope->SetParent(GetScope());
    }
    _scopes.push_back(scope);
}

/*****************************************************************************/
void RnVirtualMachine::ExecuteInstruction(bool& break_scope, size_t& index) {
    _gc_count++;
    if (_gc_count > 1000) {
        _memory_manager->GCMark();
        _memory_manager->GCSweep();
        _gc_count = 0;
    }

    auto instruction = _instructions[index];
    switch (instruction->_opcode) {
        case OP_BINARY_ADD: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = *a + b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_SUB: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = *a - b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_MUL: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = *a * b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_DIV: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = *a / b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_MOD: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = *a % b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_GTE: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = *a >= b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_LTE: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = *a <= b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_GT: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = *a > b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_LT: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = *a < b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_EQ: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = *a == b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_NEQ: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = *a != b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_POWER: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = CreateObject(std::pow(a->ToFloat(), b->ToFloat()));
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_RSH: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = *a << b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_LSH: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = *a >> b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_OR: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = *a | b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_XOR: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = *a ^ b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_AND: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = *a & b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_LOGICAL_OR: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = *a || b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_LOGICAL_AND: {
            auto b = GetStack().back();
            GetStack().pop_back();
            auto a = GetStack().back();
            GetStack().pop_back();
            auto result = *a && b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_STORE: {
            auto obj = GetStack().back();
            GetStack().pop_back();
            auto value = GetStack().back();
            GetStack().pop_back();
            obj->CopyDataFromObject(value);
            break;
        }
        case OP_POP: {
            GetStack().pop_back();
            break;
        }
        case OP_UNARY_NOT: {
            auto obj = GetScope()->GetObject(instruction->_arg1);
            obj->SetData(!obj->ToBool());
            break;
        }
        case OP_BREAK: {
            break;
        }
        case OP_CONTINUE: {
            break;
        }
        case OP_TRY_CONTEXT: {
            break;
        }
        case OP_UNARY_INVERT: {
            break;
        }
        case OP_UNARY_DECREMENT: {
            auto obj = GetScope()->GetObject(instruction->_arg1);
            obj->SetData(obj->ToFloat() - 1);
            break;
        }
        case OP_UNARY_INCREMENT: {
            auto obj = GetScope()->GetObject(instruction->_arg1);
            obj->SetData(obj->ToFloat() + 1);
            break;
        }
        case OP_UNARY_NEGATION: {
            break;
        }
        case OP_RETURN: {
            auto function_scope = _call_stack.back();
            if (!function_scope) {
                throw std::runtime_error("Cannot return outside of a function");
            }
            // Have to get the parent of the working scope, not the argument scope
            auto ret_scope = _scopes[_scopes.size() - 2];
            ret_scope->GetStack().push_back(GetStack().back());
            GetStack().pop_back();
            break_scope = true;
            break;
        }
        case OP_LOAD_INT: {
            auto value = RnObject::GetInternedInt(instruction->_arg1);
            auto obj = CreateObject(static_cast<RnIntNative>(value));
            GetScope()->GetMemoryGroup()->AddObject(obj);
            GetStack().push_back(obj);
            break;
        }
        case OP_LOAD_FLOAT: {
            auto value = RnObject::GetInternedFloat(instruction->_arg1);
            auto obj = CreateObject(value);
            GetScope()->GetMemoryGroup()->AddObject(obj);
            GetStack().push_back(obj);
            break;
        }
        case OP_LOAD_STRING: {
            auto value = RnObject::GetInternedString(instruction->_arg1);
            auto obj = CreateObject(value);
            GetScope()->GetMemoryGroup()->AddObject(obj);
            GetStack().push_back(obj);
            break;
        }
        case OP_LOAD_VALUE: {
            auto key = instruction->_arg1;
            auto object = GetScope()->GetObject(key);
            if (object) {
                GetStack().push_back(object);
            } else if (_namespaces.contains(key)) {
                auto class_obj = dynamic_cast<RnClassObject*>(_namespaces[key]);
                if (class_obj->IsModule()) {
                    throw std::runtime_error("Cannot instantiate module '" +
                                             class_obj->GetName() + "'");
                }

                auto instance = dynamic_cast<RnClassObject*>(
                    _memory_manager->CreateObject(RnType::RN_CLASS_INSTANCE));
                GetScope()->GetMemoryGroup()->AddObject(instance);
                instance->ToObject()->SetParent(class_obj->ToObject());
                class_obj->CopySymbols(instance->GetScope());
                instance->GetScope()->StoreObject(_object_this_key, instance);
                auto func_obj = dynamic_cast<RnFunctionObject*>(
                    class_obj->ToObject()->GetObject(_object_construct_key));
                auto func = func_obj->ToFunction();

                func->SetScope(new RnScope(instance->GetScope()));
                GetStack().push_back(func_obj);
            } else {
                throw std::runtime_error("Symbol does not exist: " +
                                         RnObject::GetInternedString(key));
            }
            break;
        }
        case OP_LOAD_NULL: {
            GetStack().push_back(_memory_manager->CreateObject(RnType::RN_NULL));
            break;
        }
        case OP_LOAD_BOOL: {
            auto value = static_cast<bool>(instruction->_arg1);
            auto obj = CreateObject(value);
            GetScope()->GetMemoryGroup()->AddObject(obj);
            GetStack().push_back(obj);
            break;
        }
        case OP_CALL: {
            auto func_obj = dynamic_cast<RnFunctionObject*>(GetStack().back());
            GetStack().pop_back();
//            CallFunction(func_obj, instruction->_arg1);

                std::vector<RnObject*> args;
                auto func = func_obj->GetData();
                args.reserve(instruction->_arg1);
                for (uint32_t i = 0; i < instruction->_arg1; i++) {
                    args.insert(args.begin(), GetStack().back());
                    GetStack().pop_back();
                }
                RnObject* ret_val = _memory_manager->CreateObject(func_obj->GetReturnType());

                if (func->IsBuiltIn()) {
                    func->Call(args, ret_val);
                    if (func_obj->GetReturnType() != RnType::RN_NULL) {
                        GetStack().push_back(ret_val);
                    }
                } else {
                    _call_stack.push_back(func->GetScope());
                    _scopes.push_back(func->GetScope());

                    func->PassArguments(args);
                    bool has_returned = false;
                    size_t func_index = func->GetIStart();
                    size_t end_index = func->GetIStart() + func->GetICnt();
                    for (; func_index < end_index; func_index++) {
                        ExecuteInstruction(has_returned, func_index);
                        if (has_returned) {
                            break;
                        }
                    }

                    _call_stack.pop_back();
                    _scopes.pop_back();

                    if (func->GetName() == "construct") {
                        GetStack().push_back(func->GetScope()->GetObject(_object_this_key));
                    }
                    func->Reset();
                }
            break;
        }
        case OP_MAKE_CONST: {
            auto type = static_cast<RnType::Type>(instruction->_arg1);
            auto obj = _memory_manager->CreateObject(type);
            obj->SetConstFlag(true);
            GetScope()->GetMemoryGroup()->AddObject(obj);
            GetScope()->StoreObject(instruction->_arg2, obj);
            break;
        }
        case OP_MAKE_VAR: {
            auto type = static_cast<RnType::Type>(instruction->_arg1);
            auto obj = _memory_manager->CreateObject(type);
            GetScope()->GetMemoryGroup()->AddObject(obj);
            GetScope()->StoreObject(instruction->_arg2, obj);
            break;
        }
        case OP_MAKE_MODULE: {
            auto name = RnObject::GetInternedString(instruction->_arg1);
            auto obj = dynamic_cast<RnClassObject*>(
                RnObject::Create(RnType::RN_CLASS_INSTANCE));
            obj->SetIsModule(true);
            _namespaces[instruction->_arg1] = obj;
            _scopes.push_back(obj->ToObject());
            index++;
            size_t stop_index = index + instruction->_arg2;
            for (; index < stop_index; index++) {
                ExecuteInstruction(break_scope, index);
            }
            index--;
            _scopes.pop_back();
            break;
        }
        case OP_MAKE_CLASS: {
            auto name = RnObject::GetInternedString(instruction->_arg1);
            auto obj = dynamic_cast<RnClassObject*>(
                RnObject::Create(RnType::RN_CLASS_INSTANCE));
            _namespaces[instruction->_arg1] = obj;
            auto class_scope = obj->ToObject();
            class_scope->SetParent(GetScope());
            _scopes.push_back(class_scope);
            index++;
            size_t stop_index = index + instruction->_arg2;
            for (; index < stop_index; index++) {
                ExecuteInstruction(break_scope, index);
            }
            index--;
            _scopes.pop_back();
            break;
        }
        case OP_MAKE_FUNC: {
            auto obj = dynamic_cast<RnFunctionObject*>(
                _memory_manager->CreateObject(RnType::RN_FUNCTION));
            GetScope()->GetMemoryGroup()->AddObject(obj);
            auto name = RnObject::GetInternedString(instruction->_arg1);
            auto func = new RnFunction(name, index + 1 + instruction->_arg2,
                                       instruction->_arg3);
            obj->SetData(func);
            obj->SetReturnType(static_cast<RnType::Type>(instruction->_arg2));
            func->SetScope(new RnScope(GetScope()));

            for (uint32_t i = 0; _instructions[i + index + 1]->_opcode == OP_MAKE_ARG;
                 i++) {
                auto arg_instruction = _instructions[i + index + 1];
                func->CreateArgument(arg_instruction->_arg2,
                                     static_cast<RnType::Type>(arg_instruction->_arg1),
                                     i);
            }

            index += instruction->_arg2 + func->GetICnt();
            GetScope()->StoreObject(instruction->_arg1, obj);
            break;
        }
        case OP_CREATE_CONTEXT: {
            AddScope();
            break;
        }
        case OP_DESTROY_CONTEXT: {
            auto scope = _scopes.back();
            _scopes.pop_back();
            _memory_manager->DestroyScope(scope);
            break;
        }
        case OP_RESET_CONTEXT: {
            GetScope()->GetSymbolTable()->Clear();
            GetStack().clear();
            break;
        }
        case OP_DELETE: {
            GetScope()->GetSymbolTable()->RemoveSymbol(instruction->_arg1);
            break;
        }
        case OP_NOP: {
            break;
        }
        case OP_JUMPF: {
            index += instruction->_arg1;
            break;
        }
        case OP_JUMPB: {
            index -= instruction->_arg1 + 1;
            break;
        }
        case OP_JUMPF_IF: {
            auto steps = instruction->_arg1;
            auto condition = GetStack().back()->ToInt();
            GetStack().pop_back();
            if (!condition) {
                index += steps;
            }
            break;
        }
        case OP_JUMPB_IF: {
            auto steps = instruction->_arg1;
            auto condition = GetStack().back()->ToInt();
            GetStack().pop_back();
            if (!condition) {
                index -= steps;
            }
            break;
        }
        case OP_COMPARE: {
            break;
        }
        case OP_EXIT: {
            break_scope = true;
            auto obj = _memory_manager->CreateObject(RnType::RN_INT);
            GetScope()->GetMemoryGroup()->AddObject(obj);
            obj->SetData(static_cast<RnIntNative>(instruction->_arg1));
            GetStack().push_back(obj);
            break;
        }
        case OP_ARRAY_STORE: {
            // TODO: Remove OP_ARRAY_STORE
            break;
        }
        case OP_INDEX_ACCESS: {
            auto idx_value = GetStack().back()->ToInt();
            GetStack().pop_back();
            auto obj = dynamic_cast<RnArrayObject*>(GetStack().back());
            GetStack().pop_back();

            // try/catch is used here to handle out of bounds access for performance
            // reasons. Checking the bounds for each access would be unnecessarily
            // costly.
            try {
                auto result = obj->ToArray().at(idx_value);
                GetStack().push_back(result);
            } catch (const std::exception& e) {
                if (idx_value >= obj->ToArray().size() || idx_value < 0) {
                    throw std::runtime_error("Invalid index [" +
                                             std::to_string(idx_value) + "]");
                } else {
                    throw e;
                }
            }
            break;
        }
        case OP_MAKE_ARG: {
            // Do nothing. These instructions are consumed automatically
            // by the MAKE_FUNC handler
            break;
        }
        case OP_MAKE_ALIAS: {
            GetScope()->GetSymbolTable()->AliasSymbol(instruction->_arg1,
                                                      instruction->_arg2);
            break;
        }
        case OP_MAKE_ARRAY: {
            auto obj = dynamic_cast<RnArrayObject*>(CreateObject(RnType::RN_ARRAY));
            GetScope()->GetMemoryGroup()->AddObject(obj);

            for (RnIntNative i = 0; i < instruction->_arg1; i++) {
                auto copy = RnObject::Copy(GetStack().back());
                GetScope()->GetMemoryGroup()->AddObject(copy);
                obj->Append(copy);
                GetStack().pop_back();
            }
            GetStack().push_back(obj);
            break;
        }
        case OP_ATTR_ACCESS: {
            auto object = dynamic_cast<RnClassObject*>(GetStack().back());
            GetStack().pop_back();
            auto scope = object->GetScope();
            RnObject* result = nullptr;
            if (scope->GetSymbolTable()->SymbolExists(instruction->_arg1, false)) {
                result = scope->GetObject(instruction->_arg1);
                GetStack().push_back(result);
            } else if (scope->GetParent()->GetSymbolTable()->SymbolExists(
                           instruction->_arg1, false)) {
                result = scope->GetParent()->GetObject(instruction->_arg1);
                GetStack().push_back(result);
            } else {
                throw std::runtime_error(
                    "Object has no attribute '" +
                    RnObject::GetInternedString(instruction->_arg1) + "'");
            }

            // Only want to set "this" binding if the function is immediately called.
            // Otherwise, the user will need to bind the function
            if (result->GetType() == RnType::RN_FUNCTION) {
                auto func = dynamic_cast<RnFunctionObject*>(result)->GetData();
                if (_instructions[index + 1]->_opcode == OP_CALL) {
                    func->GetScope()->GetSymbolTable()->SetSymbol(_object_this_key,
                                                                  object);
                } else {
                    auto symbol_table = func->GetScope()->GetSymbolTable();
                    if (symbol_table->SymbolExists(_object_this_key)) {
                        symbol_table->RemoveSymbol(_object_this_key);
                    }
                }
            }
            break;
        }
        case OP_RESOLVE_NAMESPACE:
            // TODO: Fix this so that it pushes the correct object onto the stack
            auto nspace = _namespaces[instruction->_arg1];
            GetStack().push_back(nspace->ToObject()->GetObject(instruction->_arg2));
            break;
    }
}

/*****************************************************************************/
RnIntNative RnVirtualMachine::Run() {
    bool has_returned = false;  // Placeholder
    auto stopwatch = StopWatch();

    stopwatch.Start();
    while (i_idx < _instructions.size()) {
        ExecuteInstruction(has_returned, i_idx);
        if (has_returned) {
            break;
        }
        i_idx++;
    }
    stopwatch.Stop();

    Log::DEBUG("\nRuntime duration: " + std::to_string(stopwatch.Duration()));
    return GetStack().back()->ToInt();
}

/*****************************************************************************/
void RnVirtualMachine::LoadInstructions(std::vector<RnInstruction*> instructions) {
    _instructions = std::move(instructions);
}

/*****************************************************************************/
RnVirtualMachine* RnVirtualMachine::GetInstance() {
    if (!_instance) {
        _instance = new RnVirtualMachine();
        _instance->Init();
    }
    return _instance;
}

/*****************************************************************************/
RnObject* RnVirtualMachine::CreateObject(RnType::Type type) {
    return _memory_manager->CreateObject(type);
}

/*****************************************************************************/
RnObject* RnVirtualMachine::CreateObject(RnStringNative data) {
    return _memory_manager->Create(std::move(data));
}

/*****************************************************************************/
RnObject* RnVirtualMachine::CreateObject(RnBoolNative data) {
    return _memory_manager->Create(data);
}

/*****************************************************************************/
RnObject* RnVirtualMachine::CreateObject(RnIntNative data) {
    return _memory_manager->Create(data);
}

/*****************************************************************************/
RnObject* RnVirtualMachine::CreateObject(RnFloatNative data) {
    return _memory_manager->Create(data);
}

/*****************************************************************************/
RnScope* RnVirtualMachine::CreateScope() {
    return _memory_manager->CreateScope();
}

/*****************************************************************************/
void RnVirtualMachine::RegisterBuiltins() {
    // Would be nice to have each of these in separate namespaces based on their
    // category
    std::vector<std::tuple<std::string, BuiltinFunction, RnType::Type>> functions = {
        {"print", CastToBuiltin(&RnBuiltins_IO::rn_builtin_print), RnType::RN_NULL},
        {"read", CastToBuiltin(&RnBuiltins_IO::read), RnType::RN_STRING},
        {"file_read", CastToBuiltin(&RnBuiltins_IO::read), RnType::RN_STRING},
        {"file_write", CastToBuiltin(&RnBuiltins_IO::file_write), RnType::RN_BOOLEAN},
        {"prompt", CastToBuiltin(&RnBuiltins_IO::rn_builtin_prompt), RnType::RN_STRING},
        {"file_size", CastToBuiltin(&RnBuiltins_IO::file_size), RnType::RN_INT},
        {"sum", CastToBuiltin(&RnBuiltins_Math::rn_builtin_sum), RnType::RN_FLOAT},
        {"pow", CastToBuiltin(&RnBuiltins_Math::pow), RnType::RN_FLOAT},
        {"is_int", CastToBuiltin(&RnBuiltins_Type::rn_builtin_is_int),
         RnType::RN_BOOLEAN},
        {"is_float", CastToBuiltin(&RnBuiltins_Type::rn_builtin_is_float),
         RnType::RN_BOOLEAN},
        {"is_array", CastToBuiltin(&RnBuiltins_Type::rn_builtin_is_array),
         RnType::RN_BOOLEAN},
        {"is_object", CastToBuiltin(&RnBuiltins_Type::rn_builtin_is_object),
         RnType::RN_BOOLEAN},
        {"is_bool", CastToBuiltin(&RnBuiltins_Type::rn_builtin_is_bool),
         RnType::RN_BOOLEAN},
        {"is_callable", CastToBuiltin(&RnBuiltins_Type::rn_builtin_is_callable),
         RnType::RN_BOOLEAN},
        {"str_merge", CastToBuiltin(&RnBuiltins_String::rn_builtin_str_merge),
         RnType::RN_STRING},
        {"array_merge", CastToBuiltin(&RnBuiltins_Array::rn_builtin_array_merge),
         RnType::RN_ARRAY},
        {"count", CastToBuiltin(&RnBuiltins_Array::rn_builtin_count), RnType::RN_INT},
        {"system", CastToBuiltin(&RnBuiltins::rn_builtin_system), RnType::RN_ARRAY},
        {"lload", CastToBuiltin(&RnBuiltins::rn_builtin_lload),
         RnType::RN_CLASS_INSTANCE},
        {"bind", CastToBuiltin(&RnBuiltins::rn_builtin_bind), RnType::RN_FUNCTION},
        {"count", CastToBuiltin(&RnBuiltins_Array::rn_builtin_count), RnType::RN_INT},
        {"array_push", CastToBuiltin(&RnBuiltins_Array::rn_builtin_array_push),
         RnType::RN_NULL},
        {"setenv", CastToBuiltin(&RnBuiltins::rn_builtin_setenv),
         RnType::RN_INT},
        {"getenv", CastToBuiltin(&RnBuiltins::rn_builtin_getenv),
         RnType::RN_STRING},
        {"unsetenv", CastToBuiltin(&RnBuiltins::rn_builtin_unsetenv),
         RnType::RN_INT},
    };

    for (auto parts : functions) {
        auto func = new RnBuiltinFunction(std::get<0>(parts), std::get<1>(parts));
        func->SetScope(GetScope());
        auto obj = dynamic_cast<RnFunctionObject*>(
            _memory_manager->CreateObject(RnType::RN_FUNCTION));
        GetScope()->GetMemoryGroup()->AddObject(obj);
        obj->SetReturnType(std::get<2>(parts));
        obj->SetData(func);
        GetScope()->StoreObject(RnObject::InternValue(std::get<0>(parts)), obj);
    }
}
