/*****************************************************************************
* File: RnMemoryGroup.cpp
* Description:
* Author: Malcolm Hall
* Date: 5/23/22
* Version: 1
*
******************************************************************************/

#include "RnMemoryGroup.h"

/*****************************************************************************/
RnMemoryGroup::RnMemoryGroup(RnMemoryGroup* parent) {
    _parent = parent;
    if (parent) {
        _parent->AddChildGroup(this);
    }
    _objects.reserve(10);
}

/*****************************************************************************/
RnMemoryGroup::~RnMemoryGroup() {}

/*****************************************************************************/
std::vector<RnObject*> RnMemoryGroup::GetObjects() const {
    return _objects;
}

/*****************************************************************************/
std::vector<RnMemoryGroup*> RnMemoryGroup::GetChildGroups() const {
    return _child_groups;
}

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
