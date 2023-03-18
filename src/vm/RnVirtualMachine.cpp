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
RnIntNative RnVirtualMachine::_object_cls_key = 0;
RnIntNative RnVirtualMachine::_object_construct_key = 0;

/*****************************************************************************/
RnVirtualMachine::RnVirtualMachine() {
    _scopes.reserve(16);
    _call_stack.reserve(50);
    _memory_manager = new RnMemoryManager();

    _object_this_key = RnObject::InternValue("this");
    _object_cls_key = RnObject::InternValue("cls");
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
void RnVirtualMachine::BindCls(RnScope* scope, RnObject* binding) {
    scope->StoreObject(_object_cls_key, binding);
}

/*****************************************************************************/
void RnVirtualMachine::BindThis(RnScope* scope, RnObject* binding) {
    scope->StoreObject(_object_this_key, binding);
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
        args.insert(args.begin(), StackPop());
    }
    RnObject* ret_val = _memory_manager->CreateObject(obj->GetReturnType());

    if (func->IsBuiltIn()) {
        func->Call(args, ret_val);
        if (obj->GetReturnType() != RnType::RN_VOID) {
            GetStack().push_back(ret_val);
        }
    } else {
        auto scope = CreateScope();
        scope->SetParent(GetScope());
        func->InitScope(scope);
        _call_stack.push_back(scope);
        _scopes.push_back(scope);

        func->PassArguments(args, scope);
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
    }
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
    Log::DEBUG(instruction->ToString());
    switch (instruction->GetOpcode()) {
        case OP_BINARY_ADD: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = *a + b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_SUB: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = *a - b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_MUL: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = *a * b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_DIV: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = *a / b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_MOD: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = *a % b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_GTE: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = *a >= b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_LTE: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = *a <= b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_GT: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = *a > b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_LT: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = *a < b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_EQ: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = *a == b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_NEQ: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = *a != b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_POWER: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = CreateObject(std::pow(a->ToFloat(), b->ToFloat()));
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_RSH: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = *a << b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_LSH: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = *a >> b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_OR: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = *a | b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_XOR: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = *a ^ b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_BINARY_AND: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = *a & b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_LOGICAL_OR: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = *a || b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_LOGICAL_AND: {
            auto b = StackPop();
            auto a = StackPop();
            auto result = *a && b;
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_STORE: {
            auto obj = StackPop();
            auto value = StackPop();
            Log::DEBUG("Storing (" + RnType::TypeToString(obj->GetType()) + " <- " +
                       RnType::TypeToString(value->GetType()) + ")");
            obj->CopyDataFromObject(value);
            break;
        }
        case OP_POP: {
            StackPop();
            break;
        }
        case OP_UNARY_NOT: {
            auto obj = StackPop();
            auto result = RnObject::Create(!obj->ToBool());
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
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
            auto obj = StackPop();
            auto result = RnObject::Create(~obj->ToInt());
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);
            break;
        }
        case OP_UNARY_DECREMENT: {
            auto obj = GetScope()->GetObject(instruction->GetArg1());
            obj->SetData(obj->ToFloat() - 1);
            break;
        }
        case OP_UNARY_INCREMENT: {
            auto obj = GetScope()->GetObject(instruction->GetArg1());
            obj->SetData(obj->ToFloat() + 1);
            break;
        }
        case OP_UNARY_NEGATION: {
            auto obj = StackPop();
            auto result = RnObject::Create(-obj->ToFloat());
            GetScope()->GetMemoryGroup()->AddObject(result);
            GetStack().push_back(result);

            break;
        }
        case OP_RETURN: {
            auto function_scope = _call_stack.back();
            if (!function_scope) {
                throw std::runtime_error("Cannot return outside of a function");
            }
            // Have to get the parent of the working scope, not the argument scope
            auto ret_scope = _scopes[_scopes.size() - 2];
            function_scope->GetStack().push_back(GetStack().back());
            GetStack().pop_back();
            break_scope = true;
            break;
        }
        case OP_LOAD_INT: {
            auto value = RnObject::GetInternedInt(instruction->GetArg1());
            auto obj = _memory_manager->CreateObject(RnType::RN_INT);
            GetScope()->GetMemoryGroup()->AddObject(obj);
            obj->SetData(static_cast<RnIntNative>(value));
            GetStack().push_back(obj);
            break;
        }
        case OP_LOAD_FLOAT: {
            auto value = RnObject::GetInternedFloat(instruction->GetArg1());
            auto obj = _memory_manager->CreateObject(RnType::RN_FLOAT);
            GetScope()->GetMemoryGroup()->AddObject(obj);
            obj->SetData(static_cast<RnFloatNative>(value));
            GetStack().push_back(obj);
            break;
        }
        case OP_LOAD_STRING: {
            auto value = RnObject::GetInternedString(instruction->GetArg1());
            auto obj = _memory_manager->CreateObject(RnType::RN_STRING);
            GetScope()->GetMemoryGroup()->AddObject(obj);
            obj->SetData(value);
            GetStack().push_back(obj);
            break;
        }
        case OP_LOAD_VALUE: {
            auto key = instruction->GetArg1();
            auto object = GetScope()->GetObject(key);

            if (object) {
                Log::DEBUG("Loading (" + RnObject::GetInternedString(key) + ", " + RnType::TypeToString(object->GetType()) + ")");
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
                BindThis(instance->GetScope(), instance);
                BindCls(instance->GetScope(), class_obj);
                auto func_obj = dynamic_cast<RnFunctionObject*>(
                    class_obj->ToObject()->GetObject(_object_construct_key));
                auto func = func_obj->ToFunction();
                auto func_scope = RnObject::Create(RnType::RN_OBJECT)->ToObject();
                func_scope->SetParent(instance->GetScope());
                BindThis(func_scope, instance);
                func->SetScope(func_scope);
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
            auto value = static_cast<bool>(instruction->GetArg1());
            auto obj = _memory_manager->CreateObject(RnType::RN_BOOLEAN);
            GetScope()->GetMemoryGroup()->AddObject(obj);
            obj->SetData(value);
            GetStack().push_back(obj);
            break;
        }
        case OP_CALL: {
            auto stack_val = StackPop();
            RnFunctionObject* func_obj = nullptr;
            if (stack_val->GetType() == RnType::RN_OBJECT) {
                auto class_obj = dynamic_cast<RnClassObject*>(stack_val);
                if (class_obj->IsModule()) {
                    throw std::runtime_error("Cannot instantiate module '" +
                                             class_obj->GetName() + "'");
                }

                auto instance = dynamic_cast<RnClassObject*>(
                    _memory_manager->CreateObject(RnType::RN_CLASS_INSTANCE));
                GetScope()->GetMemoryGroup()->AddObject(instance);
                instance->ToObject()->SetParent(class_obj->ToObject());
                class_obj->CopySymbols(instance->GetScope());
                BindThis(instance->GetScope(), instance);
                BindCls(instance->GetScope(), class_obj);
                auto constructor_obj = dynamic_cast<RnFunctionObject*>(
                    class_obj->ToObject()->GetObject(_object_construct_key));
                auto func = constructor_obj->ToFunction();
                auto func_scope = RnObject::Create(RnType::RN_OBJECT)->ToObject();
                func_scope->SetParent(instance->GetScope());
                BindThis(func_scope, instance);
                func->SetScope(func_scope);
                GetStack().push_back(constructor_obj);
                func_obj = constructor_obj;
            } else {
                func_obj = dynamic_cast<RnFunctionObject*>(stack_val);
            }
            std::vector<RnObject*> args;
            auto func = func_obj->GetData();
            args.reserve(instruction->GetArg1());
            for (uint32_t i = 0; i < instruction->GetArg1(); i++) {
                args.insert(args.begin(), StackPop());
            }

            if (func->IsBuiltIn()) {
                RnObject* ret_val =
                    _memory_manager->CreateObject(func_obj->GetReturnType());
                GetScope()->GetMemoryGroup()->AddObject(ret_val);
                func->Call(args, ret_val);
                GetStack().push_back(ret_val);
            } else {
                auto scope = CreateScope();
                scope->SetParent(func->GetScope());
                func->InitScope(scope);
                _scopes.push_back(scope);
                _call_stack.push_back(scope);
                func->PassArguments(args, scope);

                bool has_returned = false;
                size_t func_index = func->GetIStart();
                size_t end_index = func->GetIStart() + func->GetICnt();
                for (; func_index <= end_index; func_index++) {
                    ExecuteInstruction(has_returned, func_index);
                    if (has_returned) {
                        break;
                    }
                }

                _call_stack.pop_back();
                _scopes.pop_back();

                for (int i = 0; i < scope->GetLinkedScopeCount(); i++) {
                    _scopes.pop_back();
                }

                if (has_returned) {
                    GetStack().push_back(scope->GetStack().back());
                } else {
                   GetStack().push_back(RnObject::Create(RnType::RN_NULL));
                }

                if (func->GetName() == "construct") {
                    GetStack().push_back(func->GetScope()->GetObject(_object_this_key));
                }
            }
            break;
        }
        case OP_MAKE_CONST: {
            auto type = static_cast<RnType::Type>(instruction->GetArg1());
            auto obj = _memory_manager->CreateObject(type);
            obj->SetConstFlag(true);
            GetScope()->GetMemoryGroup()->AddObject(obj);
            GetScope()->StoreObject(instruction->GetArg2(), obj);
            break;
        }
        case OP_MAKE_LOCAL: {
            auto type = static_cast<RnType::Type>(instruction->GetArg1());
            auto obj = GetScope()->MakeLocal(type);
            GetScope()->StoreObject(instruction->GetArg2(), obj);
            break;
        }
        case OP_MAKE_VAR: {
            auto type = static_cast<RnType::Type>(instruction->GetArg1());
            auto obj = _memory_manager->CreateObject(type);
            GetScope()->GetMemoryGroup()->AddObject(obj);
            GetScope()->StoreObject(instruction->GetArg2(), obj);
            break;
        }
        case OP_MAKE_GLOBAL: {
            auto type = static_cast<RnType::Type>(instruction->GetArg1());
            auto obj = _memory_manager->CreateObject(type);
            _scopes.front()->GetMemoryGroup()->AddObject(obj);
            _scopes.front()->StoreObject(instruction->GetArg2(), obj);
            break;
        }
        case OP_MAKE_MODULE: {
            auto name = RnObject::GetInternedString(instruction->GetArg1());
            auto obj = dynamic_cast<RnClassObject*>(
                RnObject::Create(RnType::RN_CLASS_INSTANCE));
            obj->SetIsModule(true);
            GetScope()->StoreObject(instruction->GetArg1(), obj);
            //            _namespaces[instruction->GetArg1()] = obj;
            _scopes.push_back(obj->ToObject());
            index++;
            size_t stop_index = index + instruction->GetArg2();
            for (; index < stop_index; index++) {
                ExecuteInstruction(break_scope, index);
            }
            index--;
            _scopes.pop_back();
            break;
        }
        case OP_MAKE_CLASS: {
            auto name = RnObject::GetInternedString(instruction->GetArg1());
            auto obj = dynamic_cast<RnClassObject*>(
                RnObject::Create(RnType::RN_CLASS_INSTANCE));
            auto name_obj = RnObject::Create(name);
            name_obj->SetConstFlag(true);
            obj->GetScope()->StoreObject(RnObject::InternValue("__class"), name_obj);
            _namespaces[instruction->GetArg1()] = obj;
            auto class_scope = obj->ToObject();
            class_scope->SetParent(GetScope());
            _scopes.push_back(class_scope);
            index++;
            size_t stop_index = index + instruction->GetArg2();
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
            auto name = RnObject::GetInternedString(instruction->GetArg1());
            auto type = static_cast<RnType::Type>(instruction->GetArg2());
            auto scope_size = instruction->GetArg3();
            auto func = new RnFunction(name, index + 1, scope_size);
            obj->SetData(func);
            obj->SetReturnType(type);
            func->SetScope(new RnScope(GetScope()));

            uint32_t i = 0;  // Argument count
            for (; _instructions[i + index + 1]->GetOpcode() == OP_MAKE_ARG; i++) {
                auto arg_instruction = _instructions[i + index + 1];
                func->CreateArgument(arg_instruction->GetArg2(),
                                     static_cast<RnType::Type>(arg_instruction->GetArg1()),
                                     i);
            }

            index += scope_size + i;
            GetScope()->StoreObject(instruction->GetArg1(), obj);
            break;
        }
        case OP_CREATE_CONTEXT: {
            auto scope = _memory_manager->CreateScope();
            if (!_scopes.empty()) {
                scope->SetParent(GetScope());
            }
            _scopes.push_back(scope);

            if (!_call_stack.empty()) {
                _call_stack.back()->IncrLinkedScopeCount();
            }
            break;
        }
        case OP_DESTROY_CONTEXT: {
            auto scope = _scopes.back();
            _scopes.pop_back();
            _memory_manager->DestroyScope(scope);

            if (!_call_stack.empty()) {
                _call_stack.back()->DecrLinkedScopeCount();
            }
            break;
        }
        case OP_RESET_CONTEXT: {
            GetScope()->Reset();
            break;
        }
        case OP_DELETE: {
            GetScope()->GetSymbolTable()->RemoveSymbol(instruction->GetArg1());
            break;
        }
        case OP_NOP: {
            break;
        }
        case OP_JUMPF: {
            index += instruction->GetArg1();
            break;
        }
        case OP_JUMPB: {
            index -= instruction->GetArg1() + 1;
            break;
        }
        case OP_JUMPF_IF: {
            auto steps = instruction->GetArg1();
            auto condition = StackPop()->ToInt();
            if (!condition) {
                index += steps;
            }
            break;
        }
        case OP_JUMPB_IF: {
            auto steps = instruction->GetArg1();
            auto condition = StackPop()->ToInt();
            if (!condition) {
                index -= steps;
            }
            break;
        }
        case OP_EXIT: {
            break_scope = true;
            auto obj = _memory_manager->CreateObject(RnType::RN_INT);
            GetScope()->GetMemoryGroup()->AddObject(obj);
            obj->SetData(static_cast<RnIntNative>(instruction->GetArg1()));
            GetStack().push_back(obj);
            break;
        }
        case OP_INDEX_ACCESS: {
            auto idx_value = StackPop()->ToInt();
            auto uncast = StackPop();
            auto obj = dynamic_cast<RnArrayObject*>(uncast);

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
            GetScope()->GetSymbolTable()->AliasSymbol(instruction->GetArg1(),
                                                      instruction->GetArg2());
            break;
        }
        case OP_MAKE_ARRAY: {
            auto obj = dynamic_cast<RnArrayObject*>(CreateObject(RnType::RN_ARRAY));
            GetScope()->GetMemoryGroup()->AddObject(obj);

            for (RnIntNative i = 0; i < instruction->GetArg1(); i++) {
                auto copy = RnObject::Copy(StackPop());
                GetScope()->GetMemoryGroup()->AddObject(copy);
                obj->Append(copy);
            }
            GetStack().push_back(obj);
            break;
        }
        case OP_ATTR_ACCESS: {
            auto object = dynamic_cast<RnClassObject*>(StackPop());
            auto scope = object->GetScope();
            RnObject* result = nullptr;
            if (scope->GetSymbolTable()->SymbolExists(instruction->GetArg1(), false)) {
                result = scope->GetObject(instruction->GetArg1());
                GetStack().push_back(result);
            } else if (scope->GetParent() &&
                       scope->GetParent()->GetSymbolTable()->SymbolExists(
                           instruction->GetArg1(), false)) {
                result = scope->GetParent()->GetObject(instruction->GetArg1());
                GetStack().push_back(result);
            } else {
                throw std::runtime_error(
                    "Object has no attribute '" +
                    RnObject::GetInternedString(instruction->GetArg1()) + "'");
            }

            // Only want to set "this" binding if the function is immediately called.
            // Otherwise, the user will need to bind the function
            if (result->GetType() == RnType::RN_FUNCTION) {
                auto func = dynamic_cast<RnFunctionObject*>(result)->GetData();
                BindThis(func->GetScope(), object);
            }
            break;
        }
        case OP_RESOLVE_NAMESPACE: {
            auto nspace = _namespaces[instruction->GetArg1()];
            auto object = nspace->ToObject()->GetObject(instruction->GetArg2());
            if (object->GetType() == RnType::RN_FUNCTION) {
                BindCls(object->ToFunction()->GetScope(), nspace);
                BindThis(object->ToFunction()->GetScope(), nspace);
            }
            GetStack().push_back(object);
            break;
        }
    }
}

/*****************************************************************************/
RnIntNative RnVirtualMachine::Run() {
    std::setvbuf(stdout, nullptr, _IOLBF, 65536);
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
        {"sum", CastToBuiltin(&RnBuiltins_Math::rn_builtin_sum), RnType::RN_FLOAT},
        {"pow", CastToBuiltin(&RnBuiltins_Math::rn_builtin_pow), RnType::RN_FLOAT},
        {"mod", CastToBuiltin(&RnBuiltins_Math::rn_builtin_mod), RnType::RN_INT},
        {"sqrt", CastToBuiltin(&RnBuiltins_Math::rn_builtin_sqrt), RnType::RN_FLOAT},
        {"cbrt", CastToBuiltin(&RnBuiltins_Math::rn_builtin_cbrt), RnType::RN_FLOAT},
        {"randf", CastToBuiltin(&RnBuiltins_Math::rn_builtin_randf), RnType::RN_FLOAT},
        {"randint", CastToBuiltin(&RnBuiltins_Math::rn_builtin_randint),
         RnType::RN_INT},
        {"normal", CastToBuiltin(&RnBuiltins_Math::rn_builtin_normal),
         RnType::RN_ARRAY},
        {"file_size", CastToBuiltin(&RnBuiltins_IO::rn_builtin_file_size),
         RnType::RN_INT},
        {"file_write", CastToBuiltin(&RnBuiltins_IO::rn_builtin_file_write),
         RnType::RN_INT},
        {"print", CastToBuiltin(&RnBuiltins_IO::rn_builtin_print), RnType::RN_VOID},
        {"read", CastToBuiltin(&RnBuiltins_IO::rn_builtin_read), RnType::RN_STRING},
        {"prompt", CastToBuiltin(&RnBuiltins_IO::rn_builtin_prompt), RnType::RN_STRING},
        {"to_int", CastToBuiltin(&RnBuiltins_Type::rn_builtin_to_int), RnType::RN_INT},
        {"to_float", CastToBuiltin(&RnBuiltins_Type::rn_builtin_to_float),
         RnType::RN_FLOAT},
        {"to_string", CastToBuiltin(&RnBuiltins_Type::rn_builtin_to_string),
         RnType::RN_STRING},
        {"to_bool", CastToBuiltin(&RnBuiltins_Type::rn_builtin_to_bool),
         RnType::RN_BOOLEAN},
        {"is_string", CastToBuiltin(&RnBuiltins_Type::rn_builtin_is_string),
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
        {"is_int", CastToBuiltin(&RnBuiltins_Type::rn_builtin_is_int),
         RnType::RN_BOOLEAN},
        {"titlecase", CastToBuiltin(&RnBuiltins_String::rn_builtin_titlecase),
         RnType::RN_STRING},
        {"lower", CastToBuiltin(&RnBuiltins_String::rn_builtin_lower),
         RnType::RN_STRING},
        {"upper", CastToBuiltin(&RnBuiltins_String::rn_builtin_upper),
         RnType::RN_STRING},
        {"snakecase", CastToBuiltin(&RnBuiltins_String::rn_builtin_snakecase),
         RnType::RN_STRING},
        {"str_split", CastToBuiltin(&RnBuiltins_String::rn_builtin_str_split),
         RnType::RN_STRING},
        {"str_merge", CastToBuiltin(&RnBuiltins_String::rn_builtin_str_merge),
         RnType::RN_STRING},
        {"str_substr", CastToBuiltin(&RnBuiltins_String::rn_builtin_str_substr),
         RnType::RN_STRING},
        {"str_startwith", CastToBuiltin(&RnBuiltins_String::rn_builtin_str_startwith),
         RnType::RN_BOOLEAN},
        {"str_endswith", CastToBuiltin(&RnBuiltins_String::rn_builtin_str_endswith),
         RnType::RN_BOOLEAN},
        {"str_join", CastToBuiltin(&RnBuiltins_String::rn_builtin_str_join),
         RnType::RN_STRING},
        {"array_filter", CastToBuiltin(&RnBuiltins_Array::rn_builtin_array_filter),
         RnType::RN_ARRAY},
        {"array_union", CastToBuiltin(&RnBuiltins_Array::rn_builtin_array_union),
         RnType::RN_ARRAY},
        {"array_intersect",
         CastToBuiltin(&RnBuiltins_Array::rn_builtin_array_intersect),
         RnType::RN_ARRAY},
        {"count", CastToBuiltin(&RnBuiltins_Array::rn_builtin_count), RnType::RN_INT},
        {"array_merge", CastToBuiltin(&RnBuiltins_Array::rn_builtin_array_merge),
         RnType::RN_ARRAY},
        {"array_push", CastToBuiltin(&RnBuiltins_Array::rn_builtin_array_push),
         RnType::RN_VOID},
        {"array_pop", CastToBuiltin(&RnBuiltins_Array::rn_builtin_array_pop),
         RnType::RN_VOID},
        {"array_zeros", CastToBuiltin(&RnBuiltins_Array::rn_builtin_array_zeros),
         RnType::RN_ARRAY},
        {"array_fill", CastToBuiltin(&RnBuiltins_Array::rn_builtin_array_fill),
         RnType::RN_ARRAY},
        {"sequence", CastToBuiltin(&RnBuiltins_Array::rn_builtin_sequence),
         RnType::RN_ARRAY},
        {"unpack", CastToBuiltin(&RnBuiltins::rn_builtin_unpack), RnType::RN_VOID},
        {"call", CastToBuiltin(&RnBuiltins::rn_builtin_call), RnType::RN_VOID},
        {"system", CastToBuiltin(&RnBuiltins::rn_builtin_system), RnType::RN_VOID},
        {"lload", CastToBuiltin(&RnBuiltins::rn_builtin_lload), RnType::RN_OBJECT},
        {"bind", CastToBuiltin(&RnBuiltins::rn_builtin_bind), RnType::RN_VOID},
        {"setenv", CastToBuiltin(&RnBuiltins::rn_builtin_setenv), RnType::RN_INT},
        {"getenv", CastToBuiltin(&RnBuiltins::rn_builtin_getenv), RnType::RN_STRING},
        {"unsetenv", CastToBuiltin(&RnBuiltins::rn_builtin_unsetenv), RnType::RN_INT},
        {"listattr", CastToBuiltin(&RnBuiltins::rn_builtin_listattr), RnType::RN_ARRAY},
        {"attrpairs", CastToBuiltin(&RnBuiltins::rn_builtin_attrpairs),
         RnType::RN_ARRAY}};

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
