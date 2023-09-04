/*****************************************************************************
* File: RnMemoryGroup.h
* Description:
* Author: Malcolm Hall
* Date: 5/23/22
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

#pragma once

#include <vector>
#include "../common/RnType.h"

class RnObject;

class RnMemoryGroup {
public:
    explicit RnMemoryGroup(RnMemoryGroup* parent);
    ~RnMemoryGroup();
    void RemoveChildGroup(RnMemoryGroup* group);

    void AddObject(RnObject* obj);
    void AddChildGroup(RnMemoryGroup* child);
    [[nodiscard]] RnMemoryGroup* GetParent() const;
    void SetParent(RnMemoryGroup* parent);

    [[nodiscard]] RnArrayNative GetObjects() {
        return _objects;
    }

    [[nodiscard]] std::vector<RnMemoryGroup*> GetChildGroups() {
        return _child_groups;
    }

private:
    RnMemoryGroup* _parent = nullptr;
    std::vector<RnMemoryGroup*> _child_groups;
    RnArrayNative _objects;
};
