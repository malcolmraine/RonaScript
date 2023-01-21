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
RnMemoryGroup::RnMemoryGroup(RnMemoryGroup* parent)
{
	SetParent(parent);
	_child_blocks.reserve(10);
	_child_groups.reserve(10);
}

/*****************************************************************************/
RnMemoryGroup::~RnMemoryGroup()
{

}

///*****************************************************************************/
//RnMemoryBlock* RnMemoryGroup::GetBlockForObject(RnObject* obj)
//{
//	for (auto& block : _child_blocks)
//	{
//		if (block->GetStoredObject() == obj)
//		{
//			return block;
//		}
//	}
//	return nullptr;
//}

std::vector<RnMemoryBlock*> RnMemoryGroup::GetChildBlocks() const
{
	return _child_blocks;
}

/*****************************************************************************/
std::vector<RnMemoryGroup*> RnMemoryGroup::GetChildGroups() const
{
	return _child_groups;
}

/*****************************************************************************/
void RnMemoryGroup::RemoveChildGroup(RnMemoryGroup* group)
{
	auto iter = std::find(_child_groups.begin(), _child_groups.end(), group);
	if (iter != _child_groups.end())
	{
		_child_groups.erase(iter);
	}
}

/*****************************************************************************/
void RnMemoryGroup::AddChildBlock(RnMemoryBlock* block)
{
	_child_blocks.emplace_back(block);
}

/*****************************************************************************/
void RnMemoryGroup::AddChildGroup(RnMemoryGroup* child)
{
	_child_groups.push_back(child);
}

/*****************************************************************************/
RnMemoryGroup* RnMemoryGroup::GetParent() const
{
	return _parent;
}

/*****************************************************************************/
void RnMemoryGroup::SetParent(RnMemoryGroup* parent)
{
	_parent = parent;

	if (parent)
		_parent->AddChildGroup(this);
}
