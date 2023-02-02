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
#include "RnInternment.h"
#include "RnType.h"

class RnFunction;
class RnScope;

class RnObject {
public:
    virtual ~RnObject() = default;
    //	virtual void SetType(RnType::Type type) = 0;
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
    static size_t InternValue(RnFloatNative x);
    static size_t InternValue(const RnStringNative& x);
    static size_t InternValue(RnIntNative x);
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

    bool IsMarked() const {
        return _is_marked;
    }
    //	bool IsCached() const
    //	{
    //		return _is_cached;
    //	}
    //
    //	void SetIsCached(bool flag)
    //	{
    //		_is_cached = flag;
    //	}

public:
    //	static RnInternment<std::string>* _string_internment;
    //	static RnInternment<double>* _float_internment;
    //	static RnInternment<long>* _int_internment;
    static RnObject* null_object;

private:
    bool _is_marked = false;
    bool _is_cached = false;
};

#endif  //RONASCRIPT_RNOBJECT_H