/*****************************************************************************
* File: RnVirtualMachine.h
* Description:
* Author: Malcolm Hall
* Date: 6/20/22
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
******************************************************************************/

#pragma once

#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>
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
        return _scopes.back()->GetStack();
    }

    inline RnObject* StackPop() {
        auto item = GetStack().back();
        GetStack().pop_back();
        assert(item);
        return item;
    }

    inline void StackPush(RnObject* object) {
        _scopes.back()->GetStack().push_back(object);
    }

    void CallStackPush(RnScope* scope);
    void CallStackPop();
    RnObject* CallFunction(RnFunction* func, RnArrayNative args);
    RnIntNative Run();
    void LoadInstructions(std::vector<RnInstruction*> instructions);
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
    std::vector<RnScope*> _scopes;
    std::vector<RnScope*> _call_stack;
    std::vector<RnInstruction*> _instructions;
    RnMemoryManager* _memory_manager;
    size_t i_idx = 0;
    size_t _gc_count = 0;
    static RnIntNative _object_this_key;
    static RnIntNative _object_cls_key;
    static RnIntNative _object_construct_key;
};
