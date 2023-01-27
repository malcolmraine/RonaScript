/*****************************************************************************
* File: RnMemoryManager.cpp
* Description:
* Author: Malcolm Hall
* Date: 5/20/22
* Version: 1
*
******************************************************************************/

#include "RnMemoryManager.h"

#include <utility>
#include "RnBoolObject.h"
#include "RnFunctionObject.h"
#include "RnIntObject.h"
#include "RnStringObject.h"
#include "RnFloatObject.h"
#include "RnArrayObject.h"
#include "RnClassObject.h"
//#include <iostream>

/*****************************************************************************/
RnMemoryManager::RnMemoryManager() : root_memory_group(new RnMemoryGroup(nullptr))
{
	GrowHeap(DEFAULT_MAX_HEAP_SIZE);
	_block_size =
		std::max({ sizeof(RnArrayObject), sizeof(RnIntObject), sizeof(RnFunctionObject),
				   sizeof(RnFloatObject), sizeof(RnBoolObject),
				   sizeof(RnStringObject) });
	_allocation_size = _block_size * 100000;
	GrowAllocation(_allocation_size);
}

/*****************************************************************************/
RnMemoryManager::~RnMemoryManager()
{

}

/*****************************************************************************/
RnObject* RnMemoryManager::CreateObject(RnType::Type type)
{
	if (_available_addresses.empty())
	{
		GrowAllocation(_allocation_size);
	}

	auto address = _available_addresses.back();
	_available_addresses.pop_back();
	_used_addresses.push_back(address);

	switch (type)
	{
	case RnType::RN_BOOLEAN:
		return new(address) RnBoolObject();
	case RnType::RN_STRING:
		return new(address) RnStringObject();
	case RnType::RN_FLOAT:
		return new(address) RnFloatObject();
	case RnType::RN_INT:
		return new(address) RnIntObject();
	case RnType::RN_ARRAY:
		return new(address) RnArrayObject();
	case RnType::RN_FUNCTION:
		return new(address) RnFunctionObject();
	case RnType::RN_CLASS_INSTANCE:
	case RnType::RN_OBJECT:
		return new(address) RnClassObject();
	case RnType::RN_NULL:
	case RnType::RN_UNKNOWN:
		return nullptr;
	}
}

/*****************************************************************************/
RnObject* RnMemoryManager::Create(RnStringNative data)
{
	auto obj = CreateObject(RnType::RN_STRING);
	obj->SetData(std::move(data));
	return obj;
}

/*****************************************************************************/
RnObject* RnMemoryManager::Create(RnBoolNative data)
{
	auto obj = CreateObject(RnType::RN_BOOLEAN);
	obj->SetData(data);
	return obj;
}

/*****************************************************************************/
RnObject* RnMemoryManager::Create(RnIntNative data)
{
	auto obj = CreateObject(RnType::RN_INT);
	obj->SetData(data);
	return obj;
}

/*****************************************************************************/
RnObject* RnMemoryManager::Create(RnFloatNative data)
{
	auto obj = CreateObject(RnType::RN_FLOAT);
	obj->SetData(data);
	return obj;
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
		{
			block.UnsetMarkedFlag();
		}
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

/*****************************************************************************/
void RnMemoryManager::GrowAllocation(size_t size)
{
	char* block = (char*)malloc(size);
	if (block == nullptr) {
		throw std::runtime_error("Out of memory.");
	}
	_allocations.push_back(block);
	_available_addresses.reserve(_allocation_size / _block_size);
	_used_addresses.reserve(_allocation_size / _block_size);
	for (char* address = block; address < block + size; address += _block_size)
	{
		_available_addresses.push_back(address);
	}
}
