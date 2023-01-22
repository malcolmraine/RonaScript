/*****************************************************************************
* File: RnMemoryManager.cpp
* Description:
* Author: Malcolm Hall
* Date: 5/20/22
* Version: 1
*
******************************************************************************/

#include "RnMemoryManager.h"
#include <iostream>

/*****************************************************************************/
RnMemoryManager::RnMemoryManager() : root_memory_group(new RnMemoryGroup(nullptr))
{
	_object_cache.reserve(OBJECT_CACHE_SIZE);

	for (size_t i = 0; i < OBJECT_CACHE_SIZE; i++)
	{
		auto obj = RnObject::Create(RnType::RN_INT);
		obj->SetIsCached(true);
		_object_cache[RnType::RN_INT].push_back(obj);
	}

	for (size_t i = 0; i < OBJECT_CACHE_SIZE; i++)
	{
		auto obj = RnObject::Create(RnType::RN_FLOAT);
		obj->SetIsCached(true);
		_object_cache[RnType::RN_FLOAT].push_back(obj);
	}

	GrowHeap(DEFAULT_MAX_HEAP_SIZE);
}

/*****************************************************************************/
RnMemoryManager::~RnMemoryManager()
{

}

/*****************************************************************************/
RnObject* RnMemoryManager::CreateObject(RnType::Type type)
{
	if (_object_cache.contains(type))
	{
		auto cache = _object_cache[type];
		if (!cache.empty())
		{
			return RnObject::Create(type);
		}
		auto obj = cache.back();
		cache.pop_back();
		return obj;
	}
	return RnObject::Create(type);
}

/*****************************************************************************/
void RnMemoryManager::AddObject(RnMemoryGroup* parent_group, RnObject* obj)
{
	auto block = FindFirstFree();
	if (!block)
	{
		_last_free_index = _heap_node_cnt - 1;
		GrowHeap(DEFAULT_HEAP_GROW_SIZE);
		block = FindFirstFree();
	}

	block->SetStoredObject(obj);
	parent_group->AddChildBlock(block);
	_used_heap_blocks++;
}

/*****************************************************************************/
size_t RnMemoryManager::GetHeapCount() const
{
	return _heap_node_cnt;
}

/*****************************************************************************/
int RnMemoryManager::GetHeapUsedCount() const
{
	return _used_heap_blocks;
}

/*****************************************************************************/
size_t RnMemoryManager::GrowHeap(size_t size)
{
	_heap.resize(_heap.size() + size);
	_heap_node_cnt += size;

//	std::cout << "Growing heap by " << size << "   New heap size: " << _heap_node_cnt
//			  << std::endl;
	return _heap_node_cnt;
}

/*****************************************************************************/
RnMemoryBlock* RnMemoryManager::FindFirstFree(bool start_at_last_block)
{
	if (_last_free_index >= _heap.size())
	{
		_last_free_index = 0;
	}

	for (size_t i = (start_at_last_block ? _last_free_index : 0); i < _heap.size(); i++)
	{
		auto block = &_heap[i];
		if (block->GetStoredObject() == nullptr)
		{
			_last_free_index = i;
			return block;
		}
	}

	_last_free_index = _heap.size() - 1;
	return nullptr;
}

/*****************************************************************************/
void RnMemoryManager::GCMark()
{
	GCMarkMemoryGroup(root_memory_group);
}

/*****************************************************************************/
void RnMemoryManager::GCSweep()
{
//	std::cout << "Current heap usage: " << _used_heap_blocks << std::endl;
	size_t sweep_count = 0;
	for (auto block : _heap)
	{
		if (!block.IsMarked())
		{
			sweep_count++;
			_used_heap_blocks--;
			block.Reset();
		}
		else
			block.UnsetMarkedFlag();
	}

//	std::cout << "Cleaned up " << sweep_count << " blocks" << std::endl;
//	std::cout << "New heap usage: " << _used_heap_blocks << std::endl;
}

/*****************************************************************************/
void RnMemoryManager::SetRootMemoryGroup(RnMemoryGroup* group)
{
	root_memory_group = group;
}

/*****************************************************************************/
void RnMemoryManager::GCMarkMemoryGroup(RnMemoryGroup* memory_group)
{
	for (auto& block : memory_group->GetChildBlocks())
	{
		block->SetMarkedFlag();
	}

	for (auto& group : memory_group->GetChildGroups())
	{
		GCMarkMemoryGroup(group);
	}
}
