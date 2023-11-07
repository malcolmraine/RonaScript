/*****************************************************************************
 * File: RnVirtualMachine.cpp
 * Description:
 * Author: Malcolm Hall
 * Date: 6/19/22
 * Version: 1
 *
* MIT License
*
* Copyright (c) 2021 Malcolm Hall
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
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
#include "../builtins/RnBuiltins_System.h"
#include "../builtins/RnBuiltins_Type.h"
#include "../common/RnConfig.h"
#include "../util/log.h"
#include "../util/StopWatch.h"
#include "RnAnyObject.h"
#include "RnArrayObject.h"
#include "RnClassObject.h"
#include "RnFunction.h"
#include "RnFunctionObject.h"
#include "RnMemoryManager.h"
#include "RnOpCode.h"

// @formatter:off
#include "../common/RnBuildInfo.h"

RnVirtualMachine* RnVirtualMachine::_instance = nullptr;
RnIntNative RnVirtualMachine::_object_this_key = -1;
RnIntNative RnVirtualMachine::_object_cls_key = -1;
RnIntNative RnVirtualMachine::_object_construct_key = -1;

/*****************************************************************************/
RnVirtualMachine::RnVirtualMachine() {
    _scopes.reserve(16);
    _call_stack.reserve(RnConfig::GetCallStackMaxDepth());
    _stack.reserve(RnConfig::GetCallStackMaxDepth());
    _memory_manager = new RnMemoryManager();

    _object_this_key = RnConstStore::InternValue(static_cast<RnStringNative>("this"));
    _object_cls_key = RnConstStore::InternValue(static_cast<RnStringNative>("cls"));
    _object_construct_key =
        RnConstStore::InternValue(static_cast<RnStringNative>("construct"));
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
void RnVirtualMachine::CallStackPush(RnScope* scope) {
    if (_call_stack.size() >= RnConfig::GetCallStackMaxDepth()) {
        throw std::runtime_error("Exceeded maximum call stack size.");
    }
    _call_stack.push_back(scope);
}

/*****************************************************************************/
void RnVirtualMachine::CallStackPop() {
    _call_stack.pop_back();
}

/*****************************************************************************/
RnObject* RnVirtualMachine::CallFunction(RnFunction* func, RnArrayNative args) {
    if (func->IsBuiltIn()) {
        RnObject* ret_val = RnMemoryManager::CreateObject(func->GetReturnType());
        GetScope()->GetMemoryGroup()->AddObject(ret_val);
        func->Call(args, ret_val);
        return ret_val;
    } else {
        auto scope = RnMemoryManager::CreateScope();
        scope->SetParent(func->GetScope());
        func->InitScope(scope);
        _scopes.push_back(scope);
        CallStackPush(scope);
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

        CallStackPop();
        PopScope();

        for (int i = 0; i < scope->GetLinkedScopeCount(); i++) {
            PopScope();
        }

        for (size_t i = 0; i < scope->GetStackCount(); i++) {
            StackPop();
        }

        if (has_returned) {
            return scope->ret_val;
        } else {
            return RnObject::Create(RnType::RN_NULL);
        }
    }
}

#define SIMPLE_BINARY_OPERATION(op)                  \
    auto b = StackPop();                             \
    auto a = StackPop();                             \
    auto result = *a op b;                           \
    GetScope()->GetMemoryGroup()->AddObject(result); \
    StackPush(result);                               \
    PREDICT_OPCODE2(OP_LOAD_VALUE, OP_LOAD_LITERAL)

// This is exactly how cpython handles opcode prediction, so all credit to the
// authors there
#ifdef ENABLE_OPCODE_PREDICTION
#define PREDICT_OPCODE(op)                                 \
    {                                                      \
        if (_instructions[index + 1]->GetOpcode() == op) { \
            instruction = _instructions[++index];          \
            goto TARGET_##op;                              \
        }                                                  \
    }

#define PREDICT_OPCODE2(op1, op2)                        \
    {                                                    \
        switch (_instructions[index + 1]->GetOpcode()) { \
            case op1:                                    \
                instruction = _instructions[++index];    \
                goto TARGET_##op1;                       \
            case op2:                                    \
                instruction = _instructions[++index];    \
                goto TARGET_##op2;                       \
            default:                                     \
                break;                                   \
        }                                                \
    }

#define PREDICT_OPCODE3(op1, op2, op3)                   \
    {                                                    \
        switch (_instructions[index + 1]->GetOpcode()) { \
            case op1:                                    \
                instruction = _instructions[++index];    \
                goto TARGET_##op1;                       \
            case op2:                                    \
                instruction = _instructions[++index];    \
                goto TARGET_##op2;                       \
            case op3:                                    \
                instruction = _instructions[++index];    \
                goto TARGET_##op3;                       \
            default:                                     \
                break;                                   \
        }                                                \
    }

#define PREDICTION_TARGET(op) TARGET_##op:
#else
#define PREDICT_OPCODE(op)
#define PREDICT_OPCODE2(op, op1)
#define PREDICT_OPCODE3(op, op1, op2)
#define PREDICTION_TARGET(op)
#endif

/*****************************************************************************/
void RnVirtualMachine::ExecuteInstruction(bool& break_scope, size_t& index) {
    if (_gc_count > 20) {
        //        std::cout << "Garbage collecting...\n";
        //        _memory_manager->GCMark();
        //        _memory_manager->GCSweep();
        _gc_count = 0;
    }

    auto instruction = _instructions[index];
    //    Log::INFO(instruction->ToString());
    switch (instruction->GetOpcode()) {
        case OP_BINARY_ADD: {
            SIMPLE_BINARY_OPERATION(+)
            break;
        }
        case OP_BINARY_SUB: {
            SIMPLE_BINARY_OPERATION(-)
            break;
        }
        case OP_BINARY_MUL: {
            SIMPLE_BINARY_OPERATION(*)
            break;
        }
        case OP_BINARY_DIV: {
            SIMPLE_BINARY_OPERATION(/)
            break;
        }
        case OP_BINARY_MOD: {
            SIMPLE_BINARY_OPERATION(%)
            break;
        }
        case OP_BINARY_GTE: {
            SIMPLE_BINARY_OPERATION(>=)
            PREDICT_OPCODE(OP_JUMPF_IF)
            break;
        }
        case OP_BINARY_LTE: {
            SIMPLE_BINARY_OPERATION(<=)
            PREDICT_OPCODE(OP_JUMPF_IF)
            break;
        }
        case OP_BINARY_GT: {
            SIMPLE_BINARY_OPERATION(>)
            PREDICT_OPCODE(OP_JUMPF_IF)
            break;
        }
        case OP_BINARY_LT: {
            SIMPLE_BINARY_OPERATION(<)
            PREDICT_OPCODE(OP_JUMPF_IF)
            break;
        }
        case OP_BINARY_EQ: {
            SIMPLE_BINARY_OPERATION(==)
            PREDICT_OPCODE(OP_JUMPF_IF)
            break;
        }
        case OP_BINARY_NEQ: {
            SIMPLE_BINARY_OPERATION(!=)
            PREDICT_OPCODE(OP_JUMPF_IF)
            break;
        }
        case OP_BINARY_POWER: {
            auto b = StackPop();
            auto a = StackPop();
            RnObject* result = nullptr;
            if (b->GetType() == RnType::RN_INT) {
                if (b->ToInt() == 2) {
                    result = CreateObject(a->ToFloat() * a->ToFloat());
                } else if (b->ToInt() == 3) {
                    result = CreateObject(a->ToFloat() * a->ToFloat() * a->ToFloat());
                } else {
                    result = CreateObject(std::pow(a->ToFloat(), b->ToFloat()));
                }
            } else {
                result = CreateObject(std::pow(a->ToFloat(), b->ToFloat()));
            }
            assert(result);
            GetScope()->GetMemoryGroup()->AddObject(result);
            StackPush(result);
            PREDICT_OPCODE2(OP_LOAD_VALUE, OP_LOAD_LITERAL)
            break;
        }
        case OP_BINARY_RSH: {
            SIMPLE_BINARY_OPERATION(<<)
            break;
        }
        case OP_BINARY_LSH: {
            SIMPLE_BINARY_OPERATION(>>)
            break;
        }
        case OP_BINARY_OR: {
            SIMPLE_BINARY_OPERATION(|)
            break;
        }
        case OP_BINARY_XOR: {
            SIMPLE_BINARY_OPERATION(^)
            break;
        }
        case OP_BINARY_AND: {
            SIMPLE_BINARY_OPERATION(&)
            break;
        }
        case OP_LOGICAL_OR: {
            SIMPLE_BINARY_OPERATION(||)
            break;
        }
        case OP_LOGICAL_AND: {
            SIMPLE_BINARY_OPERATION(&&)
            break;
        }
        case OP_STORE: {
            PREDICTION_TARGET(OP_STORE)
            auto obj = StackPop();
            auto value = StackPop();
            obj->CopyDataFromObject(value);
            break;
        }
        case OP_POP: {
            PREDICTION_TARGET(OP_POP)
            StackPop();
            break;
        }
        case OP_UNARY_NOT: {
            auto obj = StackPop();
            auto result = RnObject::Create(!obj->ToBool());
            GetScope()->GetMemoryGroup()->AddObject(result);
            StackPush(result);
            break;
        }
        case OP_TRY_CONTEXT: {
            break;
        }
        case OP_UNARY_INVERT: {
            auto obj = StackPop();
            auto result = RnObject::Create(~obj->ToInt());
            GetScope()->GetMemoryGroup()->AddObject(result);
            StackPush(result);
            break;
        }
        case OP_UNARY_DECREMENT: {
            PREDICTION_TARGET(OP_UNARY_DECREMENT)
            auto obj = GetScope()->GetObject(instruction->GetArg1());
            obj->SetData(obj->ToFloat() - 1);
            break;
        }
        case OP_UNARY_INCREMENT: {
            PREDICTION_TARGET(OP_UNARY_INCREMENT)
            auto obj = GetScope()->GetObject(instruction->GetArg1());
            obj->SetData(obj->ToFloat() + 1);
            break;
        }
        case OP_UNARY_NEGATION: {
            auto obj = StackPop();
            RnObject* result = nullptr;
            if (obj->GetActiveType() == RnType::RN_FLOAT) {
                result = RnObject::Create(-obj->ToFloat());
            } else if (obj->GetActiveType() == RnType::RN_INT) {
                result = RnObject::Create(-obj->ToInt());
            } else {
                throw std::runtime_error(
                    "Cannot apply unary negation to non-numeric type");
            }
            GetScope()->GetMemoryGroup()->AddObject(result);
            StackPush(result);

            break;
        }
        case OP_RETURN: {
            auto function_scope = _call_stack.back();
            if (!function_scope) {
                throw std::runtime_error("Cannot return outside of a function");
            }
            // Have to get the parent of the working scope, not the argument scope
            auto ret_scope = _scopes[_scopes.size() - 2];
            function_scope->ret_val = StackPop();
            break_scope = true;
            break;
        }
        case OP_LOAD_LITERAL: {
            PREDICTION_TARGET(OP_LOAD_LITERAL)
            auto obj = RnConstStore::GetInternedObject(instruction->GetArg1());
            StackPush(obj);
            PREDICT_OPCODE2(OP_LOAD_VALUE, OP_LOAD_LITERAL)
            break;
        }
        case OP_LOAD_VALUE: {
            PREDICTION_TARGET(OP_LOAD_VALUE)
            auto key = instruction->GetArg1();
            auto object = GetScope()->GetObject(key);

            if (object) {
                if (object->IsClass() &&
                    _instructions[index + 1]->GetOpcode() == OP_CALL) {
                    auto class_obj = dynamic_cast<RnClassObject*>(object);
                    auto instance = dynamic_cast<RnClassObject*>(
                        RnMemoryManager::CreateObject(RnType::RN_CLASS_INSTANCE));
                    GetScope()->GetMemoryGroup()->AddObject(instance);
                    instance->ToObject()->SetParent(class_obj->ToObject());
                    instance->SetDefinition(class_obj);
                    class_obj->CopySymbols(instance->GetScope());
                    BindThis(instance->GetScope(), instance);
                    BindCls(instance->GetScope(), class_obj);
                    auto func_obj =
                        class_obj->ToObject()->GetObject(_object_construct_key);
                    auto func = func_obj->ToFunction();
                    auto func_scope = RnObject::Create(RnType::RN_OBJECT)->ToObject();
                    func_scope->SetParent(instance->GetScope());
                    BindThis(func_scope, instance);
                    func->SetScope(func_scope);
                    StackPush(func_obj);
                } else {
                    StackPush(object);
                }
            } else {
                throw std::runtime_error("Symbol does not exist: " +
                                         RnConstStore::GetInternedString(key));
            }
            PREDICT_OPCODE3(OP_LOAD_VALUE, OP_LOAD_LITERAL, OP_CALL)
            PREDICT_OPCODE(OP_STORE)
            break;
        }
        case OP_LOAD_NULL: {
            StackPush(RnMemoryManager::CreateObject(RnType::RN_NULL));
            break;
        }
        case OP_CALL: {
            PREDICTION_TARGET(OP_CALL)
            auto stack_val = StackPop();
            RnObject* func_obj = nullptr;
            if (stack_val->GetType() == RnType::RN_OBJECT) {
                auto class_obj = dynamic_cast<RnClassObject*>(stack_val);
                if (class_obj->IsModule()) {
                    throw std::runtime_error("Cannot instantiate module '" +
                                             class_obj->GetName() + "'");
                }

                auto instance = dynamic_cast<RnClassObject*>(
                    RnMemoryManager::CreateObject(RnType::RN_CLASS_INSTANCE));
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
                //                StackPush(constructor_obj);
                func_obj = constructor_obj;
            } else {
                func_obj = stack_val;
            }

            RnArrayNative args;
            auto func = func_obj->ToFunction();
            args.reserve(instruction->GetArg1());
            for (uint32_t i = 0; i < instruction->GetArg1(); i++) {
                args.push_back(StackPop());
            }
            std::reverse(args.begin(), args.end());

            auto ret_val = CallFunction(func_obj->ToFunction(), args);
            StackPush(ret_val);
            PREDICT_OPCODE(OP_POP)
            break;
        }
        case OP_MAKE_CONST: {
            auto type = static_cast<RnType::Type>(instruction->GetArg1());
            auto obj = RnMemoryManager::CreateObject(type);
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
            auto obj = RnMemoryManager::CreateObject(type);
            GetScope()->GetMemoryGroup()->AddObject(obj);
            GetScope()->StoreObject(instruction->GetArg2(), obj);
            break;
        }
        case OP_MAKE_GLOBAL: {
            auto type = static_cast<RnType::Type>(instruction->GetArg1());
            auto obj = RnMemoryManager::CreateObject(type);
            _scopes.front()->GetMemoryGroup()->AddObject(obj);
            _scopes.front()->StoreObject(instruction->GetArg2(), obj);
            break;
        }
        case OP_MAKE_MODULE: {
            auto name = RnConstStore::GetInternedString(instruction->GetArg1());
            auto obj = dynamic_cast<RnClassObject*>(
                RnObject::Create(RnType::RN_CLASS_INSTANCE));
            obj->SetIsModule(true);
            obj->GetScope()->SetParent(GetScope());
            GetScope()->StoreObject(instruction->GetArg1(), obj);
            //            _namespaces[instruction->GetArg1()] = obj;
            _scopes.push_back(obj->ToObject());
            index++;
            size_t stop_index = index + instruction->GetArg2();
            for (; index < stop_index; index++) {
                ExecuteInstruction(break_scope, index);
            }
            index--;
            PopScope();
            break;
        }
        case OP_MAKE_CLASS: {
            auto name_obj = RnConstStore::GetInternedObject(instruction->GetArg1());
            name_obj->SetConstFlag(true);
            auto obj = dynamic_cast<RnClassObject*>(
                RnObject::Create(RnType::RN_CLASS_INSTANCE));
            obj->SetIsClass(true);
            obj->SetName(name_obj->ToString());
            obj->GetScope()->StoreObject(RnConstStore::InternValue("__class"),
                                         name_obj);
            GetScope()->StoreObject(instruction->GetArg1(), obj);
            auto class_scope = obj->ToObject();
            class_scope->SetParent(GetScope());
            _scopes.push_back(class_scope);
            index++;
            size_t stop_index = index + instruction->GetArg2();
            for (; index < stop_index; index++) {
                ExecuteInstruction(break_scope, index);
            }
            index--;
            PopScope();
            break;
        }
        case OP_MAKE_FUNC: {
            auto obj = dynamic_cast<RnFunctionObject*>(
                RnMemoryManager::CreateObject(RnType::RN_FUNCTION));
            GetScope()->GetMemoryGroup()->AddObject(obj);
            auto name = RnConstStore::GetInternedString(instruction->GetArg1());
            auto type = static_cast<RnType::Type>(instruction->GetArg2());
            auto scope_size = instruction->GetArg3();
            auto func = new RnFunction(name, index + 1, scope_size);
            func->SetReturnType(type);
            func->SetScope(new RnScope(GetScope()));
            obj->SetData(func);

            uint32_t i = 0;  // Argument count
            for (; _instructions[i + index + 1]->GetOpcode() == OP_MAKE_ARG; i++) {
                auto arg_instruction = _instructions[i + index + 1];
                func->CreateArgument(
                    arg_instruction->GetArg2(),
                    static_cast<RnType::Type>(arg_instruction->GetArg1()), i);
            }

            index += scope_size + i;
//            func->SetIStart(func->GetIStart() + i);
            func->SetICnt(scope_size + i);
            GetScope()->StoreObject(instruction->GetArg1(), obj);
            break;
        }
        case OP_CREATE_CONTEXT: {
            auto scope = RnMemoryManager::CreateScope();
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
            PREDICTION_TARGET(OP_DESTROY_CONTEXT)
            auto scope = _scopes.back();
            PopScope();
            RnMemoryManager::DestroyScope(scope);

            if (!_call_stack.empty()) {
                _call_stack.back()->DecrLinkedScopeCount();
            }
            PREDICT_OPCODE(OP_DESTROY_CONTEXT)
            break;
        }
        case OP_RESET_CONTEXT: {
            GetScope()->Reset();
            PREDICT_OPCODE2(OP_UNARY_INCREMENT, OP_UNARY_DECREMENT)
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
            PREDICTION_TARGET(OP_JUMPF_IF)
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
            auto obj = RnMemoryManager::CreateObject(RnType::RN_INT);
            GetScope()->GetMemoryGroup()->AddObject(obj);
            obj->SetData(static_cast<RnIntNative>(instruction->GetArg1()));
            StackPush(obj);
            break;
        }
        case OP_INDEX_ACCESS: {
            auto idx_value = StackPop()->ToInt();
            auto obj = StackPop();

            // try/catch is used here to handle out of bounds access for performance
            // reasons. Checking the bounds for each access would be unnecessarily
            // costly.
            try {
                auto result = obj->At(idx_value);
                StackPush(result);
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
                auto copy =
                    dynamic_cast<RnAnyObject*>(RnObject::Create(RnType::RN_ANY));
                copy->CopyFrom(StackPop());
                GetScope()->GetMemoryGroup()->AddObject(copy);
                obj->Append(copy);
            }
            StackPush(obj);
            break;
        }
        case OP_ATTR_ACCESS: {
            auto object = dynamic_cast<RnClassObject*>(StackPop());
            auto scope = object->GetScope();
            RnObject* result = nullptr;
            if (scope->GetSymbolTable()->SymbolExists(instruction->GetArg1(), false)) {
                result = scope->GetObject(instruction->GetArg1());
                StackPush(result);
            } else if (scope->GetParent() &&
                       scope->GetParent()->GetSymbolTable()->SymbolExists(
                           instruction->GetArg1(), false)) {
                result = scope->GetParent()->GetObject(instruction->GetArg1());
                StackPush(result);
            } else {
                throw std::runtime_error(
                    "Object has no attribute '" +
                    RnConstStore::GetInternedString(instruction->GetArg1()) + "'");
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
            auto nspace = dynamic_cast<RnClassObject*>(StackPop());
            auto object = nspace->GetScope()->GetSymbolTable()->GetObject(
                instruction->GetArg1(), true);
            if (object->GetType() == RnType::RN_FUNCTION && !nspace->IsModule()) {
                BindCls(object->ToFunction()->GetScope(), nspace);
                BindThis(object->ToFunction()->GetScope(), nspace);
            }
            StackPush(object);
            break;
        }
    }
}

/*****************************************************************************/
RnIntNative RnVirtualMachine::Run() {
    std::setvbuf(stdout, nullptr, _IOLBF, 65536);
    bool has_returned = false;
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
            Log::INFO("\nRuntime duration: " + std::to_string(stopwatch.Duration()));
    return StackPop()->ToInt();
}

/*****************************************************************************/
void RnVirtualMachine::LoadInstructions(std::vector<RnInstruction*> instructions) {
    _instructions.clear();
    i_idx = 0;
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
    _gc_count++;
    return RnMemoryManager::CreateObject(type);
}

/*****************************************************************************/
RnObject* RnVirtualMachine::CreateObject(RnStringNative data) {
    _gc_count++;
    return RnMemoryManager::Create(std::move(data));
}

/*****************************************************************************/
RnObject* RnVirtualMachine::CreateObject(RnBoolNative data) {
    _gc_count++;
    return RnMemoryManager::Create(data);
}

/*****************************************************************************/
RnObject* RnVirtualMachine::CreateObject(RnIntNative data) {
    _gc_count++;
    return RnMemoryManager::Create(data);
}

/*****************************************************************************/
RnObject* RnVirtualMachine::CreateObject(RnFloatNative data) {
    _gc_count++;
    return RnMemoryManager::Create(data);
}

/*****************************************************************************/
RnScope* RnVirtualMachine::CreateScope() {
    _gc_count++;
    return RnMemoryManager::CreateScope();
}

/*****************************************************************************/
#undef RN_BUILTIN_FUNC
#define RN_BUILTIN_FUNC(ns, name, retval, argcnt) \
    {#name, CastToBuiltin(&ns::rn_builtin_##name), retval},

void RnVirtualMachine::RegisterBuiltins() {
    std::vector<std::tuple<RnStringNative, BuiltinFunction, RnType::Type>> functions = {
        RN_BUILTIN_REGISTRATIONS};

    for (auto parts : functions) {
        auto func = new RnBuiltinFunction(std::get<0>(parts), std::get<1>(parts));
        func->SetScope(GetScope());
        func->SetReturnType(std::get<2>(parts));
        auto obj = dynamic_cast<RnFunctionObject*>(
            RnMemoryManager::CreateObject(RnType::RN_FUNCTION));
        GetScope()->GetMemoryGroup()->AddObject(obj);
        obj->SetData(func);
        GetScope()->StoreObject(RnConstStore::InternValue(std::get<0>(parts)), obj);
    }
}
