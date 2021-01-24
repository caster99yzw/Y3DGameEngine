#include "stream.h"

namespace io {

bool Stream::SeekSet(uint64_t offset)
{
	return Seek(offset, SeekMode::SeekBegin);
}

bool Stream::SeekCur(uint64_t offset)
{
	return Seek(offset, SeekMode::SeekCurrent);
}

bool Stream::SeekEnd(uint64_t offset)
{
	return Seek(offset, SeekMode::SeekEnd);
}

} // namespace io

