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

#ifndef RONASCRIPT_RONA_OBJECT_H
#define RONASCRIPT_RONA_OBJECT_H

#include <string>
#include <vector>
#include <variant>
#include <iomanip>
#include "rona_class.h"
#include "rona_type.h"
#include "../exceptions/TypeAssignmentError.h"
#include "../exceptions/TypeCastError.h"
#include "rona_function.h"
#include "rona_class_decl.h"

class RonaClass;
class RonaFunction;

class RonaObject {
public:
    RonaObject();
    explicit RonaObject(RonaType_t type);
    explicit RonaObject(RonaClass *value);
    explicit RonaObject(RonaFunction *value);
    explicit RonaObject(long value);
    explicit RonaObject(bool value);
    explicit RonaObject(double value);
    void set(bool value);
    explicit RonaObject(std::string value);
    ~RonaObject();
    [[nodiscard]] RonaType_t type();
    void set(long value);
    void set(double value);
    void set(std::string value);
    void set(RonaClass *value);
    void set(long offset, const RonaObject &value);
    std::string to_string(bool sanitize_strings = true);
    static std::string sanitize_string(std::string str);
    long to_int();
    bool to_bool();
    double to_float();
    RonaFunction *as_function();
    RonaObject *operator[](int idx);
    RonaObject *operator+(RonaObject *other);
    RonaObject *operator+();
    RonaObject *operator-(RonaObject *other);
    RonaObject *operator-();
    RonaObject *operator/(RonaObject *other);
    RonaObject *operator*(RonaObject *other);
    RonaObject *operator%(RonaObject *other);
    RonaObject *operator>(RonaObject *other);
    RonaObject *operator<(RonaObject *other);
    RonaObject *operator>=(RonaObject *other);
    RonaObject *operator<=(RonaObject *other);
    RonaObject *operator>>(RonaObject *other);
    RonaObject *operator<<(RonaObject *other);
    RonaObject *operator==(RonaObject *other);
    RonaObject *operator!=(RonaObject *other);
    RonaObject *operator|(RonaObject *other);
    RonaObject *operator&(RonaObject *other);
    RonaObject *operator^(RonaObject *other);
    RonaObject *operator&&(RonaObject *other);
    RonaObject *operator||(RonaObject *other);
    RonaObject *operator!();
    RonaObject *operator~();
    long address();
    RonaObject *clone();
    void set_type(RonaType_t type);

    std::vector<std::variant<std::monostate, std::string, long, double, bool, RonaObject *, RonaClass *, RonaFunction *>> data;
    Scope *parent = nullptr;
    bool is_const = false;
    std::string type_string = "null";
    int ref_cnt = 0;
    bool gc_flag = false;
//    ScopeVisibility visibility = PUBLIC_VISIBILITY;

protected:
    RonaType_t _type = RN_TYPE_NULL;
};


#endif //RONASCRIPT_RONA_OBJECT_H