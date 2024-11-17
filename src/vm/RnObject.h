/*****************************************************************************
* File: RnObject.h
* Description:
* Author: Malcolm Hall
* Date: 5/18/22
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
#include <variant>
#include "../common/RnInternment.h"
#include "../common/RnType.h"

class RnFunction;
class RnScope;

class RnObject {
public:
    virtual ~RnObject() = default;
    [[nodiscard]] virtual RnType::Type GetActiveType() const = 0;
    [[nodiscard]] virtual RnType::Type GetType() const = 0;
    [[nodiscard]] virtual std::vector<char> AsBytes() const = 0;
    virtual RnObject* operator+(RnObject* obj) = 0;
    virtual RnObject* operator-(RnObject* obj) = 0;
    virtual RnObject* operator==(RnObject* obj) = 0;
    virtual RnObject* operator!=(RnObject* obj) = 0;
    virtual RnObject* operator/(RnObject* obj) = 0;
    virtual RnObject* operator|(RnObject* obj) = 0;
    virtual RnObject* operator||(RnObject* obj) = 0;
    virtual RnObject* operator&&(RnObject* obj) = 0;
    virtual RnObject* operator&(RnObject* obj) = 0;
    virtual RnObject* operator^(RnObject* obj) = 0;
    virtual RnObject* operator>(RnObject* obj) = 0;
    virtual RnObject* operator<(RnObject* obj) = 0;
    virtual RnObject* operator>=(RnObject* obj) = 0;
    virtual RnObject* operator<=(RnObject* obj) = 0;
    virtual RnObject* operator%(RnObject* obj) = 0;
    virtual RnObject* operator>>(RnObject* obj) = 0;
    virtual RnObject* operator<<(RnObject* obj) = 0;
    virtual RnObject* operator*(RnObject* obj) = 0;
    [[nodiscard]] virtual RnIntNative ToInt() const = 0;
    [[nodiscard]] virtual RnFloatNative ToFloat() const = 0;
    [[nodiscard]] virtual RnStringNative ToString() const = 0;
    [[nodiscard]] virtual RnArrayNative ToArray() const = 0;
    [[nodiscard]] virtual RnFunction* ToFunction() const = 0;
    [[nodiscard]] virtual RnScope* ToObject() const = 0;
    [[nodiscard]] virtual RnBoolNative ToBool() const = 0;
    [[nodiscard]] virtual RnIntNative GetDataItemCount() const = 0;
    virtual void SetData(RnStringNative data) = 0;
    virtual void SetData(RnIntNative data) = 0;
    virtual void SetData(RnFloatNative data) = 0;
    virtual void SetData(RnBoolNative data) = 0;
    virtual void SetData(const RnArrayNative& data) = 0;
    virtual void SetData(RnObject* data) = 0;
    virtual void SetData(RnFunction* data) = 0;
    virtual void SetData(RnScope* data) = 0;
    virtual void CopyDataFromObject(RnObject* obj) = 0;
    virtual RnObject* At(RnIntNative index) = 0;
    static RnObject* Create(RnBoolNative data);
    static RnObject* Create(const RnStringNative& data);
    static RnObject* Create(RnIntNative data);
    static RnObject* Create(RnFloatNative data);
    static RnObject* Create(RnType::Type type);
    static RnObject* Copy(RnObject* obj);
    virtual void SetConstFlag(bool flag) = 0;
    static bool ValueCompare(const RnObject* a, const RnObject* b);
    [[nodiscard]] virtual size_t GetByteSize() const = 0;
    virtual size_t GetBytes(char* buf) = 0;
    virtual void SetBytes(const char* buf, size_t n) = 0;
    [[noreturn]] static void ThrowUndefinedOperatorError(const RnStringNative& op,
                                                         const RnObject* obj1,
                                                         const RnObject* obj2);

    /*************************************************************************/
    [[nodiscard]] virtual std::string GetTypeName() const {
        return RnType::TypeToString(GetType());
    }

    /*************************************************************************/
    virtual inline void Mark() {
        _is_marked = true;
    }

    /*************************************************************************/
    virtual inline void UnMark() {
        _is_marked = false;
    }

    /*************************************************************************/
    [[nodiscard]] inline bool IsMarked() const {
        return _is_marked;
    }

    /*************************************************************************/
    [[nodiscard]] bool IsClass() const {
        return _is_class;
    }

    /*************************************************************************/
    void SetIsClass(bool flag) {
        _is_class = flag;
    }

public:
    static RnObject* null_object;

private:
    bool _is_marked = false;
    bool _is_class = false;
};
