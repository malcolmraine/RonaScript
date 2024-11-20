/*****************************************************************************
* File: RnVirtualMachine.h
* Description:
* Author: Malcolm Hall
* Date: 6/20/22
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
******************************************************************************/

#pragma once

#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>
#include "../codegen/RnCodeFrame.h"
#include "../codegen/RnInstruction.h"
#include "../common/RnInternment.h"
#include "RnScope.h"

class RnObject;
class RnFunctionObject;
class RnFunction;
class RnClassObject;
class RnMemoryManager;

class RnVirtualMachine {
public:
    ~RnVirtualMachine();

    inline RnScope* GetScope() {
        return _scopes.back();
    }

    inline void PopScope() {
        if (_scopes.size() > 1)
            _scopes.pop_back();
    }

    [[nodiscard]] inline RnArrayNative& GetStack() {
        return _stack;
    }

    inline RnObject* StackPop() {
        if (GetStack().empty())
            return nullptr;

        auto item = GetStack().back();
        GetStack().pop_back();
        assert(item);
        GetScope()->DecrementStackCount();
        return item;
    }

    inline void StackPush(RnObject* object) {
        GetStack().push_back(object);
        GetScope()->IncrementStackCount();
    }

    void CallStackPush(RnScope* scope);
    void CallStackPop();
    RnObject* CallFunction(RnFunction* func, const RnArrayNative& args);
    RnIntNative Run();
    RnIntNative ExecuteCodeFrame(RnCodeFrame* frame, RnScope* scope);
    void LoadInstructions(RnStdVector<RnInstruction*>& instructions);
    static RnVirtualMachine* GetInstance();
    RnObject* CreateObject(RnType::Type type);
    RnObject* CreateObject(RnStringNative data);
    RnObject* CreateObject(RnBoolNative data);
    RnObject* CreateObject(RnIntNative data);
    RnObject* CreateObject(RnFloatNative data);
    RnScope* CreateScope();
    static void BindCls(RnScope* scope, RnObject* binding);
    static void BindThis(RnScope* scope, RnObject* binding);

private:
    inline void ExecuteInstruction(bool& break_scope, size_t& index);
    void RegisterBuiltins();
    RnVirtualMachine();

private:
    static RnVirtualMachine* _instance;
    void Init();

protected:
    RnCodeFrame* _current_frame = nullptr;
    RnArrayNative _stack;
    RnStdVector<RnScope*> _scopes;
    RnStdVector<RnScope*> _call_stack;
    RnStdVector<RnInstruction*> _instructions;
    RnMemoryManager* _memory_manager;
    size_t i_idx = 0;
    size_t _gc_count = 0;
    bool _should_exit = false;
    static RnIntNative _object_this_key;
    static RnIntNative _object_cls_key;
    static RnIntNative _object_construct_key;
};
