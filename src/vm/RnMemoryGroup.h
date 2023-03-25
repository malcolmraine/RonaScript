/*****************************************************************************
* File: RnMemoryGroup.h
* Description:
* Author: Malcolm Hall
* Date: 5/23/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNMEMORYGROUP_H
#define RONASCRIPT_RNMEMORYGROUP_H

#include <vector>
#include "../common/RnType.h"

class RnObject;

class RnMemoryGroup {
public:
    explicit RnMemoryGroup(RnMemoryGroup* parent);
    ~RnMemoryGroup();
    [[nodiscard]] RnArrayNative GetObjects() const;
    [[nodiscard]] std::vector<RnMemoryGroup*> GetChildGroups() const;
    void RemoveChildGroup(RnMemoryGroup* group);

    void AddObject(RnObject* obj);
    void AddChildGroup(RnMemoryGroup* child);
    [[nodiscard]] RnMemoryGroup* GetParent() const;
    void SetParent(RnMemoryGroup* parent);

private:
    RnMemoryGroup* _parent = nullptr;
    std::vector<RnMemoryGroup*> _child_groups;
    RnArrayNative _objects;
};

#endif  //RONASCRIPT_RNMEMORYGROUP_H