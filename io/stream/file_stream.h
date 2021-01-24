#pragma once
#include "stream.h"
#include <cstdio>

namespace io {

enum FileAccessFlag : int32_t
{
    Read = 1 << 0,
    Write = 1 << 1
};

class FileStream : public Stream
{
public:
    FileStream();
    ~FileStream() override;
	
    void Init() override;
    bool Open(const char* file_name, int flags = FileAccessFlag::Read);
    bool IsOpen() const;
    uint64_t Size() override;
    bool Create(const char* file_name, int flags = FileAccessFlag::Write);
    bool Close();
    bool Flush();
    int64_t Read(void* buffer, uint64_t buffer_size) override;
    int64_t Write(const void* buffer, uint64_t buffer_size) override;
    bool Seek(uint64_t offset, SeekMode seek_mode = SeekMode::SeekBegin) override;
    uint64_t Position() const override;
    uint64_t RemaingPosition() override;

private:
    FILE* m_file = nullptr;
    int32_t m_access_flag = -1;
};

} // namespace io
