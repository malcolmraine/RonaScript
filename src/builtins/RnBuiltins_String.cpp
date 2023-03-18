/*****************************************************************************
* File: RnBuiltins_String.cpp
* Description:
* Author: Malcolm Hall
* Date: 2/1/22
* Version: 1
*
******************************************************************************/

#include "RnBuiltins_String.h"
#include "../util/String.h"
#include "../vm/RnObject.h"
#include "../vm/RnScope.h"

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_titlecase(RnScope* scope,
                                             const std::vector<RnObject*>& args,
                                             RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
}

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_lower(RnScope* scope,
                                         const std::vector<RnObject*>& args,
                                         RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
}

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_upper(RnScope* scope,
                                         const std::vector<RnObject*>& args,
                                         RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
}

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_snakecase(RnScope* scope,
                                             const std::vector<RnObject*>& args,
                                             RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
}

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_str_split(RnScope* scope,
                                             const std::vector<RnObject*>& args,
                                             RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
}

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_str_substr(RnScope* scope,
                                              const std::vector<RnObject*>& args,
                                              RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    assert(args.size() == 2 || args.size() == 3);
}

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_str_startwith(RnScope* scope,
                                                 const std::vector<RnObject*>& args,
                                                 RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    assert(args.size() == 2);

    ret_val->SetData(static_cast<bool>(
        String::StartsWith(args.front()->ToString(), args.back()->ToString())));
}

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_str_endswith(RnScope* scope,
                                                const std::vector<RnObject*>& args,
                                                RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    assert(args.size() == 2);

    ret_val->SetData(static_cast<bool>(
        String::EndsWith(args.front()->ToString(), args.back()->ToString())));
}

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_str_join(RnScope* scope,
                                            const std::vector<RnObject*>& args,
                                            RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    assert(args.size() == 2 || args.size() == 3);
}

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_str_merge(RnScope* scope,
                                             const std::vector<RnObject*>& args,
                                             RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    std::string result;
    for (auto arg : args) {
        result += arg->ToString();
    }
    ret_val->SetData(result);
}
