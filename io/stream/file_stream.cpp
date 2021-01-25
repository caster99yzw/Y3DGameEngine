#include "file_stream.h"
#include <cassert>

namespace io {

FileStream::FileStream()
{
    Init();
}

FileStream::~FileStream()
{
    Close();
}

void FileStream::Init()
{
    m_file = nullptr;
    m_access_flag = -1;
}

bool FileStream::Open(const char* file_name, int flags)
{
    if (flags & FileAccessFlag::Write)
    {
        if (flags & FileAccessFlag::Read)
        {
            m_access_flag = FileAccessFlag::Write | FileAccessFlag::Read;
            fopen_s(&m_file, file_name, "rb+");
        }
        else
        {
            m_access_flag = FileAccessFlag::Write;
            fopen_s(&m_file, file_name, "wb");
        }
    }
    else
    {
        m_access_flag = FileAccessFlag::Read;
        fopen_s(&m_file, file_name, "rb");
    }

    if (m_file == nullptr)
    {
        m_access_flag = -1;
        return false;
    }

    return true;
}

bool FileStream::IsOpen() const
{
    return (m_file == nullptr && m_access_flag == -1);
}

uint64_t FileStream::Size()
{
    if (IsOpen())
    {
        const auto pos = static_cast<uint64_t>(ftell(m_file));
        SeekEnd(0);
        const auto end = static_cast<uint64_t>(ftell(m_file));
        SeekSet(pos);
        return end;
    }
    return -1;
}

bool FileStream::Create(const char* file_name, int flags)
{
    if (flags & FileAccessFlag::Read)
    {
        m_access_flag = FileAccessFlag::Write | FileAccessFlag::Read;
        fopen_s(&m_file, file_name, "bw+");
    }
    else
    {
        m_access_flag = FileAccessFlag::Write;
        fopen_s(&m_file, file_name, "wb");
    }

    if (m_file == nullptr)
    {
        m_access_flag = -1;
        return false;
    }

    return true;
}

bool FileStream::Close()
{
    if (IsOpen())
    {
        if (fclose(m_file) != 0)
            return false;
    }

    m_file = nullptr;
    m_access_flag = -1;
    return true;
}

bool FileStream::Flush()
{
    if (IsOpen())
    {
        const auto pos = static_cast<uint64_t>(ftell(m_file));
        fflush(m_file);
        SeekSet(pos);
        return true;
    }
    return false;
}

uint64_t FileStream::Read(void* buffer, uint64_t buffer_size)
{
    return fread(buffer, 1, buffer_size, m_file);
}

uint64_t FileStream::Write(const void* buffer, uint64_t buffer_size)
{
    return fwrite(buffer, 1, buffer_size, m_file);
}

uint64_t FileStream::Seek(uint64_t offset, SeekMode seek_mode)
{
    return static_cast<uint64_t>(fseek(m_file, 
        static_cast<int>(offset),
        static_cast<int>(seek_mode)));
}

uint64_t FileStream::Position() const
{
    if (!IsOpen())
        return -1;

    return static_cast<uint64_t>(ftell(m_file));
}

uint64_t FileStream::RemaingPosition()
{
    if (!IsOpen())
        return -1;

    const auto pos = static_cast<uint64_t>(ftell(m_file));
    SeekEnd(0);
    const auto end = static_cast<uint64_t>(ftell(m_file));
    if (pos > end)
        return -1;

    SeekSet(pos);
    return end - pos;
}

bool FileStream::Exist(const char* file_name)
{
    struct stat status;
    const auto int_status = stat(file_name, &status);
    return (int_status == 0);
}

bool FileStream::Load(const char* file_name, std::vector<uint8_t>* data)
{
    FileStream stream;
    if (!stream.Open(file_name))
        return false;

    const auto size = stream.Size();
    stream.Read(data->data(), size);
    return true;
}

bool FileStream::Write(const char* file_name, const std::vector<uint8_t>& data)
{
    FileStream stream;
    if (!stream.Create(file_name))
        return false;

    const auto size = data.size();
    stream.Write(data.data(), size);
    return true;
}

} // namespace io
