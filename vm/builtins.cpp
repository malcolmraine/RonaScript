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


/******************************************************************************
 * @brief
 * @param scope
 * @param id
 * @param args
 */
void call_builtin_function(Scope *scope, RonaObject *id, const std::vector<RonaObject *> &args) {
    std::string func_id = id->to_string();

    if (func_id == "println") {
        rn_builtin_println(scope, args);
    } else if (func_id == "print") {
        rn_builtin_print(scope, args);
    }
}

/******************************************************************************
 * @brief
 * @param scope
 * @param args
 */
void rn_builtin_println(Scope *scope, std::vector<RonaObject *> args) {
    std::cout << args[0]->to_string() << std::endl;
}

/******************************************************************************
 * @brief
 * @param scope
 * @param args
 */
void rn_builtin_print(Scope *scope, std::vector<RonaObject *> args) {
    std::cout << args[0]->to_string();
}

/******************************************************************************
 * @brief
 * @param scope
 * @param args
 */
void rn_builtin_call(Scope *scope, std::vector<RonaObject *> args) {

}