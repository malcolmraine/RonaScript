/*****************************************************************************
* File: RnType.cpp
* Description:
* Author: Malcolm Hall
* Date: 7/11/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2020 - 2023 Malcolm Hall
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

#include "RnType.h"

/*****************************************************************************/
RnTypeComposite::RnTypeComposite(const std::string& type) {
    _type = RnType::StringToType(type);
    _bounds.lower = -std::numeric_limits<double>::infinity();
    _bounds.upper = std::numeric_limits<double>::infinity();
}

/*****************************************************************************/
RnTypeComposite::RnTypeComposite(RnType::Type type) {
    _type = type;
    _bounds.lower = -std::numeric_limits<double>::infinity();
    _bounds.upper = std::numeric_limits<double>::infinity();
}

/*****************************************************************************/
RnTypeComposite::~RnTypeComposite() = default;

/*****************************************************************************/
[[nodiscard]] bool RnTypeComposite::IsWithinRange(RnTypeComposite type) const {
    auto other_bounds = type.GetFloatBounds();
    return _bounds.lower >= other_bounds.lower && _bounds.upper <= other_bounds.upper;
}

/*****************************************************************************/
Bounds<RnIntNative> RnTypeComposite::GetIntBounds() const {
    Bounds<RnIntNative> bounds{};
    bounds.lower = static_cast<RnIntNative>(_bounds.lower);
    bounds.upper = static_cast<RnIntNative>(_bounds.upper);

    return bounds;
}

/*****************************************************************************/
Bounds<RnFloatNative> RnTypeComposite::GetFloatBounds() {
    return _bounds;
}

/*****************************************************************************/
void RnTypeComposite::SetBounds(RnIntNative lower, RnIntNative upper) {
    _bounds.lower = static_cast<RnFloatNative>(lower);
    _bounds.upper = static_cast<RnFloatNative>(upper);
}

/*****************************************************************************/
void RnTypeComposite::SetBounds(RnFloatNative lower, RnFloatNative upper) {
    _bounds.lower = lower;
    _bounds.upper = upper;
}

/*****************************************************************************/
std::string RnTypeComposite::ToString() {
    if (_bounds.lower == -std::numeric_limits<double>::infinity() ||
        _bounds.upper == std::numeric_limits<double>::infinity()) {
        return RnType::TypeToString(_type);
    }

    switch (_type) {
        case RnType::RN_INT:
        case RnType::RN_STRING:
        case RnType::RN_ARRAY: {
            auto bounds = GetIntBounds();
            return RnType::TypeToString(_type) + "<" + std::to_string(bounds.lower) +
                   ".." + std::to_string(bounds.upper) + ">";
        }
        default: {
            return RnType::TypeToString(_type) + "<" + std::to_string(_bounds.lower) +
                   ".." + std::to_string(_bounds.upper) + ">";
        }
    }
}