/*****************************************************************************
* File: RnFunction.h
* Description:
* Author: Malcolm Hall
* Date: 3/22/23
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

#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include "../common/RnType.h"

class RnScope;
class RnObject;

/*****************************************************************************/
class RnFunction {
public:
    RnFunction(RnStringNative name, long i_start, long i_cnt);
    ~RnFunction();
    void Reset();
    [[nodiscard]] RnStringNative GetName() const;
    void SetName(const RnStringNative& name);
    [[nodiscard]] long GetIStart() const;
    [[nodiscard]] long GetICnt() const;
    [[nodiscard]] RnScope* GetScope();
    void SetScope(RnScope* scope);
    [[nodiscard]] virtual bool IsBuiltIn() const;
    virtual void Call(const RnArrayNative& args, RnObject* ret_val);
    void CreateArgument(RnIntNative key, RnType::Type type, size_t index);
    void PassArguments(const RnArrayNative& args, RnScope* scope);
    void InitScope(RnScope* scope);
    void Bind(RnIntNative this_key, RnObject* object);

    void SetIStart(long n) {
        _i_start = n;
    }

    void SetICnt(long n) {
        _i_cnt = n;
    }

    [[nodiscard]] RnType::Type GetReturnType() const {
        return _return_type;
    }

    void SetReturnType(RnType::Type type) {
        _return_type = type;
    }

private:
    std::unordered_map<size_t, RnIntNative> _argument_index_map;
    RnStringNative _name;
    long _i_start = -1;
    long _i_cnt = -1;
    RnScope* _scope = nullptr;
    RnScope* _argument_scope = nullptr;
    RnType::Type _return_type = RnType::RN_VOID;
};

/*****************************************************************************/
typedef void (*BuiltinFunction)(RnScope*, RnArrayNative, RnObject*);

auto CastToBuiltin = [](auto f) {
    return reinterpret_cast<BuiltinFunction>(f);
};

class RnBuiltinFunction : public RnFunction {
public:
    RnBuiltinFunction(const RnStringNative& name, BuiltinFunction func);
    ~RnBuiltinFunction();
    [[nodiscard]] bool IsBuiltIn() const override;
    void Call(const RnArrayNative& args, RnObject* ret_val) override;

private:
    BuiltinFunction _function;
};
