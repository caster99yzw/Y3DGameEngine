#pragma once
#include <cstring>
#include "allocator.h"

namespace memory {

constexpr MemSize NaturalChunkSize = 1024;
struct FastFreeList;

class FreeListAllocator : public MemoryAllocator
{
public:
	FreeListAllocator()
		: m_freelists(nullptr)
		, m_default_chunk_size(NaturalChunkSize) {}

	explicit FreeListAllocator(MemSize chunk_size)
		: m_freelists(nullptr)
		, m_default_chunk_size(chunk_size) {}

protected:
	void* MemAlloc(MemSize size, uint32_t alignment) override;
	void MemFree(void* ptr) override;

private:
	AlignedMallocAllocator m_heap_allocator;
	FastFreeList* m_freelists;
	MemSize m_default_chunk_size;
};

class FreeListBucketAllocator : public MemoryAllocator
{
public:
	FreeListBucketAllocator();
	explicit FreeListBucketAllocator(MemSize chunk_size);

protected:
	void* MemAlloc(MemSize size, uint32_t alignment) override;
	void MemFree(void* ptr) override;

private:
	constexpr static uint16_t BucketCount = 8;
	AlignedMallocAllocator m_heap_allocator;
	FastFreeList* m_freelist_buckets[BucketCount];
	MemSize m_default_chunk_size;
};

} // namespace memory

