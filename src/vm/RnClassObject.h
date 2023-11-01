/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
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

#include "RnClass.h"
#include "RnObjectBase.h"

/**
 * class objects are object with a scope data type
 * class definitions have a scope but are static in duration and store the class function definitions and
 * -> binary expression gets the class object and the object for the second name from that objects scope.
 * 		functions will come from the parent scope, which will be the class definition scope and will be passed the l-value
 * 		object before being pushed to the stack
 *
 *
 * on class construction we create a new class object and copy the symbols from the declaration. The parent scope of the class object
 * is the declaration scope.
 * We load the class object scope onto the call stack and call the function "construct" on it
 *
 *
 * create a function that operates on a class object scope
 */

class RnScope;

// This is a class instance
class RnClassObject : public RnObjectBase<RnScope*> {
public:
    RnClassObject();
    explicit RnClassObject(RnScope* data);
    ~RnClassObject() override;
    [[nodiscard]] RnStringNative ToString() const override;
    void CopySymbols(RnScope* target);
    RnObject* CallFunction(RnIntNative key, RnArrayNative args);
    RnBoolNative HasSymbol(RnIntNative key);
    bool TryMagicMethod(RnIntNative key, RnArrayNative args, RnObject* ret_val);

    [[nodiscard]] RnScope* GetScope() const {
        return GetData();
    }

    [[nodiscard]] RnScope* ToObject() const override {
        return GetData();
    }

    void SetData(RnScope* data) override {
        SetDataInternal(data);
    }

    [[nodiscard]] RnType::Type GetType() const override {
        return RnType::RN_CLASS_INSTANCE;
    }

    [[nodiscard]] RnStringNative GetName() const {
        return _name;
    }

    static RnIntNative MAGIC_METHOD_KEY_STR;
    static RnIntNative MAGIC_METHOD_KEY_INT;
    static RnIntNative MAGIC_METHOD_KEY_FLOAT;
    static RnIntNative MAGIC_METHOD_KEY_ARRAY;
    static RnIntNative MAGIC_METHOD_KEY_BOOL;
private:
    RnStringNative _name;
};
