/******************************************************************************
* File: RnFunction.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/29/22
* Version: 1
*
*******************************************************************************/

#include "RnFunction.h"
#include <utility>
#include "RnObject.h"
#include "RnScope.h"

/*****************************************************************************/
RnFunction::RnFunction(std::string name, long i_start, long i_cnt) {
    _name = std::move(name);
    _i_start = i_start;
    _i_cnt = i_cnt;
    _argument_scope = new RnScope(nullptr);
}

/*****************************************************************************/
RnFunction::~RnFunction() = default;

/*****************************************************************************/
std::string RnFunction::GetName() const {
    return _name;
}

/*****************************************************************************/
void RnFunction::SetName(const std::string& name) {
    _name = name;
}

/*****************************************************************************/
long RnFunction::GetIStart() const {
    return _i_start;
}

/*****************************************************************************/
long RnFunction::GetICnt() const {
    return _i_cnt;
}

/*****************************************************************************/
RnScope* RnFunction::GetScope() {
    return _scope;
}

/*****************************************************************************/
void RnFunction::SetScope(RnScope* scope) {
    _scope = scope;
}

/*****************************************************************************/
bool RnFunction::IsBuiltIn() const {
    return false;
}

/*****************************************************************************/
void RnFunction::Call(const std::vector<RnObject*>& args, RnObject* ret_val) {}

/*****************************************************************************/
void RnFunction::CreateArgument(RnIntNative key, RnType::Type type, size_t index) {
    auto obj = RnObject::Create(type);
    _argument_scope->StoreObject(key, obj);
    _argument_index_map[index] = key;
}

/*****************************************************************************/
void RnFunction::PassArguments(const std::vector<RnObject*>& args, RnScope* scope) {
    if (args.size() < _argument_index_map.size()) {
        throw std::runtime_error("Expected " +
                                 std::to_string(_argument_index_map.size()) +
                                 " arguments but was passed " +
                                 std::to_string(args.size()) + ": '" + GetName() + "'");
    } else if (args.size() > _argument_index_map.size()) {
        throw std::runtime_error("Too many arguments passed to function '" + GetName() +
                                 "'");
    } else {
        for (size_t i = 0; i < args.size(); i++) {
            scope->StoreObject(_argument_index_map[i], args[i]);
        }
    }
}

/*****************************************************************************/
void RnFunction::InitScope(RnScope* scope) {
    for (const auto& symbol : _argument_scope->GetSymbolTable()->GetSymbols()) {
        scope->StoreObject(
            symbol, RnObject::Create(_argument_scope->GetObject(symbol)->GetType()));
    }
}

/*****************************************************************************/
/*****************************************************************************/
RnBuiltinFunction::RnBuiltinFunction(const std::string& name, BuiltinFunction func)
    : RnFunction(name, -1, -1), _function(func) {}

/*****************************************************************************/
RnBuiltinFunction::~RnBuiltinFunction() = default;

/*****************************************************************************/
bool RnBuiltinFunction::IsBuiltIn() const {
    return true;
}

/*****************************************************************************/
void RnBuiltinFunction::Call(const std::vector<RnObject*>& args, RnObject* ret_val) {
    _function(GetScope(), args, ret_val);
}
