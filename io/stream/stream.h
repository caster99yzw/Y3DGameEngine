#pragma once
#include "common/Types.h"

namespace io {

enum class SeekMode
{
    SeekBegin,
	SeekCurrent,
	SeekEnd
};

class Stream
{
public:
    Stream() = default;
    Stream(Stream&) = delete;
    Stream& operator=(Stream&) = delete;
    Stream(Stream&&) = default;
    Stream& operator=(Stream&&) = default;
    virtual ~Stream() = default;

    virtual void Init() = 0;
    virtual uint64_t Size() = 0;
    virtual int64_t Read(void* buffer, uint64_t buffer_size) = 0;
    virtual int64_t Write(const void* buffer, uint64_t buffer_size) = 0;
    virtual bool Seek(uint64_t offset, SeekMode seek_mode = SeekMode::SeekBegin) = 0;
    virtual uint64_t Position() const = 0;
    virtual uint64_t RemaingPosition() = 0;

    bool SeekSet(uint64_t offset);
    bool SeekCur(uint64_t offset);
    bool SeekEnd(uint64_t offset);
};

} // namespace io
