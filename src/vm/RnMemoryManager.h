/*****************************************************************************
* File: RnMemoryManager.h
* Description:
* Author: Malcolm Hall
* Date: 5/20/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNMEMORYMANAGER_H
#define RONASCRIPT_RNMEMORYMANAGER_H

#include "RnMemoryGroup.h"
#include "RnMemoryBlock.h"
#include <unordered_map>
#include <set>
#include "RnType.h"

#define DEFAULT_MAX_HEAP_SIZE (1024000)
#define DEFAULT_MIN_HEAP_SIZE (10)
#define DEFAULT_HEAP_GROW_SIZE (2000000)
#define OBJECT_CACHE_SIZE (1000)

class RnObject;

class RnMemoryManager
{
 public:
	RnMemoryManager();
	~RnMemoryManager();
	RnObject* CreateObject(RnType::Type type);
	RnObject* Create(RnStringNative data);
	RnObject* Create(RnBoolNative data);
	RnObject* Create(RnIntNative data);
	RnObject* Create(RnFloatNative data);
	RnScope* CreateScope();
	void DestroyScope(RnScope* scope);
	void GCMark();
	void GCSweep();
	void SetRootMemoryGroup(RnMemoryGroup* group);

 private:
	void GCMarkMemoryGroup(RnMemoryGroup* memory_group);
	void GrowAllocation(size_t size);

 private:
	std::vector<RnObject*> _heap;
	std::vector<char*> _allocations; // Larger blocks of memory
	std::vector<char*> _scope_allocations; // Larger blocks of memory

	size_t _block_size = 0;
	size_t _allocation_size = 0;
	std::vector<RnObject*> _available_addresses;
	std::vector<RnObject*> _used_addresses;

	std::vector<RnScope*> _available_scope_addresses;
	std::vector<RnScope*> _used_scope_addresses;
	RnMemoryGroup* root_memory_group;
	RnObject* _cached_bool_true_object = nullptr;
	RnObject* _cached_bool_false_object = nullptr;
	RnObject* _cached_int_object = nullptr;
};

#endif //RONASCRIPT_RNMEMORYMANAGER_H