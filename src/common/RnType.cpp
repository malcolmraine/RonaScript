/*****************************************************************************
* File: RnType.cpp
* Description:
* Author: Malcolm Hall
* Date: 7/11/22
* Version: 1
*
******************************************************************************/

#include "RnType.h"

/*****************************************************************************/
RnTypeComposite::RnTypeComposite(const std::string& type) {
    _type = RnType::StringToType(type);
    _bounds.lower = std::numeric_limits<double>::infinity();
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