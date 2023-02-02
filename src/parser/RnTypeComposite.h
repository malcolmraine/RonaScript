/*****************************************************************************
* File: RnTypeComposite.h
* Description:
* Author: Malcolm Hall
* Date: 7/11/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNTYPECOMPOSITE_H
#define RONASCRIPT_RNTYPECOMPOSITE_H

#include <limits>
#include <string>
#include <utility>
#include "../vm/RnType.h"

template <typename T>
struct Bounds {
    T lower;
    T upper;
};

class RnTypeComposite {
public:
    explicit RnTypeComposite(const std::string& type);
    explicit RnTypeComposite(RnType::Type type);
    ~RnTypeComposite();
    [[nodiscard]] bool IsWithinRange(RnTypeComposite type) const;
    [[nodiscard]] Bounds<RnIntNative> GetIntBounds() const;
    Bounds<RnFloatNative> GetFloatBounds();
    void SetBounds(RnIntNative lower, RnIntNative upper);
    void SetBounds(RnFloatNative lower, RnFloatNative upper);
    std::string ToString();

    [[nodiscard]] RnType::Type GetType() const { return _type; }

private:
    Bounds<RnFloatNative> _bounds{};
    RnType::Type _type;
    std::string _str_type;
};

#endif  //RONASCRIPT_RNTYPECOMPOSITE_H