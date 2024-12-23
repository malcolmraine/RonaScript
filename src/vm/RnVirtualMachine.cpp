/*****************************************************************************
 * File: RnVirtualMachine.cpp
 * Description:
 * Author: Malcolm Hall
 * Date: 6/19/22
 * Version: 1
 *
* MIT License
*
* Copyright (c) 2020 - 2023 Malcolm Hall
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

#include <memory>
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
#include "../objects/RnAnyObject.h"
#include "../objects/RnPackedObject.h"
#include "../util/StopWatch.h"
#include "RnFunction.h"
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
    RnObject* obj = CreateObject(RnType::RN_OBJECT);
    obj->SetData(CreateScope());
    RnScope* scope = obj->ToScope();
    scope->GetMemoryGroup()->AddObject(obj);
    if (!_scopes.empty()) {
        scope->SetParent(GetScope());
    }
    _scopes.push_back(scope);
    _memory_manager->SetRootMemoryGroup(GetScope()->GetMemoryGroup());
    RegisterBuiltins();
}

/*****************************************************************************/
RnIntNative RnVirtualMachine::HandleObjectUnpack(RnObject* obj) {
    auto unpack_obj = dynamic_cast<RnPackedObject*>(obj);
    unpack_obj->UnpackToStack(GetStack(), false);
    RnIntNative item_count = unpack_obj->GetDataItemCount();
    GetScope()->IncrementStackCount(item_count);
    return item_count;
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
    for (RnInstruction* instruction : _instructions) {
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
RnObject* RnVirtualMachine::CallFunction(RnFunction* func, const RnArrayNative& args) {
    if (func->IsBuiltIn()) {
        return func->Call(args);
    } else {
        RnScope* scope = RnMemoryManager::CreateScope();
        scope->SetParent(func->GetScope());
        func->InitScope(scope);
        _scopes.push_back(scope);
        CallStackPush(scope);
        func->PassArguments(args, scope);

        bool has_returned = false;
        size_t func_index = func->GetIStart();
        size_t end_index = func->GetIStart() + func->GetICnt();
        RnCodeFrame* previous_frame = _current_frame;
        _current_frame = func->GetCodeFrame();

        for (; func_index <= end_index; func_index++) {
            ExecuteInstruction(has_returned, func_index);
            if (has_returned) {
                break;
            }
        }
        _current_frame = previous_frame;

        CallStackPop();
        PopScope();

        for (int i = 0; i < scope->GetLinkedScopeCount(); i++) {
            PopScope();
        }

        while (scope->GetStackCount() > 0) {
            StackPop();
        }

        RnObject* ret_val = nullptr;
        if (has_returned) {
            ret_val = scope->ret_val;
        } else {
            ret_val = RnObject::Create(RnType::RN_NULL);
        }
        RnMemoryManager::DestroyScope(scope);
        return ret_val;
    }
}

#define SIMPLE_BINARY_OPERATION(op) \
    RnObject* b = StackPop();       \
    RnObject* a = StackPop();       \
    RnObject* result = *a op b;     \
    StackPush(result);              \
    PREDICT_OPCODE2(OP_LOAD_VALUE, OP_LOAD_LITERAL)

// This is exactly how cpython handles opcode prediction, so all credit to the
// authors there
#ifdef ENABLE_OPCODE_PREDICTION
#define PREDICT_OPCODE(op)                                   \
    {                                                        \
        if (GET_INSTRUCTION(index + 1)->GetOpcode() == op) { \
            instruction = GET_INSTRUCTION(++index);          \
            goto TARGET_##op;                                \
        }                                                    \
    }

#define PREDICT_OPCODE2(op1, op2)                          \
    {                                                      \
        switch (GET_INSTRUCTION(index + 1)->GetOpcode()) { \
            case op1:                                      \
                instruction = GET_INSTRUCTION(++index);    \
                goto TARGET_##op1;                         \
            case op2:                                      \
                instruction = GET_INSTRUCTION(++index);    \
                goto TARGET_##op2;                         \
            default:                                       \
                break;                                     \
        }                                                  \
    }

#define PREDICT_OPCODE3(op1, op2, op3)                     \
    {                                                      \
        switch (GET_INSTRUCTION(index + 1)->GetOpcode()) { \
            case op1:                                      \
                instruction = GET_INSTRUCTION(++index);    \
                goto TARGET_##op1;                         \
            case op2:                                      \
                instruction = GET_INSTRUCTION(++index);    \
                goto TARGET_##op2;                         \
            case op3:                                      \
                instruction = GET_INSTRUCTION(++index);    \
                goto TARGET_##op3;                         \
            default:                                       \
                break;                                     \
        }                                                  \
    }

#define PREDICTION_TARGET(op) TARGET_##op:
#else
#define PREDICT_OPCODE(op)
#define PREDICT_OPCODE2(op, op1)
#define PREDICT_OPCODE3(op, op1, op2)
#define PREDICTION_TARGET(op)
#endif

#define GET_INSTRUCTION(i) _current_frame->GetInstruction(i)

/*****************************************************************************/
void RnVirtualMachine::ExecuteInstruction(bool& break_scope, size_t& index) {
    if (_gc_count > 2000) {
        //        std::cout << "Garbage collecting...\n";
        _memory_manager->GCMark();
        RnMemoryManager::GCSweep();
        _gc_count = 0;
    }

    RnInstruction* instruction = GET_INSTRUCTION(index);

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
            RnObject* b = StackPop();
            RnObject* a = StackPop();
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
            RnObject* obj = StackPop();
            RnObject* value = StackPop();
            obj->CopyDataFromObject(value);
            break;
        }
        case OP_POP: {
            PREDICTION_TARGET(OP_POP)
            StackPop();
            break;
        }
        case OP_UNARY_NOT: {
            RnObject* obj = StackPop();
            RnObject* result = RnObject::Create(!obj->ToBool());
            GetScope()->GetMemoryGroup()->AddObject(result);
            StackPush(result);
            break;
        }
        case OP_TRY_CONTEXT: {
            break;
        }
        case OP_UNARY_INVERT: {
            RnObject* obj = StackPop();
            RnObject* result = RnObject::Create(~obj->ToInt());
            GetScope()->GetMemoryGroup()->AddObject(result);
            StackPush(result);
            break;
        }
        case OP_FAST_SUB: {
            PREDICTION_TARGET(OP_FAST_SUB)
            RnObject* obj = GetScope()->GetObject(instruction->GetArg1());
            obj->SetData(obj->ToFloat() - 1);
            break;
        }
        case OP_FAST_ADD: {
            PREDICTION_TARGET(OP_FAST_ADD)
            RnObject* obj = GetScope()->GetObject(instruction->GetArg1());
            obj->SetData(obj->ToFloat() + 1);
            break;
        }
        case OP_UNARY_NEGATION: {
            RnObject* obj = StackPop();
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
        case OP_CHECK_MEMBERSHIP: {
            RnObject* lhs_object = StackPop();
            RnObject* rhs_object = StackPop();

            if (lhs_object->GetType() == RnType::RN_ARRAY) {
                auto array_object = dynamic_cast<RnArrayObject*>(lhs_object);
                assert(array_object);
                RnObject* result = CreateObject(
                    static_cast<RnBoolNative>(array_object->Contains(rhs_object)));
                StackPush(result);
            } else if (lhs_object->GetType() == RnType::RN_ANY) {
                auto any_object = dynamic_cast<RnAnyObject*>(lhs_object);
                assert(any_object);
                RnObject* result = CreateObject(
                    static_cast<RnBoolNative>(any_object->Contains(rhs_object)));
                StackPush(result);
            } else {
                auto class_object = dynamic_cast<RnClassObject*>(lhs_object);
                if (class_object) {
                    RnObject* result = CreateObject(
                        static_cast<RnBoolNative>(class_object->Contains(rhs_object)));
                    StackPush(result);
                } else {
                    RnObject::ThrowUndefinedOperatorError("in", rhs_object, lhs_object);
                }
            };
            break;
        }
        case OP_RETURN: {
            RnScope* function_scope = _call_stack.back();
            if (!function_scope) {
                throw std::runtime_error("Cannot return outside of a function");
            }
            // Have to get the parent of the working scope, not the argument scope
            RnScope* ret_scope = _scopes[_scopes.size() - 2];
            function_scope->ret_val = StackPop();
            break_scope = true;
            break;
        }
        case OP_LOAD_LITERAL: {
            PREDICTION_TARGET(OP_LOAD_LITERAL)

            RnObject* obj = nullptr;
            RnInstructionArg key = instruction->GetArg1();
            if (key == UINT32_MAX) {
                obj = CreateObject(RnType::RN_NULL);
            } else {
                obj = RnConstStore::GetInternedObject(key);
            }
            StackPush(obj);
            PREDICT_OPCODE2(OP_LOAD_VALUE, OP_LOAD_LITERAL)
            break;
        }
        case OP_LOAD_VALUE: {
            PREDICTION_TARGET(OP_LOAD_VALUE)
            RnInstructionArg key = instruction->GetArg1();

            RnObject* object = GetScope()->GetObject(key);
            if (object) {
                if (object->IsClass() &&
                    GET_INSTRUCTION(index + 1)->GetOpcode() == OP_CALL) {
                    auto class_obj = dynamic_cast<RnClassObject*>(object);
                    auto instance =
                        dynamic_cast<RnClassObject*>(CreateObject(RnType::RN_OBJECT));
                    instance->SetData(CreateScope());
                    instance->ToScope()->SetParent(class_obj->ToScope());
                    instance->SetDefinition(class_obj);
                    class_obj->CopySymbols(instance->GetScope());
                    BindThis(instance->GetScope(), instance);
                    BindCls(instance->GetScope(), class_obj);

                    if (!class_obj->ToScope()) {
                        throw std::runtime_error(
                            "Cannot call constructor routine on null object");
                    }

                    RnObject* func_obj =
                        class_obj->ToScope()->GetObject(_object_construct_key);
                    RnFunction* func = func_obj->ToFunction();
                    RnScope* func_scope = CreateScope();
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
            PREDICT_OPCODE3(OP_LOAD_VALUE, OP_LOAD_LITERAL, OP_STORE)
            break;
        }
        case OP_LOAD_NULL: {
            StackPush(RnMemoryManager::CreateObject(RnType::RN_NULL));
            break;
        }
        case OP_CALL: {
            RnObject* stack_val = StackPop();
            RnObject* func_obj = nullptr;
            if (stack_val->GetType() == RnType::RN_OBJECT) {
                auto class_obj = dynamic_cast<RnClassObject*>(stack_val);
                auto instance =
                    dynamic_cast<RnClassObject*>(CreateObject(RnType::RN_OBJECT));
                GetScope()->GetMemoryGroup()->AddObject(instance);
                instance->ToScope()->SetParent(class_obj->ToScope());
                class_obj->CopySymbols(instance->GetScope());

                BindThis(instance->GetScope(), instance);
                BindCls(instance->GetScope(), class_obj);

                auto constructor_obj = dynamic_cast<RnFunctionObject*>(
                    class_obj->ToScope()->GetObject(_object_construct_key));
                RnFunction* func = constructor_obj->ToFunction();
                RnScope* func_scope = RnObject::Create(RnType::RN_OBJECT)->ToScope();
                func_scope->SetParent(instance->GetScope());

                BindThis(func_scope, instance);
                func->SetScope(func_scope);
                func_obj = constructor_obj;
            } else {
                func_obj = stack_val;
            }

            RnIntNative arg_count = instruction->GetArg1();
            RnArrayNative args;
            args.reserve(arg_count);
            for (uint32_t i = arg_count; i > 0; --i) {
                auto stack_obj = StackPop();
                if (stack_obj->GetType() == RnType::RN_OBJECT_PACK) {
                    i += HandleObjectUnpack(stack_obj);
                    continue;
                }
                args.push_back(stack_obj);
            }
            std::reverse(args.begin(), args.end());

            RnObject* ret_val = CallFunction(func_obj->ToFunction(), args);
            StackPush(ret_val);
            PREDICT_OPCODE(OP_POP)
            break;
        }
        case OP_MAKE_CONST: {
            auto type = static_cast<RnType::Type>(instruction->GetArg1());
            RnObject* obj = CreateObject(type);

//            RnInstructionArg key = instruction->GetArg3();
//            if (key == UINT32_MAX) {
//                throw std::runtime_error("Invalid literal key while making const value");
//            } else {
//                obj->CopyDataFromObject(RnConstStore::GetInternedObject(key));
//            }

            obj->SetConstFlag(true);
            GetScope()->GetMemoryGroup()->AddObject(obj);
            GetScope()->StoreObject(instruction->GetArg2(), obj);
            break;
        }
        case OP_MAKE_LOCAL: {
            auto type = static_cast<RnType::Type>(instruction->GetArg1());
            RnObject* obj = GetScope()->MakeLocal(type);
            GetScope()->StoreObject(instruction->GetArg2(), obj);
            break;
        }
        case OP_MAKE_VAR: {
            auto type = static_cast<RnType::Type>(instruction->GetArg1());
            RnObject* obj = CreateObject(type);
            GetScope()->StoreObject(instruction->GetArg2(), obj);
            break;
        }
        case OP_MAKE_GLOBAL: {
            auto type = static_cast<RnType::Type>(instruction->GetArg1());
            RnObject* obj = RnMemoryManager::CreateObject(type);
            _scopes.front()->GetMemoryGroup()->AddObject(obj);
            _scopes.front()->StoreObject(instruction->GetArg2(), obj);
            break;
        }
        case OP_MAKE_CLASS: {
            RnObject* name_obj =
                RnConstStore::GetInternedObject(instruction->GetArg1());
            name_obj->SetConstFlag(true);
            auto obj =
                dynamic_cast<RnClassObject*>(RnObject::Create(RnType::RN_OBJECT));
            auto scope = CreateScope();
            obj->SetData(scope);
            scope->SetParent(GetScope());
            obj->SetIsClass(true);
            obj->SetName(name_obj->ToString());
            obj->GetScope()->StoreObject(RnConstStore::InternValue("__class"),
                                         name_obj);
            GetScope()->StoreObject(instruction->GetArg1(), obj);
            RnScope* class_scope = obj->ToScope();

            if (!class_scope) {
                throw std::runtime_error("Invalid use of null object.");
            }

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
            auto obj =
                dynamic_cast<RnFunctionObject*>(CreateObject(RnType::RN_FUNCTION));
            RnStringNative name =
                RnConstStore::GetInternedString(instruction->GetArg1());
            auto type = static_cast<RnType::Type>(instruction->GetArg2());
            RnInstructionArg scope_size = instruction->GetArg3();
            void* func_addr = RnLinearAllocator::Instance()->Malloc(sizeof(RnFunction));
            auto func = std::construct_at<RnFunction>(
                reinterpret_cast<RnFunction*>(func_addr), name, index + 1, scope_size);
            func->SetReturnType(type);
            RnScope* func_scope = RnMemoryManager::CreateScope();
            func_scope->SetParent(GetScope());
            func->SetScope(func_scope);
            func->SetCodeFrame(_current_frame);
            obj->SetData(func);

            uint32_t i = 0;  // Argument count
            for (; GET_INSTRUCTION(i + index + 1)->GetOpcode() == OP_MAKE_ARG; i++) {
                RnInstruction* arg_instruction = GET_INSTRUCTION(i + index + 1);
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
        case OP_MAKE_CLOSURE: {
            auto obj =
                dynamic_cast<RnFunctionObject*>(CreateObject(RnType::RN_FUNCTION));
            auto type = static_cast<RnType::Type>(instruction->GetArg1());
            RnInstructionArg scope_size = instruction->GetArg2();
            void* func_addr = RnLinearAllocator::Instance()->Malloc(sizeof(RnFunction));
            auto func =
                std::construct_at<RnFunction>(reinterpret_cast<RnFunction*>(func_addr),
                                              "closure", index + 1, scope_size);
            func->SetReturnType(type);
            RnScope* func_scope = RnMemoryManager::CreateScope();
            func_scope->SetParent(GetScope());
            func->SetScope(func_scope);
            func->SetCodeFrame(_current_frame);
            obj->SetData(func);

            uint32_t i = 0;  // Argument count
            for (; GET_INSTRUCTION(i + index + 1)->GetOpcode() == OP_MAKE_ARG; i++) {
                RnInstruction* arg_instruction = GET_INSTRUCTION(i + index + 1);
                func->CreateArgument(
                    arg_instruction->GetArg2(),
                    static_cast<RnType::Type>(arg_instruction->GetArg1()), i);
            }

            index += scope_size + i;
            func->SetICnt(scope_size + i);
            StackPush(obj);
            break;
        }
        case OP_CREATE_CONTEXT: {
            RnScope* scope = RnMemoryManager::CreateScope();
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
            RnScope* scope = _scopes.back();
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
            PREDICT_OPCODE2(OP_FAST_ADD, OP_FAST_SUB)
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
            RnInstructionArg steps = instruction->GetArg1();
            RnIntNative condition = StackPop()->ToInt();
            if (!condition) {
                index += steps;
            }
            break;
        }
        case OP_JUMPB_IF: {
            RnInstructionArg steps = instruction->GetArg1();
            RnIntNative condition = StackPop()->ToInt();
            if (!condition) {
                index -= steps;
            }
            break;
        }
        case OP_EXIT: {
            break_scope = true;
            _should_exit = true;
            break;
        }
        case OP_INDEX_ACCESS: {
            RnIntNative idx_value = StackPop()->ToInt();
            RnObject* obj = StackPop();

            // try/catch is used here to handle out of bounds access for performance
            // reasons. Checking the bounds for each access would be unnecessarily
            // costly.
            try {
                RnObject* result = obj->At(idx_value);
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

            RnIntNative item_count = instruction->GetArg1();
            for (RnIntNative i = 0; i < item_count; i++) {
                auto stack_obj = StackPop();
                if (stack_obj->GetActiveType() == RnType::RN_OBJECT_PACK) {
                    item_count += HandleObjectUnpack(stack_obj);
                    continue;
                }
                auto copy =
                    dynamic_cast<RnAnyObject*>(RnObject::Create(RnType::RN_ANY));
                copy->CopyFrom(stack_obj);
                GetScope()->GetMemoryGroup()->AddObject(copy);
                obj->Append(copy);
            }
            StackPush(obj);
            break;
        }
        case OP_UNPACK: {
            RnObject* stack_obj = StackPop();
            assert(stack_obj);
            assert(stack_obj->GetType() == RnType::RN_ARRAY ||
                   stack_obj->GetType() == RnType::RN_STRING);

            auto unpack_obj = CreateObject(RnType::RN_OBJECT_PACK);
            unpack_obj->SetData(stack_obj);
            StackPush(unpack_obj);
            break;
        }
        case OP_LOAD_ATTR: {
            RnObject* stack_obj = StackPop();
            assert(stack_obj);
            auto instance = dynamic_cast<RnClassObject*>(stack_obj);
            if (!instance) {
                throw std::runtime_error("Cannot get attribute from type '" +
                                         RnType::TypeToString(stack_obj->GetType()) +
                                         "'");
            }

            RnScope* scope = instance->GetScope();
            if (!scope) {
                throw std::runtime_error("Cannot get attribute from null object.");
            }

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
                BindThis(func->GetScope(), instance);
            }
            break;
        }
        case OP_IMPORT: {
            RnCodeFrame* import_frame =
                _current_frame->GetSubframe(instruction->GetArg1());
            RnScope* scope = _scopes.front();
            ExecuteCodeFrame(import_frame, scope);
            break;
        }
    }
}

/*****************************************************************************/
RnIntNative RnVirtualMachine::Run() {
    std::setvbuf(stdout, nullptr, _IOLBF, 65536);
    bool has_returned = false;
    StopWatch stopwatch;

    stopwatch.Start();
    while (i_idx < _instructions.size() && !_should_exit) {
        ExecuteInstruction(has_returned, i_idx);
        if (has_returned) {
            break;
        }
        i_idx++;
    }
    stopwatch.Stop();
    //                Log::INFO("\nRuntime duration: " + std::to_string(stopwatch.Duration()));
    return StackPop()->ToInt();
}

/*****************************************************************************/
RnIntNative RnVirtualMachine::ExecuteCodeFrame(RnCodeFrame* frame, RnScope* scope) {
    _scopes.push_back(scope);
    std::setvbuf(stdout, nullptr, _IOLBF, 65536);
    StopWatch stopwatch;

    RnCodeFrame* previous_frame = _current_frame;
    _current_frame = frame;
    uint32_t instructionCount = frame->GetInstructionCount();
    size_t instructionIndex = 0;
    bool has_returned = false;

    stopwatch.Start();
    while (instructionIndex < instructionCount && !_should_exit) {
        ExecuteInstruction(has_returned, instructionIndex);
        if (has_returned) {
            break;
        }
        instructionIndex++;
    }
    stopwatch.Stop();
    //    Log::INFO("\nRuntime duration: " + std::to_string(stopwatch.Duration()));
    _scopes.pop_back();
    _current_frame = previous_frame;
    return 0;
}

/*****************************************************************************/
void RnVirtualMachine::LoadInstructions(RnStdVector<RnInstruction*>& instructions) {
    _instructions.clear();
    i_idx = 0;
    std::swap(_instructions, instructions);
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
    auto obj = RnMemoryManager::CreateObject(type);
    if (!_scopes.empty()) {
        GetScope()->GetMemoryGroup()->AddObject(obj);
    }
    return obj;
}

/*****************************************************************************/
RnObject* RnVirtualMachine::CreateObject(RnStringNative data) {
    _gc_count++;
    auto obj = RnMemoryManager::Create(std::move(data));
    GetScope()->GetMemoryGroup()->AddObject(obj);
    return obj;
}

/*****************************************************************************/
RnObject* RnVirtualMachine::CreateObject(RnBoolNative data) {
    _gc_count++;
    auto obj = RnMemoryManager::Create(data);
    GetScope()->GetMemoryGroup()->AddObject(obj);
    return obj;
}

/*****************************************************************************/
RnObject* RnVirtualMachine::CreateObject(RnIntNative data) {
    _gc_count++;
    auto obj = RnMemoryManager::Create(data);
    GetScope()->GetMemoryGroup()->AddObject(obj);
    return obj;
}

/*****************************************************************************/
RnObject* RnVirtualMachine::CreateObject(RnFloatNative data) {
    _gc_count++;
    auto obj = RnMemoryManager::Create(data);
    GetScope()->GetMemoryGroup()->AddObject(obj);
    return obj;
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
        auto obj = dynamic_cast<RnFunctionObject*>(CreateObject(RnType::RN_FUNCTION));
        obj->SetData(func);
        GetScope()->StoreObject(RnConstStore::InternValue(std::get<0>(parts)), obj);
    }
}
