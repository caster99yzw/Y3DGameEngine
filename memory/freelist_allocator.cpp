#include "freelist_allocator.h"

namespace memory {

struct ContinuousFreeList
{
	ContinuousFreeList()
		: m_entry_count(0)
		, m_active_entry_count(0)
		, m_allocator(nullptr)
		, m_chunks(nullptr) {}
	
	struct FreeListChunk
	{
		FreeListChunk* m_next;
		uint8_t** free_entries;
		uint32_t free_entries_count;
		uint8_t* buffer;
		uint8_t* buffer_end;
	};

	static ContinuousFreeList* CreateFreeList(Resource block, Format entry_format,
		uint32_t entry_count,
		MemoryAllocator* allocator);

	FreeListChunk* AddChunk();
	void* AllocateEntry();
	void DeallocateEntry(void* ptr) const;
	bool Contains(void* ptr) const;

	Format m_entry_format;
	uint32_t m_entry_count;
	Format m_chunk_format;
	uint32_t m_active_entry_count;
	MemoryAllocator* m_allocator;
	FreeListChunk* m_chunks;
};

ContinuousFreeList* ContinuousFreeList::CreateFreeList(Resource block, Format entry_format,
	uint32_t entry_count,
	MemoryAllocator* allocator)
{
	auto* result = reinterpret_cast<ContinuousFreeList*>(block.Data());
	result->m_entry_format = entry_format;
	result->m_entry_count = entry_count;
	result->m_chunk_format = Format(sizeof(FreeListChunk), alignof(FreeListChunk));;
	result->m_chunk_format += Format(
		entry_count * sizeof(uint8_t*), alignof(uint8_t*));
	result->m_chunk_format += Format(
		entry_count * entry_format.AlignedSize(), entry_format.Alignment());
	result->m_active_entry_count = 0;
	result->m_chunks = 0;
	block.Increment(sizeof(ContinuousFreeList));
	result->m_allocator = allocator;
	return result;
}

ContinuousFreeList::FreeListChunk* ContinuousFreeList::AddChunk()
{
	auto* new_chunk = m_allocator->Alloc<FreeListChunk>(1);
	Resource block(reinterpret_cast<uint8_t*>(new_chunk), m_chunk_format.Size());
	block.Increment(sizeof(FreeListChunk));

	const Format entry_fmt(m_entry_count * sizeof(uint8_t*), alignof(uint8_t*));
	block.Align(entry_fmt.Alignment());
	new_chunk->free_entries = reinterpret_cast<uint8_t**>(block.Data());
	new_chunk->free_entries_count = m_entry_count;
	block.Increment(entry_fmt.Size());

	block.Align(m_entry_format.Alignment());
	new_chunk->buffer = block.Data();
	new_chunk->buffer_end = block.Data() + m_entry_format.AlignedSize() * m_entry_count;
	new_chunk->m_next = nullptr;

	for (uint32_t i = 0; i < m_entry_count; ++i)
		new_chunk->free_entries[i] = new_chunk->buffer + m_entry_format.AlignedSize() * i;

	if (!m_chunks)
	{
		m_chunks = new_chunk;
	}
	else
	{
		auto* chunk_end = m_chunks;
		while (chunk_end->m_next != nullptr)
			chunk_end = chunk_end->m_next;
		chunk_end->m_next = new_chunk;
	}

	 m_active_entry_count += m_entry_count;
	 return new_chunk;
}

void* ContinuousFreeList::AllocateEntry()
{
	auto* chunk = m_chunks;
	while (chunk)
	{
		if (chunk->free_entries_count != 0)
		{
			uint32_t lc = chunk->free_entries_count--;
			return chunk->free_entries[lc - 1];
		}
		chunk = chunk->m_next;
	}

	chunk = AddChunk();
	assert(chunk);
	uint32_t lc = chunk->free_entries_count--;
	return chunk->free_entries[lc - 1];
}

void ContinuousFreeList::DeallocateEntry(void* ptr) const
{
	assert(m_chunks);
	auto* chunk = m_chunks;
	while (chunk)
	{
		if ((ptr >= chunk->buffer) && (ptr < chunk->buffer_end))
		{
			// Yes it was. Deallocate it.
			assert(chunk->free_entries_count < m_entry_count);
			chunk->free_entries[chunk->free_entries_count] = static_cast<uint8_t*>(ptr);
			++chunk->free_entries_count;
			break;
		}
		else
		{
			chunk = chunk->m_next;
		}
	}
}

bool ContinuousFreeList::Contains(void* ptr) const
{
	auto* chunk = m_chunks;
	while (chunk)
	{
		if ((ptr >= chunk->buffer) && (ptr < chunk->buffer_end))
		{
			return true;
		}
		chunk = chunk->m_next;
	}
	return false;
}

struct FreeListAllocator::FreeList
{
	ContinuousFreeList* free_list;
	FreeList* next;
	
};

void* FreeListAllocator::MemAlloc(MemSize size, uint32_t alignment)
{
	Format need_to_alloc(size, alignment);
	
	FreeList** freelist = &m_freelists;
	while (freelist)
	{
		if ((*freelist)->free_list->m_entry_format == need_to_alloc)
		{
			void* ptr = (*freelist)->free_list->AllocateEntry();
			return ptr;
		}
		(*freelist) = (*freelist)->next;
	}

	uint32_t entry_count = m_default_chunk_size / need_to_alloc.AlignedSize();
	if (entry_count == 0)
		entry_count = 1;

	ContinuousFreeList* new_free_list = ContinuousFreeList::CreateFreeList(
		need_to_alloc, entry_count, &m_heap_allocator
	);

	FreeList* newlink;
	newlink->next = nullptr;
	newlink->free_list = new_free_list;
	(*freelist) = newlink;
	
	void* ptr = new_free_list->AllocateEntry();
	return ptr;
}

void FreeListAllocator::MemFree(void* ptr)
{
	FreeList* freelist = m_freelists;
	while (freelist)
	{
		if (freelist->free_list->Contains(ptr))
		{
			freelist->free_list->DeallocateEntry(ptr);
			return;
		}
		freelist = freelist->next;
	}

}



} // namespace memory
