#include "allocator.h"
#include <cstdlib>

namespace memory
{
	
MemoryAllocator::MemoryAllocator()
{
	m_total_internal.Reset();
	m_partial_internal.Reset();
}

void MemoryAllocator::StartReset()
{
	m_total_internal.Reset();
}

struct AllocationSizeInfo
{
	void* allocator;
	uint32_t aligned_offset;
};

void MemoryAllocator::LogAlloc(uint32_t size)
{
	m_total_internal.LogAlloc(size);
	m_partial_internal.LogAlloc(size);
}

void MemoryAllocator::LogFree(uint32_t size)
{
	m_total_internal.LogFree(size);
	m_partial_internal.LogFree(size);
}

void* AlignedMallocAllocator::MemAlloc(uint32_t size, uint32_t alignment)
{
	const auto info_size = sizeof(AllocationSizeInfo);
	auto* unaligned = static_cast<uint8_t*>(malloc(info_size + size + alignment));
	auto* aligned = reinterpret_cast<uint8_t*>(Impl::Align(unaligned + info_size, alignment));
	{
		AllocationSizeInfo* p = reinterpret_cast<AllocationSizeInfo*>(aligned) - 1;
		p->allocator = this;
		p->aligned_offset = static_cast<uint32_t>(aligned - unaligned);
	}
	return static_cast<void*>(aligned);
}

void AlignedMallocAllocator::MemFree(void* ptr)
{
	if (ptr)
	{
		AllocationSizeInfo* p = static_cast<AllocationSizeInfo*>(ptr) - 1;
		assert(this == p->allocator);
		p->allocator = nullptr;
		auto* unaligned = static_cast<uint8_t*>(ptr) - p->aligned_offset;
		free(unaligned);
		ptr = nullptr;
	}
}

} // namespace memory
