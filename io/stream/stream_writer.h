#pragma once
#include <string>
#include "stream.h"

namespace io {

class StreamWriter
{
public:
	StreamWriter(Stream& stream);
    StreamWriter(StreamWriter&) = delete;
    StreamWriter& operator=(StreamWriter&) = delete;
    StreamWriter(StreamWriter&&) = default;
    StreamWriter& operator=(StreamWriter&&) = delete;
	virtual ~StreamWriter() = default;
	virtual size_t Write(const void* buffer, size_t size) const;
	virtual void WriteChar(ascichar value) const;
	virtual void WriteByte(uint8_t value) const;
	virtual void WriteInt16(int16_t value) const;
	virtual void WriteUInt16(uint16_t value) const;
	virtual void WriteInt32(int32_t value) const;
	virtual void WriteUInt32(uint32_t value) const;
	virtual void WriteInt64(int64_t value) const;
	virtual void WriteUInt64(uint64_t value) const;
	virtual void WriteFloat32(float32 value) const;
	virtual void WriteFloat64(float64 value) const;
	virtual void WriteSize(size_t value) const;
	virtual void WriteString(const std::string& value) const;
	FORCEINLINE Stream& GetStream() const { return m_stream; }

private:
	Stream& m_stream;
};

} // namespace io
