#pragma once
#include "allocator.h"

namespace memory {

constexpr MemSize NaturalChunkSize = 1024;


class FreeListAllocator : public MemoryAllocator
{
protected:
	FreeListAllocator()
		: m_freelists(nullptr)
		, m_default_chunk_size(NaturalChunkSize) {}

	explicit FreeListAllocator(MemSize chunk_size)
		: m_freelists(nullptr)
		, m_default_chunk_size(chunk_size) {}

	void* MemAlloc(MemSize size, uint32_t alignment) override;
	void MemFree(void* ptr) override;

private:
	struct FastFreeList;
	AlignedMallocAllocator m_heap_allocator;
	FastFreeList* m_freelists;
	MemSize m_default_chunk_size;
};

} // namespace memory

