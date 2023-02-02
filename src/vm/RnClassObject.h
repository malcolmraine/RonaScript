/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_VM_RNCLASSOBJECT_H_
#define RONASCRIPT_VM_RNCLASSOBJECT_H_

#include "RnClass.h"
#include "RnObjectBase.h"
#include "RnVirtualMachine.h"

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
    RnClassObject() {
        _data = RnVirtualMachine::GetInstance()->CreateScope();
    }

    ~RnClassObject() {}

    [[nodiscard]] RnScope* GetScope() const {
        return GetData();
    }

    [[nodiscard]] RnScope* ToObject() const override {
        return GetData();
    }

    void SetData(RnScope* data) override {
        SetDataInternal(data);
    }

    [[nodiscard]] RnStringNative ToString() const override;

    [[nodiscard]] RnType::Type GetType() const override {
        return RnType::RN_OBJECT;
    }

    [[nodiscard]] std::string GetName() const {
        return _name;
    }

    void SetName(const std::string& name) {
        _name = name;
    }

    [[nodiscard]] bool IsModule() const {
        return _is_module;
    }

    void SetIsModule(bool flag) {
        _is_module = flag;
    }

    void CopySymbols(RnScope* target);

private:
    bool _is_module = false;
    std::string _name;
};

#endif  //RONASCRIPT_VM_RNCLASSOBJECT_H_