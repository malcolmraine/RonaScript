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
    void RemoveChildGroup(RnMemoryGroup* group);

    void AddObject(RnObject* obj);
    void AddChildGroup(RnMemoryGroup* child);
    [[nodiscard]] RnMemoryGroup* GetParent() const;
    void SetParent(RnMemoryGroup* parent);

    [[nodiscard]] RnArrayNative& GetObjects() {
        return _objects;
    }

    [[nodiscard]] std::vector<RnMemoryGroup*>& GetChildGroups() {
        return _child_groups;
    }

private:
    RnMemoryGroup* _parent = nullptr;
    std::vector<RnMemoryGroup*> _child_groups;
    RnArrayNative _objects;
};

#endif  //RONASCRIPT_RNMEMORYGROUP_H