/*****************************************************************************
* File: RnObjectList.cpp
* Description:
* Author: Malcolm Hall
* Date: 11/9/24
* Version: 1
*
* MIT License
*
* Copyright (c) 2020 - 2024 Malcolm Hall
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

#pragma once

#include <initializer_list>
#include "../memory_mgmt/RnStdAllocator.h"

class RnObject;

/*****************************************************************************/
/*****************************************************************************/
class RnObjectList : public RnStdVector<RnObject*> {
public:
    RnObjectList() : RnStdVector<RnObject*>() {}

    RnObjectList(std::initializer_list<RnObject*> initializerList)
        : RnStdVector<RnObject*>(initializerList) {}

    RnObjectList(RnStdVector<RnObject*>::const_iterator first,
                 RnStdVector<RnObject*>::const_iterator last)
        : RnStdVector<RnObject*>(first, last) {}

    RnObjectList(size_t n, RnObject* value) : RnStdVector<RnObject*>(n, value) {}
};
