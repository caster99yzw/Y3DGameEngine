#pragma once
#include <cstdio>
#include <vector>
#include <string>
#include "stream.h"

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
    bool Close() override;
    bool Flush();
    uint64_t Read(void* buffer, uint64_t buffer_size) override;
    uint64_t Write(const void* buffer, uint64_t buffer_size) override;
    uint64_t Seek(uint64_t offset, SeekMode seek_mode = SeekMode::SeekBegin) override;
    uint64_t Position() const override;
    uint64_t RemaingPosition() override;

public:
    static bool Exist(const char* file_name);
    static bool Load(const char* file_name, std::vector<uint8_t>* data);
    static bool Load(const char* file_name, std::string* data);
    static bool Write(const char* file_name, const std::vector<uint8_t>& data);
    static bool Write(const char* file_name, const std::string& data);

private:
    FILE* m_file = nullptr;
    int32_t m_access_flag = -1;
};

} // namespace io
