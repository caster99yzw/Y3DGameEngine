#include "stream_reader.h"

namespace io {

StreamReader::StreamReader(Stream& stream)
	: m_stream(stream)
{
}

size_t StreamReader::Read(void* buffer, size_t size) const
{
	return m_stream.Read(buffer, size);
}

ascichar StreamReader::ReadChar() const
{
	ascichar ret;
	Read(&ret, sizeof(ret));
	return ret;
}

uint8_t StreamReader::ReadByte() const
{
	uint8_t ret;
	Read(&ret, sizeof(ret));
	return ret;
}

int16_t StreamReader::ReadInt16() const
{
	uint16_t ret;
	Read(&ret, sizeof(ret));
	return ret;
}

uint16_t StreamReader::ReadUInt16() const
{
	uint16_t ret;
	Read(&ret, sizeof(ret));
	return ret;
}

int32_t StreamReader::ReadInt32() const
{
	int32_t ret;
	Read(&ret, sizeof(ret));
	return ret;
}

uint32_t StreamReader::ReadUInt32() const
{
	uint32_t ret;
	Read(&ret, sizeof(ret));
	return ret;
}

int64_t StreamReader::ReadInt64() const
{
	int64_t ret;
	Read(&ret, sizeof(ret));
	return ret;
}

uint64_t StreamReader::ReadUInt64() const
{
	uint64_t ret;
	Read(&ret, sizeof(ret));
	return ret;
}

float32 StreamReader::ReadFloat32() const
{
	float32 ret;
	Read(&ret, sizeof(ret));
	return ret;
}

float64 StreamReader::ReadFloat64() const
{
	float64 ret;
	Read(&ret, sizeof(ret));
	return ret;
}

size_t StreamReader::ReadSize() const
{
	size_t ret;
	Read(&ret, sizeof(ret));
	return ret;
}

std::string StreamReader::ReadString() const
{
	size_t len = ReadSize();
	std::string ret;
	if (len > 0)
	{
		ret.resize(len);
		Read(const_cast<char*>(ret.data()), sizeof(ascichar) * len);
	}
	return ret;
}

} // namespace io

