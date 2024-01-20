/*****************************************************************************
* File: RnScope.h
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

#include <variant>
#include <vector>
#include "../util/RnStack.h"
#include "RnMemoryGroup.h"
#include "RnSymbolTable.h"

class RnBoolObject;
class RnIntObject;
class RnFloatObject;
class RnArrayObject;
class RnClassObject;
class RnStringObject;
class RnFunctionObject;
class RnAnyObject;
class RnMemoryGroup;

class RnScope {
public:
    explicit RnScope(RnScope* parent);
    virtual ~RnScope();
    void StoreObject(RnIntNative key, RnObject* value);
    [[nodiscard]] RnObject* GetObject(RnIntNative key);
    void RemoveObject(RnIntNative key);
    [[nodiscard]] RnSymbolTable* GetSymbolTable();
    void SetParent(RnScope* scope);
    [[nodiscard]] RnScope* GetParent() const;
    [[nodiscard]] RnMemoryGroup* GetMemoryGroup();

    static bool LoadLibraryIntoScope(RnScope* scope, const RnStringNative& library,
                                     bool add_data = false);
    RnObject* MakeLocal(RnType::Type type);
    void Reset();

    void IncrLinkedScopeCount() {
        _linked_scope_count++;
    }

    void DecrLinkedScopeCount() {
        _linked_scope_count--;
    }

    [[nodiscard]] int GetLinkedScopeCount() const {
        return _linked_scope_count;
    }

    void IncrementStackCount() {
        _stack_count++;
    }

    void DecrementStackCount() {
        _stack_count--;
    }

    [[nodiscard]] size_t GetStackCount() const {
        return _stack_count;
    }

    RnObject* ret_val = nullptr;

protected:
    size_t _stack_count = 0;
    RnScope* _parent = nullptr;
    RnSymbolTable _symbolTable;
    RnMemoryGroup _memory_group;
    std::vector<
        std::variant<RnBoolObject, RnStringObject, RnIntObject, RnFloatObject,
                     RnFunctionObject, RnClassObject, RnArrayObject, RnAnyObject>>
        _locals;  // OP_CREATE_CONTEXT can pass a local count to reserve memory for this

private:
    int _linked_scope_count = 0;
    static std::unordered_map<RnStringNative, void*> _handles;
};
