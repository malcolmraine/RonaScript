/*****************************************************************************
* File: RnMemoryManager.h
* Description:
* Author: Malcolm Hall
* Date: 5/20/22
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

#include <set>
#include <unordered_map>
#include "../common/RnType.h"
#include "RnMemoryGroup.h"

#define OBJECT_ALLOCATION_COUNT 1000000
#define GC_THRESHOLD (OBJECT_ALLOCATION_COUNT * 0.75)

class RnObject;
class RnScope;

class RnMemoryManager {
public:
    RnMemoryManager();
    ~RnMemoryManager();
    RnObject* CreateObject(RnType::Type type);
    RnObject* Create(RnStringNative data);
    RnObject* Create(RnBoolNative data);
    RnObject* Create(RnIntNative data);
    RnObject* Create(RnFloatNative data);
    RnScope* CreateScope();
    void DestroyScope(RnScope* scope);
    void GCMark();
    void GCSweep();
    void SetRootMemoryGroup(RnMemoryGroup* group);

private:
    void GCMarkMemoryGroup(RnMemoryGroup* memory_group);

private:
    RnMemoryGroup* root_memory_group;
};
