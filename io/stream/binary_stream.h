#pragma once
#include "stream.h"

namespace io {

class BinaryStream : public Stream
{
public:
    BinaryStream();
    ~BinaryStream() override;
	
    void Init() override;
    bool Open(void* data, uint64_t size, bool owns = false);
    uint64_t Size() override;
    bool Close() override;
    uint64_t Read(void* buffer, uint64_t buffer_size) override;
    uint64_t Write(const void* buffer, uint64_t buffer_size) override;
    uint64_t Seek(uint64_t offset, SeekMode seek_mode = SeekMode::SeekBegin) override;
    uint64_t Position() const override;
    uint64_t RemaingPosition() override;

    FORCEINLINE uint8_t* Start() const { return m_start; }
    FORCEINLINE uint8_t* Current() const { return m_current; }
    FORCEINLINE uint8_t* End() const { return m_end; }

private:
	uint8_t* m_start = nullptr;
	uint8_t* m_current = nullptr;
	uint8_t* m_end = nullptr;
	bool m_owns_buff;
};

} // namespace io

