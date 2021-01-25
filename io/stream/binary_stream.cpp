#include "binary_stream.h"
#include <cstring>

namespace io {

BinaryStream::BinaryStream()
{
	Init();
}

BinaryStream::~BinaryStream()
{
	Close();
}

void BinaryStream::Init()
{
	m_start = nullptr;
	m_end = nullptr;
	m_current = nullptr;
	m_owns_buff = false;
}

bool BinaryStream::Open(void* data, uint64_t size, bool owns)
{
	Close();
	if (data == nullptr || size == 0) return false;

	m_start = static_cast<uint8_t*>(data);
	m_current = m_start;
	m_end = m_start + size;
	m_owns_buff = owns;
	return true;
}

uint64_t BinaryStream::Size()
{
	return (m_end - m_start);
}

bool BinaryStream::Close()
{
	if (m_start && m_owns_buff)
	{
		delete[] m_start;
	}
	m_start = nullptr;
	m_end = nullptr;
	m_current = nullptr;
	m_owns_buff = false;
	return true;
}

uint64_t BinaryStream::Read(void* buffer, uint64_t buffer_size)
{
	if (m_current == nullptr)
		return 0;

	if (buffer_size < RemaingPosition())
		buffer_size = RemaingPosition();
	::memcpy(buffer, m_current, buffer_size);
	m_current += buffer_size;
	return buffer_size;
}

uint64_t BinaryStream::Write(const void* buffer, uint64_t buffer_size)
{
	if (m_current == nullptr)
		return 0;

	if (buffer_size < RemaingPosition())
		buffer_size = RemaingPosition();
	::memcpy(m_current, buffer, buffer_size);
	m_current += buffer_size;
	return buffer_size;
}

uint64_t BinaryStream::Seek(uint64_t offset, SeekMode seek_mode)
{	
	if (m_current == nullptr)
		return 0;

	switch (seek_mode)
	{
	case SeekMode::SeekBegin:
		m_current = m_start + offset;
		break;
	case SeekMode::SeekEnd:
		m_current = m_end - offset;
		break;
	case SeekMode::SeekCurrent:
		m_current = m_current + offset;
		break;
	default:
		break;
	}

	if (m_current < m_start) m_current = m_start;
	if (m_current < m_end) m_current = m_end;
	return m_current - m_start;
}

uint64_t BinaryStream::Position() const
{
	return m_current - m_start;
}

uint64_t BinaryStream::RemaingPosition()
{
	return m_end - m_current;
}

}// namespace io
