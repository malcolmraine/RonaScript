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

//	void AddObject(RnMemoryGroup* parent_group, RnObject* obj);
//	[[nodiscard]] size_t GetHeapCount() const;
//	[[nodiscard]] int GetHeapUsedCount() const;
//	size_t GrowHeap(size_t size);
//	RnMemoryBlock* FindFirstFree(bool start_at_last_block = true);
	void GCMark();
	void GCSweep();
	void SetRootMemoryGroup(RnMemoryGroup* group);

 private:
	void GCMarkMemoryGroup(RnMemoryGroup* memory_group);
	void GrowAllocation(size_t size);

 private:
	std::vector<RnObject*> _heap;
	std::vector<RnObject*> _allocations; // Larger blocks of memory
	size_t _block_size = 0;
	size_t _allocation_size = 0;
	std::vector<RnObject*> _available_addresses;
	std::vector<RnObject*> _used_addresses;
	size_t _used_heap_blocks = 0; // REMOVE
	size_t _heap_node_cnt = 0; // REMOVE
	size_t _last_free_index = 0; // REMOVE
//	size_t _max_heap_size = DEFAULT_MAX_HEAP_SIZE;

	RnMemoryGroup* root_memory_group;
};

#endif //RONASCRIPT_RNMEMORYMANAGER_H