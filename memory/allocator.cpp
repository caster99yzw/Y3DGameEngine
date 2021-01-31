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

void MemoryAllocator::LogAlloc(MemSize size)
{
	m_total_internal.LogAlloc(size);
	m_partial_internal.LogAlloc(size);
}

void MemoryAllocator::LogFree(MemSize size)
{
	m_total_internal.LogFree(size);
	m_partial_internal.LogFree(size);
}

namespace Impl
{
struct AllocationSizeInfo
{
	void* allocator;
	uint32_t size;
	uint32_t aligned_offset;
};
}

void* AlignedMallocAllocator::MemAlloc(MemSize size, uint32_t alignment)
{
	const auto info_size = sizeof(Impl::AllocationSizeInfo);
	const auto real_size = info_size + size + alignment;
	LogAlloc(real_size);
	auto* unaligned = static_cast<uint8_t*>(malloc(real_size));
	auto* aligned = reinterpret_cast<uint8_t*>(
		Impl::Align(reinterpret_cast<uint64_t>(unaligned) + info_size, alignment));
	{
		Impl::AllocationSizeInfo* p = reinterpret_cast<Impl::AllocationSizeInfo*>(aligned) - 1;
		p->allocator = this;
		p->size = static_cast<uint32_t>(real_size);
		p->aligned_offset = static_cast<uint32_t>(aligned - unaligned);
	}
	return static_cast<void*>(aligned);
}

void AlignedMallocAllocator::MemFree(void* ptr)
{
	if (ptr)
	{
		Impl::AllocationSizeInfo* p = static_cast<Impl::AllocationSizeInfo*>(ptr) - 1;
		assert(this == p->allocator);
		p->allocator = nullptr;
		auto* unaligned = static_cast<uint8_t*>(ptr) - p->aligned_offset;
		LogFree(p->size);
		free(unaligned);
		ptr = nullptr;
	}
}

} // namespace memory
