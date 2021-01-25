#pragma once
#include <string>
#include "stream.h"

namespace io {

class StreamReader
{
public:
	StreamReader(Stream& stream);
    StreamReader(StreamReader&) = delete;
    StreamReader& operator=(StreamReader&) = delete;
    StreamReader(StreamReader&&) = default;
    StreamReader& operator=(StreamReader&&) = delete;
	virtual ~StreamReader() = default;
	virtual size_t Read(void* buffer, size_t size) const;
	virtual ascichar ReadChar() const;
	virtual uint8_t ReadByte() const;
	virtual int16_t ReadInt16() const;
	virtual uint16_t ReadUInt16() const;
	virtual int32_t ReadInt32() const;
	virtual uint32_t ReadUInt32() const;
	virtual int64_t ReadInt64() const;
	virtual uint64_t ReadUInt64() const;
	virtual float32 ReadFloat32() const;
	virtual float64 ReadFloat64() const;
	virtual size_t ReadSize() const;
	virtual std::string ReadString() const;
	FORCEINLINE Stream& GetStream() const { return m_stream; }

private:
	Stream& m_stream;
};

} // namespace io
