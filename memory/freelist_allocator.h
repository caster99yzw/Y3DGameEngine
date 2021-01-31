#pragma once
#include "allocator.h"

namespace memory {

class FreeListAllocator : public MemoryAllocator
{
protected:
	void* MemAlloc(MemSize size, uint32_t alignment) override;
	void MemFree(void* ptr) override;
	
private:
	struct FreeList;
	AlignedMallocAllocator m_heap_allocator;
	FreeList* m_freelists;
	uint32_t m_default_chunk_size;
};

} // namespace memory

