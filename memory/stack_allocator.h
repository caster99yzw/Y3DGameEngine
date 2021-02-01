#pragma once
#include "allocator.h"

namespace memory {

constexpr uint32_t NaturalSlabSize = 32 * 1024;

class StackAllocator : public MemoryAllocator
{
public:
	StackAllocator()
		: m_internal_data(nullptr)
		, m_start_in_slab(nullptr)
		, m_default_slab_size(NaturalSlabSize) {}

	explicit StackAllocator(uint32_t slab_size)
		: m_internal_data(nullptr)
		, m_start_in_slab(nullptr)
		, m_default_slab_size(slab_size) {}

	StackAllocator(const StackAllocator&) = delete;
	StackAllocator& operator=(const StackAllocator&) = delete;
	StackAllocator(StackAllocator&&) = default;
	StackAllocator& operator=(StackAllocator&&) = default;
	~StackAllocator() override;

protected:
	void* MemAlloc(MemSize size, uint32_t alignment) override;
	void* MemAllocForLargeBlockOrSlab(MemSize size);
	void MemFree(void* ptr) override;

private:
	struct StackInternalData;
	StackInternalData* m_internal_data;
	uint8_t* m_start_in_slab;
	Format m_current_in_slab;
	AlignedMallocAllocator m_heap_allocator;
	uint32_t m_default_slab_size;
};

} // namespace memory
