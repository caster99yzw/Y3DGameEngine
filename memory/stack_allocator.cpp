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
	m_start_in_slab = nullptr;
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
		auto node = m_internal_data->FreeList.begin();
		while (node != m_internal_data->FreeList.end())
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
		return;
	}

	if (real_size > m_default_slab_size)
	{
		--m_internal_data->SlabCount;
		m_heap_allocator.Dealloc(ptr);
	}
	else
	{
		if (static_cast<uint8_t*>(ptr) + real_size == current)
			MemFreeSlabAndBlock(ptr);
		else
			MemFreeNotInStackTop(ptr, real_size);
	}
}

void StackAllocator::MemFreeNotInStackTop(void* start, MemSize size) const
{
	assert(m_internal_data->FreeList.size() < 100);
	auto* start_free = static_cast<uint8_t*>(start);

	auto slab_index = m_internal_data->SlabList.size() - 1;
	for (; slab_index != static_cast<uint32_t>(-1); --slab_index)
	{
		auto* slab_start = static_cast<uint8_t*>(m_internal_data->SlabList[slab_index]);
		if ((start_free - slab_start) < m_default_slab_size)
		{
			break;
		}
	}

	assert(slab_index == static_cast<uint32_t>(-1));
	uint8_t* end_free = start_free + size;
	auto node = m_internal_data->FreeList.begin();
	while (node != m_internal_data->FreeList.end())
	{
		if (node->SlabIndex == slab_index)
		{
			if (start_free == node->End)
			{
				node->End = end_free;
				m_internal_data->EndInFreeList = m_internal_data->FreeList.front().End;
				return;
			}
			else if (end_free == node->Start)
			{
				node->Start = start_free;
				m_internal_data->EndInFreeList = m_internal_data->FreeList.front().End;
				return;
			}
			if (start_free < node->Start)
			{
				++node;
			}
			break;
		}
		if (node->SlabIndex < slab_index)
		{
			break;
		}
		++node;
	}

	StackInternalData::FreeListNode free{ start_free, end_free, slab_index };
	m_internal_data->FreeList.emplace(node, free);

	if (!m_internal_data->FreeList.empty())
		m_internal_data->EndInFreeList = m_internal_data->FreeList.front().End;

}

void StackAllocator::MemFreeSlabAndBlock(void* ptr)
{
	auto* current = static_cast<uint8_t*>(ptr);

	auto node = m_internal_data->FreeList.begin();
	while (node != m_internal_data->FreeList.end())
	{
		if (node->End == current)
		{
			current = node->Start;
			node = m_internal_data->FreeList.erase(node);
		}
		else
		{
			break;
		}
	}

	while (!m_internal_data->SlabList.empty()
		&& (current - static_cast<uint8_t*>(m_internal_data->SlabList.back()) > m_default_slab_size
			|| current == static_cast<uint8_t*>(m_internal_data->SlabList.back())))
	{
		if (m_internal_data->CachedSlab)
		{
			m_heap_allocator.Dealloc(m_internal_data->CachedSlab);
			--m_internal_data->SlabCount;
		}
		m_internal_data->CachedSlab = m_internal_data->SlabList.back();
		m_internal_data->SlabList.pop_back();
	}

	m_internal_data->EndInFreeList = !m_internal_data->FreeList.empty()
		? m_internal_data->FreeList.front().End
		: nullptr;

	m_start_in_slab = !m_internal_data->SlabList.empty()
		? static_cast<uint8_t*>(m_internal_data->SlabList.back())
		: nullptr;

	m_current_in_slab.Set(current - m_start_in_slab, 16);
}

} // namespace memory


