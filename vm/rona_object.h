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

class RonaClass;

class RonaFunction;


class RonaObject {
public:
    RonaObject();
    RonaObject(RonaType_t type);
    RonaObject(RonaClass *value);
    RonaObject(RonaFunction *value);
    RonaObject(long value);
    RonaObject(bool value);
    RonaObject(double value);
    void set(bool value);
    RonaObject(std::string value);
    ~RonaObject();
    [[nodiscard]] RonaType_t type() const;
    void set(long value);
    void set(double value);
    void set(std::string value);
    void set(RonaClass *value);
    void set(long offset, const RonaObject &value);
    std::string to_string();
    long to_int();
    RonaFunction *as_function();
    RonaObject *operator[](int idx) const;
    RonaObject *operator+(RonaObject *other) const;
    RonaObject *operator-(RonaObject *other) const;
    RonaObject *operator/(RonaObject *other) const;
    RonaObject *operator*(RonaObject *other) const;
    RonaObject *operator%(RonaObject *other) const;
    RonaObject *operator>(RonaObject *other) const;
    RonaObject *operator<(RonaObject *other) const;
    RonaObject *operator>=(RonaObject *other) const;
    RonaObject *operator<=(RonaObject *other) const;
    RonaObject *operator>>(RonaObject *other) const;
    RonaObject *operator<<(RonaObject *other) const;
    RonaObject *operator==(RonaObject *other);
    RonaObject *operator!=(RonaObject *other);
    RonaObject *operator|(RonaObject *other) const;
    RonaObject *operator&(RonaObject *other) const;
    RonaObject *operator^(RonaObject *other) const;
    RonaObject *operator&&(RonaObject *other) const;
    RonaObject *operator||(RonaObject *other) const;
    void free_reference_cnt();
    void add_reference_cnt();
    int get_reference_cnt();
    long address();

    std::vector<std::variant<std::string, long, double, bool, RonaObject *, RonaClass *, RonaFunction *>> data;

protected:
    RonaType_t _type = RONA_NULL;
    int _reference_cnt = 0;

};


#endif //RONASCRIPT_RONA_OBJECT_H
