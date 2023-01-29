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
#include <memory>
//#include <iostream>

#define OBJECT_ALLOCATION_COUNT 10000

/*****************************************************************************/
RnMemoryManager::RnMemoryManager() : root_memory_group(new RnMemoryGroup(nullptr))
{
//	GrowHeap(DEFAULT_MAX_HEAP_SIZE);
	_block_size =
		std::max({ sizeof(RnArrayObject), sizeof(RnIntObject), sizeof(RnFunctionObject),
				   sizeof(RnFloatObject), sizeof(RnBoolObject),
				   sizeof(RnStringObject) });
	_allocation_size = _block_size * OBJECT_ALLOCATION_COUNT;
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
		if (!block->IsMarked())
		{
			sweep_count++;
			_available_addresses.push_back(block);
			std::destroy_at(block);
		}
		else
		{
			block->Unmark();
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
	for (auto& block : memory_group->GetObjects())
	{
		block->Mark();
	}

	for (auto& group : memory_group->GetChildGroups())
	{
		GCMarkMemoryGroup(group);
	}
}

/*****************************************************************************/
void RnMemoryManager::GrowAllocation(size_t size)
{
	auto* block = (char*)malloc(size);
	if (block == nullptr) {
		throw std::runtime_error("Out of memory.");
	}
	_allocations.push_back(block);
	_available_addresses.reserve(_allocation_size / _block_size);
	_used_addresses.reserve(_allocation_size / _block_size);
	for (auto address = block; address < block + size; address += _block_size)
	{
		_available_addresses.push_back((RnObject*)(address));
	}
}
