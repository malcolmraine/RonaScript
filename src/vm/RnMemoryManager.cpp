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
		return std::construct_at<RnBoolObject>(reinterpret_cast<RnBoolObject*>(address));
	case RnType::RN_STRING:
		return std::construct_at<RnStringObject>(reinterpret_cast<RnStringObject*>(address));
	case RnType::RN_FLOAT:
		return std::construct_at<RnFloatObject>(reinterpret_cast<RnFloatObject*>(address));
	case RnType::RN_INT:
		return std::construct_at<RnIntObject>(reinterpret_cast<RnIntObject*>(address));
	case RnType::RN_ARRAY:
		return std::construct_at<RnArrayObject>(reinterpret_cast<RnArrayObject*>(address));
	case RnType::RN_FUNCTION:
		return std::construct_at<RnFunctionObject>(reinterpret_cast<RnFunctionObject*>(address));
	case RnType::RN_CLASS_INSTANCE:
	case RnType::RN_OBJECT:
		return std::construct_at<RnClassObject>(reinterpret_cast<RnClassObject*>(address));
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
	for (auto address : _heap)
	{
		if (!address->IsMarked())
		{
			_available_addresses.push_back(address);
			_used_addresses.erase(std::find(_used_addresses.begin(),
				_used_addresses.end(),
				address));

			switch (address->GetType())
			{
			case RnType::RN_BOOLEAN:
				std::destroy_at<RnBoolObject>(reinterpret_cast<RnBoolObject*>(address));
				break;
			case RnType::RN_STRING:
				std::destroy_at<RnStringObject>(reinterpret_cast<RnStringObject*>(address));
				break;
			case RnType::RN_FLOAT:
				std::destroy_at<RnFloatObject>(reinterpret_cast<RnFloatObject*>(address));
				break;
			case RnType::RN_INT:
				std::destroy_at<RnIntObject>(reinterpret_cast<RnIntObject*>(address));
				break;
			case RnType::RN_ARRAY:
				std::destroy_at<RnArrayObject>(reinterpret_cast<RnArrayObject*>(address));
				break;
			case RnType::RN_FUNCTION:
				std::destroy_at<RnFunctionObject>(reinterpret_cast<RnFunctionObject*>(address));
				break;
			case RnType::RN_CLASS_INSTANCE:
			case RnType::RN_OBJECT:
				std::destroy_at<RnClassObject>(reinterpret_cast<RnClassObject*>(address));
				break;
			case RnType::RN_NULL:
			case RnType::RN_UNKNOWN:
				std::destroy_at<RnObject>(reinterpret_cast<RnObject*>(address));
				break;
			}
		}
		else
		{
			address->Unmark();
		}
	}
}

/*****************************************************************************/
void RnMemoryManager::SetRootMemoryGroup(RnMemoryGroup* group)
{
	root_memory_group = group;
}

/*****************************************************************************/
void RnMemoryManager::GCMarkMemoryGroup(RnMemoryGroup* memory_group)
{
	for (auto& obj : memory_group->GetObjects())
	{
		obj->Mark();
	}

	for (auto& group : memory_group->GetChildGroups())
	{
		GCMarkMemoryGroup(group);
	}
}

/*****************************************************************************/
void RnMemoryManager::GrowAllocation(size_t size)
{
	auto* block = static_cast<char*>(malloc(size));
	if (block == nullptr)
	{
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
