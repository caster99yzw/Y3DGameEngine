#include "stream_writer.h"

namespace io {

StreamWriter::StreamWriter(Stream& stream)
	: m_stream(stream)
{
}

size_t StreamWriter::Write(const void* buffer, size_t size) const
{
	return m_stream.Write(buffer, size);
}

void StreamWriter::WriteChar(ascichar value) const
{
	Write(&value, sizeof(value));
}

void StreamWriter::WriteByte(uint8_t value) const
{
	// ReSharper disable once CppExpressionWithoutSideEffects
	Write(&value, sizeof(value));
}

void StreamWriter::WriteInt16(int16_t value) const
{
	Write(&value, sizeof(value));
}

void StreamWriter::WriteUInt16(uint16_t value) const
{
	Write(&value, sizeof(value));
}

void StreamWriter::WriteInt32(int32_t value) const
{
	Write(&value, sizeof(value));
}

void StreamWriter::WriteUInt32(uint32_t value) const
{
	Write(&value, sizeof(value));
}

void StreamWriter::WriteInt64(int64_t value) const
{
	Write(&value, sizeof(value));
}

void StreamWriter::WriteUInt64(uint64_t value) const
{
	Write(&value, sizeof(value));
}

void StreamWriter::WriteFloat32(float32 value) const
{
	Write(&value, sizeof(value));
}

void StreamWriter::WriteFloat64(float64 value) const
{
	Write(&value, sizeof(value));
}

void StreamWriter::WriteSize(size_t value) const
{
	Write(&value, sizeof(value));
}

void StreamWriter::WriteString(const std::string& value) const
{
	size_t len = value.size();
	WriteSize(len);
	if (len > 0)
	{
		Write(value.data(), sizeof(ascichar) * len);
	}
}

} // namespace io

