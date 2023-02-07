/*****************************************************************************
* File: RnBuiltins_Array.cpp
* Description:
* Author: Malcolm Hall
* Date: 5/30/22
* Version: 1
*
******************************************************************************/

#include "RnBuiltins_Array.h"
#include "../vm/RnArrayObject.h"
#include "../vm/RnScope.h"

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_array_filter(RnScope* scope, const std::vector<RnObject*>& args,
                                    RnObject* ret_val) {}

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_array_union(RnScope* scope, const std::vector<RnObject*>& args,
                                   RnObject* ret_val) {}

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_array_intersect(RnScope* scope,
                                       const std::vector<RnObject*>& args,
                                       RnObject* ret_val) {}

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_array_merge(RnScope* scope,
                                              const std::vector<RnObject*>& args,
                                              RnObject* ret_val) {
    std::vector<RnObject*> data;
    for (auto arg : args) {
        for (auto& item : arg->ToArray()) {
            data.emplace_back(item);
        }
    }
    ret_val->SetData(data);
}

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_count(RnScope* scope,
                                        const std::vector<RnObject*>& args,
                                        RnObject* ret_val) {
    auto obj = args.front();
    if (obj->GetType() == RnType::RN_STRING) {
        ret_val->SetData(static_cast<RnIntNative>(obj->ToString().length()));
    } else {
        ret_val->SetData(static_cast<RnIntNative>(obj->ToArray().size()));
    }
}

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_array_push(RnScope* scope,
                                             const std::vector<RnObject*>& args,
                                             RnObject* ret_val) {
    auto array_obj = dynamic_cast<RnArrayObject*>(args.front());
    for (size_t i = 1; i < args.size(); i++) {
        array_obj->Append(args.at(i));
    }
}

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_array_pop(RnScope* scope,
                                            const std::vector<RnObject*>& args,
                                            RnObject* ret_val) {}

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_array_zeros(RnScope* scope,
                                              const std::vector<RnObject*>& args,
                                              RnObject* ret_val) {
    assert(args.size() == 1);
    std::vector<RnObject*> data;
    data.reserve(args.front()->ToInt());
    for (RnIntNative i = 0; i < args.front()->ToInt(); i++) {
        // make objects and add to vector
    }
    ret_val->SetData(data);
}

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_array_fill(RnScope* scope,
                                             const std::vector<RnObject*>& args,
                                             RnObject* ret_val) {
    assert(args.size() == 2);
}

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_sequence(RnScope* scope,
                                           const std::vector<RnObject*>& args,
                                           RnObject* ret_val) {
    assert(args.size() > 2 && args.size() < 4);
    /**
	 * arg1: array
	 * arg2: count
	 * arg3: start
	 * arg4: step
	 */
}
