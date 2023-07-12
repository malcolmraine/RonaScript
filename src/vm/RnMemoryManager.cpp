/*****************************************************************************
* File: RnMemoryManager.cpp
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

#include "RnMemoryManager.h"
#include <algorithm>
#include <memory>
#include <utility>
#include "RnAnyObject.h"
#include "RnArrayObject.h"
#include "RnBoolObject.h"
#include "RnClassObject.h"
#include "RnFloatObject.h"
#include "RnFunctionObject.h"
#include "RnIntObject.h"
#include "RnNullObject.h"
#include "RnStringObject.h"

#include "../memory_mgmt/RnObjectAllocator.h"

// Need to figure out the best settings for heap and max sizes
RnObjectAllocator<RnBoolObject> bool_allocator(1000000, 1000000);
RnObjectAllocator<RnArrayObject> array_allocator(10000, 1000000);
RnObjectAllocator<RnClassObject> class_allocator(10000, 1000000);
RnObjectAllocator<RnAnyObject> any_allocator(10000, 1000000);
RnObjectAllocator<RnFloatObject> float_allocator(10000, 1000000);
RnObjectAllocator<RnFunctionObject> func_allocator(10000, 1000000);
RnObjectAllocator<RnIntObject> int_allocator(10000, 1000000);
RnObjectAllocator<RnStringObject> string_allocator(10000, 1000000);
RnObjectAllocator<RnNullObject> null_allocator(10000, 1000000);
RnObjectAllocator<RnScope> scope_allocator(10000, 1000000);


/*****************************************************************************/
RnMemoryManager::RnMemoryManager() : root_memory_group(new RnMemoryGroup(nullptr)) {
}

/*****************************************************************************/
RnMemoryManager::~RnMemoryManager() = default;

/*****************************************************************************/
RnObject* RnMemoryManager::CreateObject(RnType::Type type) {
    switch (type) {
        case RnType::RN_BOOLEAN:
            return bool_allocator.CreateObject();
        case RnType::RN_STRING:
            return string_allocator.CreateObject();
        case RnType::RN_FLOAT:
            return float_allocator.CreateObject();
        case RnType::RN_INT:
            return int_allocator.CreateObject();
        case RnType::RN_ANY:
            return any_allocator.CreateObject();
        case RnType::RN_ARRAY:
            return array_allocator.CreateObject();
        case RnType::RN_FUNCTION:
        case RnType::RN_CALLABLE:
            return func_allocator.CreateObject();
        case RnType::RN_CLASS_INSTANCE:
        case RnType::RN_OBJECT:
            return class_allocator.CreateObject();
        case RnType::RN_NULL:
        case RnType::RN_VOID:
            return null_allocator.CreateObject();
        case RnType::RN_UNKNOWN:
        default:
            assert(false);
            return nullptr;
    }
}

/*****************************************************************************/
RnObject* RnMemoryManager::Create(RnStringNative data) {
    auto obj = CreateObject(RnType::RN_STRING);
    obj->SetData(std::move(data));
    return obj;
}

/*****************************************************************************/
RnObject* RnMemoryManager::Create(RnBoolNative data) {
    auto obj = CreateObject(RnType::RN_BOOLEAN);
    obj->SetData(data);
    return obj;
}

/*****************************************************************************/
RnObject* RnMemoryManager::Create(RnIntNative data) {
    auto obj = CreateObject(RnType::RN_INT);
    obj->SetData(data);
    return obj;
}

/*****************************************************************************/
RnObject* RnMemoryManager::Create(RnFloatNative data) {
    auto obj = CreateObject(RnType::RN_FLOAT);
    obj->SetData(data);
    return obj;
}

/*****************************************************************************/
RnScope* RnMemoryManager::CreateScope() {
    return scope_allocator.CreateObject(nullptr);
}

/*****************************************************************************/
void RnMemoryManager::DestroyScope(RnScope* scope) {
    scope_allocator.FreeObject(scope);
}

/*****************************************************************************/
void RnMemoryManager::GCMark() {
    GCMarkMemoryGroup(root_memory_group);
}

/*****************************************************************************/
void RnMemoryManager::GCSweep() {
    bool_allocator.FreeIf([](auto object) {
        return !object->IsMarked();
    });

    int_allocator.FreeIf([](auto object) {
        return !object->IsMarked();
    });

    float_allocator.FreeIf([](auto object) {
        return !object->IsMarked();
    });

    string_allocator.FreeIf([](auto object) {
        return !object->IsMarked();
    });

    class_allocator.FreeIf([](auto object) {
        return !object->IsMarked();
    });

    func_allocator.FreeIf([](auto object) {
        return !object->IsMarked();
    });

    null_allocator.FreeIf([](auto object) {
        return !object->IsMarked();
    });

    any_allocator.FreeIf([](auto object) {
        return !object->IsMarked();
    });
}

/*****************************************************************************/
void RnMemoryManager::SetRootMemoryGroup(RnMemoryGroup* group) {
    root_memory_group = group;
}

/*****************************************************************************/
void RnMemoryManager::GCMarkMemoryGroup(RnMemoryGroup* memory_group) {
    if (!memory_group) {
        return;
    }

    for (auto& obj : memory_group->GetObjects()) {
        obj->Mark();
    }

    for (auto& group : memory_group->GetChildGroups()) {
        GCMarkMemoryGroup(group);
    }
}
