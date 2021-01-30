#pragma once
#include "allocator.h"

namespace memory {

class FreeListAllocator : public MemoryAllocator
{
protected:
	void* MemAlloc(uint32_t size, uint32_t alignment) override;
	void MemFree(void* ptr) override;
	
private:
	struct FreeList;
	AlignedMallocAllocator m_heap_allocator;
	FreeList* m_freelists;
};

} // namespace memory

