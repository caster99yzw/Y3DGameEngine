#include "stack_allocator.h"
#include <list>
#include <vector>

namespace memory {

struct StackAllocator::StackInternalData
{
	StackInternalData()
		: CachedSlab(nullptr)
		, SlabCount(0)
		, EndInFreeList(nullptr)
		, HeapAllocator(nullptr) {}
	
	void Release()
	{
		if (CachedSlab)
		{
			HeapAllocator->Dealloc(CachedSlab);
			CachedSlab = nullptr;
		}

		for (auto* slab : SlabList)
		{
			HeapAllocator->Dealloc(slab);
			slab = nullptr;
		}

		FreeList.clear();
		EndInFreeList = nullptr;
	}
	
	struct FreeListNode
	{
		uint8_t* Start = nullptr;
		uint8_t* End = nullptr;
		size_t SlabIndex = -1;
	};

	std::vector<void*> SlabList;
	void* CachedSlab;
	uint32_t SlabCount;
	std::list<FreeListNode> FreeList;
	void* EndInFreeList;
	MemoryAllocator* HeapAllocator;
};
	
StackAllocator::~StackAllocator()
{
	m_internal_data->Release();
}

void* StackAllocator::MemAlloc(MemSize size, uint32_t alignment)
{
	const Format old_current = m_current_in_slab;
	const Format need_to_alloc(size, alignment);
	m_current_in_slab += 4;
	m_current_in_slab += need_to_alloc;

	assert(alignment <= 16);
	const auto real_size = m_current_in_slab.Size() - old_current.Size();
	if (real_size <= m_default_slab_size 
		&& m_current_in_slab.Size() < m_default_slab_size)
	{
		auto* temp = m_start_in_slab + old_current.Size();
		*reinterpret_cast<uint32_t*>(*temp) = static_cast<uint32_t>(real_size);
		return reinterpret_cast<void*>(temp);
	}

	return MemAllocForLargeBlockOrSlab(size);
}

void* StackAllocator::MemAllocForLargeBlockOrSlab(MemSize size)
{
	if (size > m_default_slab_size)
	{
		++m_internal_data->SlabCount;
		auto* block = m_heap_allocator.Alloc(size, 16);
		assert(block);
		return block;
	}

	if (m_internal_data->CachedSlab)
	{
		m_internal_data->CachedSlab = nullptr;
		return m_internal_data->CachedSlab;
	}

	++m_internal_data->SlabCount;
	auto* block = m_heap_allocator.Alloc(m_default_slab_size, 16);
	assert(block);

	if (!m_internal_data->SlabList.empty())
	{
		StackInternalData::FreeListNode node;
		node.Start = m_start_in_slab + m_current_in_slab.Size();
		node.End = m_start_in_slab + m_default_slab_size;
		node.SlabIndex = m_internal_data->SlabList.size() - 1;
		m_internal_data->EndInFreeList = node.End;
		m_internal_data->FreeList.emplace_back(node);
	}

	m_internal_data->SlabList.emplace_back(block);
	m_start_in_slab = static_cast<uint8_t*>(block);
	m_current_in_slab.Set(size, 16);

	auto* temp = m_start_in_slab + m_current_in_slab.Size();
	*reinterpret_cast<uint32_t*>(*temp) = static_cast<uint32_t>(size);
	return block;
}

void StackAllocator::MemFree(void* ptr)
{
	auto check_position_valid = [this](void* ptr)
	{
		auto node = m_internal_data->FreeList.rbegin();
		while (node != m_internal_data->FreeList.rend())
		{
			if (node->End <= static_cast<uint8_t*>(ptr))
				break;
			if (node->Start <= static_cast<uint8_t*>(ptr))
				return false;
			++node;
		}
		return true;
	};

	assert(check_position_valid(ptr));
	const auto real_size = *static_cast<uint32_t*>(ptr);
	uint8_t* current = m_start_in_slab + m_current_in_slab.Size();
	if (real_size < m_default_slab_size 
		&& static_cast<uint8_t*>(ptr) + real_size == current
		&& m_internal_data->EndInFreeList != ptr)
	{
		m_current_in_slab -= real_size;
	}
}

} // namespace memory
