/*****************************************************************************
* File: RnMemoryGroup.cpp
* Description:
* Author: Malcolm Hall
* Date: 5/23/22
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

#include "RnMemoryGroup.h"
#include <algorithm>

/*****************************************************************************/
RnMemoryGroup::RnMemoryGroup(RnMemoryGroup* parent) {
    _parent = parent;
    if (parent) {
        _parent->AddChildGroup(this);
    }
    _objects.reserve(10);
}

/*****************************************************************************/
RnMemoryGroup::~RnMemoryGroup() = default;

/*****************************************************************************/
void RnMemoryGroup::RemoveChildGroup(RnMemoryGroup* group) {
    auto iter = std::find(_child_groups.begin(), _child_groups.end(), group);
    if (iter != _child_groups.end()) {
        _child_groups.erase(iter);
    }
}

/*****************************************************************************/
void RnMemoryGroup::AddObject(RnObject* obj) {
    _objects.emplace_back(obj);
}

/*****************************************************************************/
void RnMemoryGroup::AddChildGroup(RnMemoryGroup* child) {
    _child_groups.push_back(child);
}

/*****************************************************************************/
RnMemoryGroup* RnMemoryGroup::GetParent() const {
    return _parent;
}

/*****************************************************************************/
void RnMemoryGroup::SetParent(RnMemoryGroup* parent) {
    _parent = parent;
    if (parent)
        _parent->AddChildGroup(this);
}

/*****************************************************************************/
void RnMemoryGroup::Clear() {
    _child_groups.clear();
    _objects.clear();
}
