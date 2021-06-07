/******************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Version: 1
*
* MIT License
*
* Copyright (c) 2021 Malcolm Hall
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*******************************************************************************/

#include "builtins.h"
#include "lib/array.h"
#include "lib/str.h"
#include "lib/identity.h"
#include "lib/cast.h"
#include "lib/file.h"
#include "lib/util.h"
#include "rona_null.h"

/******************************************************************************
 * @brief
 * @param scope
 * @param args
 */
RonaObject *rn_builtin_println(std::vector<RonaObject *> args) {
    std::string output;

    for (auto &arg : args) {
        output += arg->to_string();
    }
    output += "\n";
    output.erase(
            std::remove(output.begin(), output.end(), '\"'),
            output.end());
    std::cout << output;

    return new RonaObject(RN_TYPE_NULL);
}

/******************************************************************************
 * @brief
 * @param scope
 * @param args
 */
RonaObject *rn_builtin_print(std::vector<RonaObject *> args) {
    std::string output;

    for (auto &arg : args) {
        std::string sanitized_arg = arg->to_string();

        if (sanitized_arg[0] == '\"') {
            sanitized_arg.erase(0, 1);
        }

        if (sanitized_arg[sanitized_arg.length() - 1] == '\"') {
            sanitized_arg.erase(sanitized_arg.length() - 1, 1);
        }

        output += sanitized_arg;
    }
    std::cout << output;

    return new RonaObject(RN_TYPE_NULL);
}

/******************************************************************************
 * @brief
 * @param scope
 * @param args
 */
RonaObject *rn_builtin_call(std::vector<RonaObject *> args) {

}

/******************************************************************************
 * @brief
 * @param scope
 * @param args
 */
RonaObject *rn_builtin_exit(std::vector<RonaObject *> args) {

}

/******************************************************************************
 * @brief
 * @param scope
 * @param args
 */
RonaObject *rn_builtin_open(std::vector<RonaObject *> args) {

}

/******************************************************************************
 * @brief
 * @param scope
 * @param args
 */
RonaObject *rn_builtin_read(std::vector<RonaObject *> args) {

}

/******************************************************************************
 * @brief
 * @param scope
 * @param args
 */
RonaObject *rn_builtin_write(std::vector<RonaObject *> args) {

}

/******************************************************************************
 * @brief
 * @param scope
 * @param args
 */
RonaObject *rn_builtin_sum(std::vector<RonaObject *> args) {

}

/******************************************************************************
 * @brief
 * @param scope
 * @param args
 */
RonaObject *rn_builtin_pow(std::vector<RonaObject *> args) {

}

/******************************************************************************
 * @brief
 * @param scope
 * @param args
 */
RonaObject *rn_builtin_clear(std::vector<RonaObject *> args) {
    args[0]->data.clear();

    return new RonaObject(RN_TYPE_NULL);
}

/******************************************************************************
 * @brief
 * @param scope
 * @param args
 */
RonaObject *rn_builtin_count(std::vector<RonaObject *> args) {
    switch (args[0]->type()) {
        case RN_TYPE_ARRAY:
            return new RonaObject(static_cast<long>(args[0]->data.size()));
        case RN_TYPE_STRING:
            return new RonaObject(static_cast<long>(args[0]->to_string().length()));
        default:
            return new RonaObject(1L);
    }
}

// Sys methods
/******************************************************************************
 * @brief
 * @param scope
 * @param args
 */
RonaObject *rn_builtin_sys_exec(std::vector<RonaObject *> args) {

}

// Runtime methods
/******************************************************************************
 * @brief
 * @param scope
 * @param args
 */
RonaObject *rn_builtin_throw(std::vector<RonaObject *> args) {

}

BuiltinMethodManager::BuiltinMethodManager() {
    this->interface.insert("println", rn_builtin_println);
    this->interface.insert("print", rn_builtin_print);
    this->interface.insert("call", rn_builtin_call);
    this->interface.insert("is_string", rn_builtin_is_string);
    this->interface.insert("is_int", rn_builtin_is_int);
    this->interface.insert("is_bool", rn_builtin_is_bool);
    this->interface.insert("is_null", rn_builtin_is_null);
    this->interface.insert("is_array", rn_builtin_is_array);
    this->interface.insert("is_float", rn_builtin_is_float);
    this->interface.insert("is_callable", rn_builtin_is_callable);
    this->interface.insert("is_const", rn_builtin_is_const);
    this->interface.insert("is_var", rn_builtin_is_var);
    this->interface.insert("is_object", rn_builtin_is_object);
    this->interface.insert("to_string", rn_builtin_to_string);
    this->interface.insert("to_int", rn_builtin_to_int);
    this->interface.insert("to_bool", rn_builtin_to_bool);
    this->interface.insert("to_float", rn_builtin_to_float);
    this->interface.insert("array_merge", rn_builtin_array_merge);
    this->interface.insert("array_slice", rn_builtin_array_slice);
    this->interface.insert("array_intersect", rn_builtin_array_intersect);
    this->interface.insert("array_union", rn_builtin_array_union);
    this->interface.insert("array_add", rn_builtin_array_add);
    this->interface.insert("array_sub", rn_builtin_array_sub);
    this->interface.insert("array_find", rn_builtin_array_find);
    this->interface.insert("array_has", rn_builtin_array_has);
    this->interface.insert("array_push", rn_builtin_array_push);
    this->interface.insert("array_expand", rn_builtin_array_expand);
    this->interface.insert("array_splice", rn_builtin_array_splice);
    this->interface.insert("str_cmp", rn_builtin_str_cmp);
    this->interface.insert("str_cat", rn_builtin_str_cat);
    this->interface.insert("str_slice", rn_builtin_str_slice);
    this->interface.insert("str_splice", rn_builtin_str_splice);
    this->interface.insert("str_upper", rn_builtin_str_upper);
    this->interface.insert("str_lower", rn_builtin_str_lower);
    this->interface.insert("clear", rn_builtin_clear);
    this->interface.insert("count", rn_builtin_count);
    this->interface.insert("sys_exec", rn_builtin_sys_exec);
    this->interface.insert("throw", rn_builtin_throw);
    this->interface.insert("clone", rn_builtin_clone);

}

BuiltinMethodManager::~BuiltinMethodManager() {

}

bool BuiltinMethodManager::exists(RonaObject *id) {
    return this->interface.m1.find(id->to_string()) != this->interface.m1.end();
}

RonaObject * BuiltinMethodManager::call_builtin_function(RonaObject *id, const std::vector<RonaObject *> &args) {
    return this->interface.searchAndCall(id->to_string(), args);
}

//bool BuiltinMethodManager::builtin_has_retval(RonaObject *id) {
//    return this->interface.m1[id->to_string()].second;
////    return false;
//}
