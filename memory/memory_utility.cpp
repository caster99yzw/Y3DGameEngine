#include "memory_utility.h"
#include <algorithm>

namespace memory {

void Stats::LogAlloc(uint32_t size)
{
	m_allocated_bytes += size;
	++m_allocated_count;
}

void Stats::LogFree(uint32_t size)
{
	m_free_btyes += size;
	++m_free_count;
}

void Stats::Reset()
{
	m_allocated_bytes = 0;
	m_allocated_count = 0;
	m_free_btyes = 0;
	m_free_count = 0;
}

uint32_t Stats::GetAllocBytes() const
{
	return m_allocated_bytes;
}

uint32_t Stats::GetAllocCount() const
{
	return m_allocated_count;
}

uint32_t Stats::GetFreeBytes() const
{
	return m_free_btyes;
}

uint32_t Stats::GetFreeCount() const
{
	return m_free_count;
}

void Format::Set(uint32_t size, uint32_t align)
{
	m_size = size;
	m_alignment = align;
}

void Format::Align()
{
	m_size = Impl::Align(m_size, m_alignment);
}

bool Format::operator==(const Format& other) const
{
	return (m_size == other.m_size) &&
		(m_alignment == other.m_alignment);
}

bool Format::operator!=(const Format& other) const
{
	return (*this) == other;
}

Format& Format::operator+=(const Format& other)
{
	m_alignment = std::max(m_alignment, other.m_alignment);
	const auto size = Impl::Align(m_size, other.m_alignment);
	m_size = size + other.m_size;
	return *this;
}

Format Format::operator+(const Format& other) const
{
	Format result(*this);
	return result += other;
}

Format& Format::operator*=(uint32_t n)
{
	if (n > 1)
	{
		const auto size = Impl::Align(m_size, m_alignment);
		m_size = size * n;
	}
	return *this;
}

Format Format::operator*(uint32_t n) const
{
	Format result(*this);
	return result *= n;
}

Format& Format::operator+=(uint32_t size)
{
	m_size += size;
	return *this;
}

Format Format::operator+(uint32_t size) const
{
	Format result(*this);
	return result += size;
}

Format& Format::operator-=(uint32_t size)
{
	m_size -= size;
	return *this;
}

Format Format::operator-(uint32_t size) const
{
	Format result(*this);
	return result -= size;
}

void Resource::Set(uint8_t* buffer, uint32_t size)
{
	m_buffer = buffer;
	m_size = size;
}

void Resource::Increment(uint32_t size)
{
	m_buffer = m_buffer + size;
	m_size -= size;
}

void Resource::Decrement(uint32_t size)
{
	m_buffer = m_buffer - size;
	m_size += size;
}

uint8_t* Resource::Align(uint32_t align)
{
	uint8_t* aligned_buffer = Impl::Align(m_buffer, align);
	Increment(static_cast<uint32_t>(aligned_buffer - m_buffer));
	return m_buffer;
}

uint8_t* Resource::AlignAndIncrement(const Format& align)
{
	Align(align.Alignment());
	Increment(align.Size());
	return m_buffer;
}

bool Resource::Contains(const uint8_t* data) const
{
	if (data < m_buffer + m_size)
		if (data >= m_buffer)
			return true;
	return false;
}

} // namespace memory
