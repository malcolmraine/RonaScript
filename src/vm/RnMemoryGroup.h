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
#include "RnMemoryBlock.h"

class RnMemoryGroup
{
 public:
	explicit RnMemoryGroup(RnMemoryGroup* parent);
	~RnMemoryGroup();
//	RnMemoryBlock* GetBlockForObject(RnObject* obj);
	[[nodiscard]] std::vector<RnObject*> GetObjects() const;
	[[nodiscard]] std::vector<RnMemoryGroup*> GetChildGroups() const;
	void RemoveChildGroup(RnMemoryGroup* group);

	void AddChildBlock(RnObject* block);
	void AddChildGroup(RnMemoryGroup* child);
	[[nodiscard]] RnMemoryGroup* GetParent() const;
	void SetParent(RnMemoryGroup* parent);

 private:
	RnMemoryGroup* _parent;
	std::vector<RnMemoryGroup*> _child_groups;
	std::vector<RnObject*> _child_blocks;

};

#endif //RONASCRIPT_RNMEMORYGROUP_H