/*****************************************************************************
* File: RnScope.h
* Description:
* Author: Malcolm Hall
* Date: 6/20/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNSCOPE_H
#define RONASCRIPT_RNSCOPE_H

#include <map>
#include <variant>
#include <vector>
#include "../util/RnStack.h"
#include "RnMemoryGroup.h"
#include "RnScope.h"
#include "RnSymbolTable.h"

class RnBoolObject;
class RnIntObject;
class RnFloatObject;
class RnArrayObject;
class RnClassObject;
class RnStringObject;
class RnFunctionObject;
class RnAnyObject;

class RnScope {
public:
    explicit RnScope(RnScope* parent);
    virtual ~RnScope();
    void StoreObject(RnIntNative key, RnObject* value);
    [[nodiscard]] RnObject* GetObject(RnIntNative key);
    void RemoveObject(RnIntNative key);
    [[nodiscard]] RnSymbolTable* GetSymbolTable();
    [[nodiscard]] RnArrayNative& GetStack();
    void SetParent(RnScope* scope);
    [[nodiscard]] RnScope* GetParent() const;
    [[nodiscard]] RnMemoryGroup* GetMemoryGroup();

    static void LoadLibraryIntoScope(RnScope* scope, const std::string& library,
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

protected:
    RnScope* _parent = nullptr;
    RnArrayNative _stack;
    RnSymbolTable _symbolTable;
    RnMemoryGroup _memory_group;
    std::vector<
        std::variant<RnBoolObject, RnStringObject, RnIntObject, RnFloatObject,
                     RnFunctionObject, RnClassObject, RnArrayObject, RnAnyObject>>
        _locals;  // OP_CREATE_CONTEXT can pass a local count to reserve memory for this

private:
    int _linked_scope_count = 0;
    static std::map<std::string, void*> _handles;
    std::unordered_map<RnIntNative, RnClassObject*> _namespaces;
};

#endif  //RONASCRIPT_RNSCOPE_H