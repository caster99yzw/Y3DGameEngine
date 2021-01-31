#include "freelist_allocator.h"

namespace memory {

struct ContinuousChunk
{
	ContinuousChunk* NextChunk = nullptr;
	uint8_t** Entries = nullptr;
	uint32_t FreeCount = 0;
	uint8_t* DataBegin = nullptr;
	uint8_t* DataEnd = nullptr;

	void Reset()
	{
		NextChunk = nullptr;
		Entries = nullptr;
		FreeCount = 0;
		DataBegin = nullptr;
		DataEnd = nullptr;
	}
};

struct FreeListAllocator::FastFreeList
{
	FastFreeList()
		: NextFreeList(nullptr)
		, EntryCount(0)
		, HeapAllocator(nullptr)
		, Chunks(nullptr)
		, EntryCountPerChunk(0)
		, CachedChunks(nullptr) {}
	
	Format GetChunkFormat() const;

	static FastFreeList* CreateFreeList(
		Format entry_format,
		uint32_t entry_count,
		MemoryAllocator* allocator);

	ContinuousChunk* AllocateChunk();
	void DeallocateChunk(ContinuousChunk* chunk);
	void* AllocateEntry();
	void DeallocateEntry(void* ptr);
	bool Contains(void* ptr) const;

	FastFreeList* NextFreeList;
	Format EntryFormat;
	uint32_t EntryCount;
	MemoryAllocator* HeapAllocator;
	ContinuousChunk* Chunks;
	Format ChunkFormat;
	uint32_t EntryCountPerChunk;
	// 避免在chunk边缘反复创建销毁
	ContinuousChunk* CachedChunks;
};

Format FreeListAllocator::FastFreeList::GetChunkFormat() const
{
	Format result = Format(sizeof(ContinuousChunk), alignof(ContinuousChunk));;
	result += Format(EntryCountPerChunk * sizeof(uint8_t*), alignof(uint8_t*));
	result += Format(EntryCountPerChunk * EntryFormat.AlignedSize(), EntryFormat.Alignment());
	return result;
}

FreeListAllocator::FastFreeList* FreeListAllocator::FastFreeList::CreateFreeList(
	Format entry_format,
	uint32_t entry_count,
	MemoryAllocator* allocator)
{
	auto* result = allocator->Alloc<FastFreeList>(1);
	result->NextFreeList = nullptr;
	result->EntryFormat = entry_format;
	result->EntryCount = 0;
	result->HeapAllocator = allocator;
	result->Chunks = nullptr;
	result->EntryCountPerChunk = entry_count;
	result->EntryFormat = result->GetChunkFormat();
	return result;
}

ContinuousChunk* FreeListAllocator::FastFreeList::AllocateChunk()
{
	ContinuousChunk* new_chunk = nullptr;
	if (CachedChunks)
	{
		new_chunk = CachedChunks;
		CachedChunks = nullptr;
		assert(new_chunk->NextChunk);
	}
	else
	{
		new_chunk = HeapAllocator->Alloc<ContinuousChunk>(1);
	}

	Resource block(reinterpret_cast<uint8_t*>(new_chunk), ChunkFormat.Size());
	block.Increment(sizeof(ContinuousChunk));
	block.Align(alignof(uint8_t*));

	new_chunk->Entries = reinterpret_cast<uint8_t**>(block.Data());
	new_chunk->FreeCount = EntryCountPerChunk;
	block.Increment(EntryCountPerChunk * sizeof(uint8_t*));
	block.Align(EntryFormat.Alignment());

	const auto entry_data_size = EntryCountPerChunk * EntryFormat.AlignedSize();
	new_chunk->DataBegin = block.Data();
	new_chunk->DataEnd = block.Data() + entry_data_size;
	new_chunk->NextChunk = nullptr;
	block.Increment(entry_data_size);
	assert(block.FreeSize() == 0);

	// 设置好chunk里所有的freelist入口
	for (uint32_t i = 0; i < EntryCountPerChunk; ++i)
		new_chunk->Entries[i] = 
			new_chunk->DataBegin + EntryFormat.AlignedSize() * i;

	if (Chunks)
	{
		auto* chunk_end = Chunks;
		while (chunk_end->NextChunk != nullptr)
			chunk_end = chunk_end->NextChunk;
		chunk_end->NextChunk = new_chunk;
	}
	else
	{
		Chunks = new_chunk;
	}

	 EntryCount += EntryCountPerChunk;
	 return new_chunk;
}

void FreeListAllocator::FastFreeList::DeallocateChunk(ContinuousChunk* chunk)
{
	assert(Chunks);
	if (CachedChunks)
	{
		HeapAllocator->Dealloc(chunk);
		CachedChunks = nullptr;
	}

	ContinuousChunk* chunk_end = Chunks;
	while (chunk_end->NextChunk != nullptr)
	{
		if (chunk_end == chunk)
		{
			chunk_end->NextChunk = chunk->NextChunk;
			CachedChunks = chunk;
			CachedChunks->Reset();
			break;
		}
		chunk_end = chunk_end->NextChunk;
	}
}

void* FreeListAllocator::FastFreeList::AllocateEntry()
{
	ContinuousChunk* chunk = Chunks;
	while (chunk)
	{
		if (chunk->FreeCount != 0)
		{
			const uint32_t current = --chunk->FreeCount;
			return chunk->Entries[current];
		}
		chunk = chunk->NextChunk;
	}

	chunk = AllocateChunk();
	assert(chunk);
	const uint32_t current = --chunk->FreeCount;
	return chunk->Entries[current];
}

void FreeListAllocator::FastFreeList::DeallocateEntry(void* ptr)
{
	assert(Chunks);
	ContinuousChunk* chunk_end = Chunks;
	while (chunk_end)
	{
		if ((ptr >= chunk_end->DataBegin) && (ptr < chunk_end->DataEnd))
		{
			// Yes it was. Deallocate it.
			assert(chunk_end->FreeCount < EntryCountPerChunk);
			chunk_end->Entries[chunk_end->FreeCount] = static_cast<uint8_t*>(ptr);
			++chunk_end->FreeCount;

			if (chunk_end->FreeCount == EntryCountPerChunk)
				DeallocateChunk(chunk_end);
			break;
		}
		chunk_end = chunk_end->NextChunk;
	}
}

bool FreeListAllocator::FastFreeList::Contains(void* ptr) const
{
	auto* chunk = Chunks;
	while (chunk)
	{
		if ((ptr >= chunk->DataBegin) && (ptr < chunk->DataEnd))
			return true;
		chunk = chunk->NextChunk;
	}
	return false;
}

void* FreeListAllocator::MemAlloc(MemSize size, uint32_t alignment)
{
	const Format need_to_alloc(size, alignment);
	
	FastFreeList** freelist = &m_freelists;
	while (freelist)
	{
		if ((*freelist)->EntryFormat == need_to_alloc)
		{
			void* ptr = (*freelist)->AllocateEntry();
			return ptr;
		}
		(*freelist) = (*freelist)->NextFreeList;
	}

	auto entry_count = m_default_chunk_size / need_to_alloc.AlignedSize();
	if (entry_count == 0) entry_count = 1;

	auto* new_free_list = FastFreeList::CreateFreeList(
		need_to_alloc, static_cast<uint32_t>(entry_count), &m_heap_allocator
	);

	void* ptr = new_free_list->AllocateEntry();
	assert(ptr);
	(*freelist)->NextFreeList = new_free_list;
	return ptr;
}

void FreeListAllocator::MemFree(void* ptr)
{
	FastFreeList* freelist = m_freelists;
	while (freelist)
	{
		if (freelist->Contains(ptr))
		{
			freelist->DeallocateEntry(ptr);
			return;
		}
		freelist = freelist->NextFreeList;
	}
}



} // namespace memory
