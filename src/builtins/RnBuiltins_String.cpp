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
void RnBuiltins_String::rn_builtin_str_titlecase(RnScope* scope,
                                             const RnArrayNative& args,
                                             RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    ret_val->SetData(String::TitleCase(args.front()->ToString()));
}

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_str_lower(RnScope* scope,
                                         const RnArrayNative& args,
                                         RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    ret_val->SetData(String::Lower(args.front()->ToString()));
}

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_str_upper(RnScope* scope,
                                         const RnArrayNative& args,
                                         RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    ret_val->SetData(String::Upper(args.front()->ToString()));
}

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_str_snakecase(RnScope* scope,
                                             const RnArrayNative& args,
                                             RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    ret_val->SetData(String::SnakeCase(args.front()->ToString()));
}

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_str_split(RnScope* scope,
                                             const RnArrayNative& args,
                                             RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    RnArrayNative result;

    if (args.size() == 1 || args.size() == 2 && args[1]->ToString().empty()) {
        auto original_string = args[0]->ToString();
        result.reserve(original_string.size());
        for (char c : original_string) {
            result.push_back(RnObject::Create(std::string(1, c)));
        }
    } else {
        auto strings = String::Split(args[0]->ToString(), args[1]->ToString());
        result.reserve(strings.size());
        for (const auto& s : strings) {
            result.push_back(RnObject::Create(s));
        }
    }
    ret_val->SetData(result);
}

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_str_substr(RnScope* scope,
                                              const RnArrayNative& args,
                                              RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    assert(args.size() == 2 || args.size() == 3);
    // original string, position, count
    if (args.size() == 2) {
        ret_val->SetData(args[0]->ToString().substr(args[1]->ToInt()));
    } else if (args.size() == 3) {
        ret_val->SetData(args[0]->ToString().substr(args[1]->ToInt(), args[2]->ToInt()));
    }
}

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_str_startswith(RnScope* scope,
                                                 const RnArrayNative& args,
                                                 RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    assert(args.size() == 2);

    // subject, search
    ret_val->SetData(static_cast<bool>(
        String::StartsWith(args.front()->ToString(), args.back()->ToString())));
}

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_str_endswith(RnScope* scope,
                                                const RnArrayNative& args,
                                                RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    assert(args.size() == 2);

    // subject, search
    ret_val->SetData(static_cast<bool>(
        String::EndsWith(args.front()->ToString(), args.back()->ToString())));
}

/*****************************************************************************/
void RnBuiltins_String::rn_builtin_str_join(RnScope* scope,
                                            const RnArrayNative& args,
                                            RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    assert(args.size() == 1 || args.size() == 2 || args.size() == 3);
    std::string result;
    auto strings = args[0]->ToArray();
    std::string join_str;

    if (args.size() == 2) {
        join_str = args[1]->ToString();
    }

    if (strings.empty()) {
        ret_val->SetData(std::string());
    } else if (strings.size() == 1) {
        ret_val->SetData(strings[0]->ToString());
    } else {
        for (size_t i = 0; i < strings.size() - 1; ++i) {
            result.append(strings[i]->ToString());
            result.append(join_str);
        }

        if (strings.size() > 1) {
            result.append(strings.back()->ToString());
        }
        ret_val->SetData(result);
    }
}
