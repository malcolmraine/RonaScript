/******************************************************************************
* File: RnFunction.h
* Description:
* Author: Malcolm Hall
* Date: 6/29/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNFUNCTION_H
#define RONASCRIPT_RNFUNCTION_H

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
    RnFunction(std::string name, long i_start, long i_cnt);
    ~RnFunction();
    void Reset();
    [[nodiscard]] std::string GetName() const;
    void SetName(const std::string& name);
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

    [[nodiscard]] RnType::Type GetReturnType() const {
        return _return_type;
    }

    void SetReturnType(RnType::Type type) {
        _return_type = type;
    }

private:
    std::unordered_map<size_t, RnIntNative> _argument_index_map;
    std::string _name;
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
    RnBuiltinFunction(const std::string& name, BuiltinFunction func);
    ~RnBuiltinFunction();
    [[nodiscard]] bool IsBuiltIn() const override;
    void Call(const RnArrayNative& args, RnObject* ret_val) override;

private:
    BuiltinFunction _function;
};

#endif  //RONASCRIPT_RNFUNCTION_H