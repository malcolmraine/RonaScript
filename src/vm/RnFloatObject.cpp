/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
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
******************************************************************************/

#include "RnFloatObject.h"
#include <iomanip>
#include <sstream>

/*****************************************************************************/
RnFloatObject::RnFloatObject(RnFloatNative data) {
    _data.d_data = data;
}

/*****************************************************************************/
RnFloatObject::~RnFloatObject() = default;

/*****************************************************************************/
auto RnFloatObject::operator%(RnObject* obj) -> RnObject* {
    switch (obj->GetActiveType()) {
        case RnType::RN_INT:
        case RnType::RN_FLOAT:
            return RnObject::Create(std::fmod(ToFloat(), obj->ToFloat()));
        default:
            throw std::runtime_error("Operator '%' is not defined for types '" +
                                     RnType::TypeToString(GetType()) + "' and '" +
                                     RnType::TypeToString(obj->GetType()) + "'");
    }
}

/*****************************************************************************/
auto RnFloatObject::ToString() const -> RnStringNative {
    std::stringstream ss;
    ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
    ss << std::setprecision(std::numeric_limits<float>::digits10) << _data.d_data;
    std::string str = ss.str();
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    if (str.ends_with('.')) {
        str.append("0");
    }
    return str;
}