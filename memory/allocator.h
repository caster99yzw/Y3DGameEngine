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
		return static_cast<ElementType*>(
			MemAlloc(sizeof(ElementType) * element_count, alignof(ElementType)));
	}

	void* Alloc(MemSize size, uint32_t alignment) { return MemAlloc(size, alignment); }
	void Dealloc(void* ptr) { return MemFree(ptr); }

protected:
	virtual void* MemAlloc(MemSize size, uint32_t alignment) = 0;
	virtual void MemFree(void* ptr) = 0;
	void LogAlloc(MemSize size);
	void LogFree(MemSize size);

private:
	Stats m_total_internal;
	Stats m_partial_internal;
};

class AlignedMallocAllocator : public MemoryAllocator
{
protected:
	void* MemAlloc(MemSize size, uint32_t alignment) override;
	void MemFree(void* ptr) override;
	
};
	
} // namespace memory
