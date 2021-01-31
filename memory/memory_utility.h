#pragma once
#include <cassert>
#include "common/Types.h"

namespace memory {

constexpr uint32_t NaturalTypeAlignment = 4;
using MemSize = uint64_t;

namespace Impl
{
FORCEINLINE uint64_t Align(uint64_t value, uint64_t alignment)
{
	assert(alignment != 0);
	return (value + (alignment - 1)) & ~(alignment - 1);
}
}

class Stats
{
public:
	void LogAlloc(MemSize size);
	void LogFree(MemSize size);
	void Reset();

	MemSize GetAllocBytes() const;
	uint32_t GetAllocCount() const;
	MemSize GetFreeBytes() const;
	uint32_t GetFreeCount() const;
	
private:
	MemSize m_allocated_bytes = 0;
	uint32_t m_allocated_count = 0;
	MemSize m_free_btyes = 0;
	uint32_t m_free_count = 0;
};
	
class Format
{
public:
	Format()
		: m_size(0)
		, m_alignment(NaturalTypeAlignment) {}

	Format(MemSize size, uint32_t align)
		: m_size(size)
		, m_alignment(align) {}

	void Set(MemSize size, uint32_t align);
	void Align();
	uint32_t Alignment() const { return m_alignment; }
	MemSize Size() const { return m_size; }
	MemSize AlignedSize() const;
	bool operator==(const Format& other) const;
	bool operator!=(const Format& other) const;
	Format& operator+=(const Format& other);
	Format operator+(const Format& other) const;
	Format& operator*=(uint32_t n);
	Format operator*(uint32_t n) const;
	Format& operator+=(MemSize size);
	Format operator+(MemSize size) const;
	Format& operator-=(MemSize size);
	Format operator-(MemSize size) const;

private:
	MemSize m_size;
	uint32_t m_alignment;
};

class Resource // freed block
{
public:
	Resource()
		: m_buffer(nullptr)
		, m_size(0) {}

	Resource(uint8_t* buffer, MemSize size)
		: m_buffer(buffer)
		, m_size(size) {}

	void Set(uint8_t* buffer, MemSize size);
	void Increment(MemSize size);
	void Decrement(MemSize size);
	uint8_t* Align(uint32_t align);
	uint8_t* AlignAndIncrement(const Format& align);
	bool Contains(const uint8_t* data) const;
	uint8_t* Data() const;
	MemSize FreeSize() const;

private:
	uint8_t* m_buffer;
	MemSize m_size;
};

} // namespace memory
