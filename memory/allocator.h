#pragma once
#include "memory_utility.h"

	namespace memory {

class MemoryAllocator
{
public:
	MemoryAllocator();
	MemoryAllocator(MemoryAllocator&) = delete;
	MemoryAllocator& operator=(MemoryAllocator&) = delete;
	MemoryAllocator(MemoryAllocator&&) = default;
	MemoryAllocator& operator=(MemoryAllocator&&) = default;
	virtual ~MemoryAllocator() = default;

	void StartReset();

	template <typename ElementType>
	ElementType* Alloc(uint32_t element_count)
	{
		auto size = sizeof(ElementType) * element_count;
		alloc->LogAlloc(size);
		return static_cast<ElementType*>(
			alloc->MemAlloc(size, alignof(ElementType)));
	}

	template <typename ElementType>
	void Dealloc(ElementType *ptr, uint32_t element_count)
	{
		auto size = sizeof(ElementType) * element_count;
		alloc->LogFree(size);
		alloc->MemFree(ptr);
	}

protected:
	virtual void* MemAlloc(uint32_t size, uint32_t alignment) = 0;
	virtual void MemFree(void* ptr) = 0;
	void LogAlloc(uint32_t size);
	void LogFree(uint32_t size);

private:
	Stats m_total_internal;
	Stats m_partial_internal;
};

class AlignedMallocAllocator : public MemoryAllocator
{
protected:
	void* MemAlloc(uint32_t size, uint32_t alignment) override;
	void MemFree(void* ptr) override;
	
};
	
} // namespace memory
