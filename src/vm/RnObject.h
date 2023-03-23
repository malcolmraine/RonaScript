/*****************************************************************************
* File: RnObject.h
* Description:
* Author: Malcolm Hall
* Date: 5/18/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNOBJECT_H
#define RONASCRIPT_RNOBJECT_H

#include <string>
#include <variant>
#include "../common/RnType.h"
#include "RnInternment.h"

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
    [[nodiscard]] virtual std::vector<RnObject*> ToArray() const = 0;
    [[nodiscard]] virtual RnFunction* ToFunction() const = 0;
    [[nodiscard]] virtual RnScope* ToObject() const = 0;
    [[nodiscard]] virtual RnBoolNative ToBool() const = 0;
    virtual void SetData(RnStringNative data) = 0;
    virtual void SetData(RnIntNative data) = 0;
    virtual void SetData(RnFloatNative data) = 0;
    virtual void SetData(RnBoolNative data) = 0;
    virtual void SetData(std::vector<RnObject*> data) = 0;
    virtual void SetData(RnObject* data) = 0;
    virtual void SetData(RnFunction* data) = 0;
    virtual void SetData(RnScope* data) = 0;
    virtual void CopyDataFromObject(RnObject* obj) = 0;
    static std::string GetInternedString(InternmentKey key);
    static double GetInternedFloat(InternmentKey key);
    static long GetInternedInt(InternmentKey key);
    static RnIntNative InternValue(RnFloatNative x);
    static RnIntNative InternValue(const RnStringNative& x);
    static RnIntNative InternValue(RnIntNative x);
    static RnObject* Create(RnBoolNative data);
    static RnObject* Create(RnStringNative data);
    static RnObject* Create(RnIntNative data);
    static RnObject* Create(RnFloatNative data);
    static RnObject* Create(RnType::Type type);
    static RnObject* Copy(RnObject* obj);
    virtual void SetConstFlag(bool flag) = 0;

    void Mark() {
        _is_marked = true;
    }

    void Unmark() {
        _is_marked = false;
    }

    [[nodiscard]] bool IsMarked() const {
        return _is_marked;
    }

    [[nodiscard]] bool IsModule() const {
        return _is_module;
    }

    void SetIsModule(bool flag) {
        _is_module = flag;
    }

    [[nodiscard]] bool IsClass() const {
        return _is_class;
    }

    void SetIsClass(bool flag) {
        _is_class = flag;
    }


public:
    static RnObject* null_object;

private:
    bool _is_marked = false;
    bool _is_module = false;
    bool _is_class = false;
};

#endif  //RONASCRIPT_RNOBJECT_H