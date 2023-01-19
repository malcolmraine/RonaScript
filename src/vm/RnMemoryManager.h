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
	void AddObject(RnMemoryGroup* parent_group, RnObject* obj);
//	RnObject* MakeObject(RnMemoryGroup* parent_group,
//		RnType::Type type = RnType::RN_NULL);
	//RnMemoryGroup* MakeMemoryGroup(RnMemoryGroup* parent_group);

//	void EnableGC()
//	{
//		_gc_enabled = true;
//	}
//
//	void DisableGC()
//	{
//		_gc_enabled = false;
//	}
//
//	[[nodiscard]] bool GetGCEnabled() const
//	{
//		return _gc_enabled;
//	}

//	void SetMaxHeapSize(size_t size)
//	{
//		_max_heap_size = size;
//	}
//
//	[[nodiscard]] size_t GetMaxHeapSize() const
//	{
//		return _max_heap_size;
//	}

//	size_t GetHeapSizeMB();
	[[nodiscard]] size_t GetHeapCount() const;
	[[nodiscard]] int GetHeapUsedCount() const;
	size_t GrowHeap(size_t size);
//	size_t ShrinkHeap(size_t size);
	RnMemoryBlock* FindFirstFree(bool start_at_last_block =  true);
	void GCMark();
	void GCSweep();
	void SetRootMemoryGroup(RnMemoryGroup* group);

 private:
	void GCMarkMemoryGroup(RnMemoryGroup* memory_group);
//	void MoveToEnd(RnMemoryBlock* block);

	bool _gc_enabled = true;
	std::vector<RnMemoryBlock> _heap;
	std::unordered_map<RnType::Type, std::vector<RnObject*>> _object_cache;
//	RnMemoryBlock* _heap = nullptr;
//	RnMemoryBlock* _last_heap_node = nullptr;
	size_t _used_heap_blocks = 0;
	size_t _heap_node_cnt = 0;
	size_t _last_free_index = 0;
//	size_t _max_heap_size = DEFAULT_MAX_HEAP_SIZE;

	RnMemoryGroup* root_memory_group;
};

#endif //RONASCRIPT_RNMEMORYMANAGER_H