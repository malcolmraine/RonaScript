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

    [[nodiscard]] RnArrayNative& GetStack() {
        return _scopes.back()->GetStack();
    }

    RnObject* StackPop() {
        auto item = _scopes.back()->GetStack().back();
        _scopes.back()->GetStack().pop_back();
        assert(item);
        return item;
    }

    void StackPush(RnObject* object) {
        _scopes.back()->GetStack().push_back(object);
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

    size_t GetIndex() const {
        return i_idx;
    }

    RnInstruction* GetCurrentInstruction() {
        return _instructions[i_idx];
    }

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
    std::vector<RnScope*> _scopes;
    std::vector<RnScope*> _call_stack;
    std::vector<RnInstruction*> _instructions;
    RnMemoryManager* _memory_manager;
    size_t i_idx = 0;
    size_t _gc_count = 0;
    static RnIntNative _object_this_key;
    static RnIntNative _object_cls_key;
    static RnIntNative _object_construct_key;
};

#endif  //RONASCRIPT_RNVIRTUALMACHINE_H