#pragma once
#include <cassert>
#include "common/Types.h"

namespace memory {

constexpr uint32_t NaturalTypeAlignment = 4;

namespace Impl
{
template <typename T>
FORCEINLINE T Align(T value, uint32_t alignment)
{
	assert(alignment != 0);
	return (T)(value + (alignment - 1)) & ~(alignment - 1);
}
}

class Stats
{
public:
	void LogAlloc(uint32_t size);
	void LogFree(uint32_t size);
	void Reset();

	uint32_t GetAllocBytes() const;
	uint32_t GetAllocCount() const;
	uint32_t GetFreeBytes() const;
	uint32_t GetFreeCount() const;
	
private:
	uint32_t m_allocated_bytes = 0;
	uint32_t m_allocated_count = 0;
	uint32_t m_free_btyes = 0;
	uint32_t m_free_count = 0;
};
	
class Format
{
public:
	Format()
		: m_size(0)
		, m_alignment(NaturalTypeAlignment) {}

	Format(uint32_t size, uint32_t align)
		: m_size(size)
		, m_alignment(align) {}

	void Set(uint32_t size, uint32_t align);
	void Align();
	uint32_t Alignment() const { return m_alignment; }
	uint32_t Size() const { return m_size; }
	bool operator==(const Format& other) const;
	bool operator!=(const Format& other) const;
	Format& operator+=(const Format& other);
	Format operator+(const Format& other) const;
	Format& operator*=(uint32_t n);
	Format operator*(uint32_t n) const;
	Format& operator+=(uint32_t size);
	Format operator+(uint32_t size) const;
	Format& operator-=(uint32_t size);
	Format operator-(uint32_t size) const;

private:
	uint32_t m_size;
	uint32_t m_alignment;
};

class Resource // freed block
{
public:
	Resource()
		: m_buffer(nullptr)
		, m_size(0) {}

	Resource(uint8_t* buffer, uint32_t size)
		: m_buffer(buffer)
		, m_size(size) {}

	void Set(uint8_t* buffer, uint32_t size);
	void Increment(uint32_t size);
	void Decrement(uint32_t size);
	uint8_t* Align(uint32_t align);
	uint8_t* AlignAndIncrement(const Format& align);
	bool Contains(const uint8_t* data) const;

private:
	uint8_t* m_buffer;
	uint32_t m_size;
};

} // namespace memory
