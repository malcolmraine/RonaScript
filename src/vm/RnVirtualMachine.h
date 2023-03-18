/*****************************************************************************
* File: RnVirtualMachine.h
* Description:
* Author: Malcolm Hall
* Date: 6/19/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNVIRTUALMACHINE_H
#define RONASCRIPT_RNVIRTUALMACHINE_H

#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>
#include "../codegen/RnInstruction.h"
#include "../util/RnStack.h"
#include "RnInternment.h"
#include "RnScope.h"

class RnObject;
class RnFunctionObject;
class RnClassObject;
class RnMemoryManager;

class RnVirtualMachine {
public:
    ~RnVirtualMachine();

    RnScope* GetScope() {
        return _scopes.back();
    }

    [[nodiscard]] PtrStack<RnObject*>& GetStack() {
        return _scopes.back()->GetStack();
    }

    RnObject* StackPop() {
        auto item = _scopes.back()->GetStack().Pop();
        assert(item);
        return item;
//        auto obj = _scopes.back()->GetStack().back();
//        _scopes.back()->GetStack().pop_back();
//        return obj;
    }

    void CallFunction(RnFunctionObject* obj, uint32_t arg_cnt);
    RnIntNative Run();
    void LoadInstructions(std::vector<RnInstruction*> instructions);
    static RnVirtualMachine* GetInstance();
    RnObject* CreateObject(RnType::Type type);
    RnObject* CreateObject(RnStringNative data);
    RnObject* CreateObject(RnBoolNative data);
    RnObject* CreateObject(RnIntNative data);
    RnObject* CreateObject(RnFloatNative data);
    RnScope* CreateScope();

private:
    inline void ExecuteInstruction(bool& break_scope, size_t& index);
    void RegisterBuiltins();
    RnVirtualMachine();
    static void BindCls(RnScope* scope, RnObject* binding);
    static void BindThis(RnScope* scope, RnObject* binding);

private:
    static RnVirtualMachine* _instance;
    void Init();

protected:
    RnInternment<std::string> _string_internment;
    RnInternment<double> _float_internment;
    RnInternment<long> _int_internment;
    std::vector<RnScope*> _scopes;
    std::vector<RnScope*> _call_stack;
    std::unordered_map<RnIntNative, RnClassObject*> _namespaces;
    std::vector<RnInstruction*> _instructions;
    RnMemoryManager* _memory_manager;
    size_t i_idx = 0;
    size_t _gc_count = 0;
    static RnIntNative _object_this_key;
    static RnIntNative _object_cls_key;
    static RnIntNative _object_construct_key;
};

#endif  //RONASCRIPT_RNVIRTUALMACHINE_H