/*****************************************************************************
* File: RnMetaDataObject.cpp
* Description:
* Author: Malcolm Hall
* Date: 11/22/24
* Version: 1
*
*****************************************************************************/

#include "RnPackedObject.h"
#include "RnArrayObject.h"
#include "RnStringObject.h"

RnPackedObject::RnPackedObject() = default;

RnPackedObject::~RnPackedObject() = default;

/*****************************************************************************/
void RnPackedObject::UnpackToStack(RnArrayNative& stack, bool reverse) const {
    RnType::Type active_type = _data->GetActiveType();
    if (active_type == RnType::RN_ARRAY || active_type == RnType::RN_STRING) {
        if (reverse) {
            for (RnIntNative i = GetDataItemCount() - 1; i >=0; --i) {
                stack.push_back(_data->At(i));
            }
        } else {
            for (RnIntNative i = 0; i < GetDataItemCount(); ++i) {
                stack.push_back(_data->At(i));
            }
        }
    } else {
        throw std::runtime_error("Cannot unpack object of type " +
                                 RnType::TypeToString(_data->GetActiveType()));
    }
}
