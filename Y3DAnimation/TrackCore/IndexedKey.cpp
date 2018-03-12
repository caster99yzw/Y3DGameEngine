#include "IndexedKey.h"

//////////////////////////////////////////////////////////////////////////
// IndexedKey

IndexedKey::IndexedKey(TimeType time /* = 0 */)
	: m_time(time)
	, m_handle(NextHandle())
{
}

IndexedKey::IndexedKey(TimeType time, KeyHandle handle)
	: m_time(time)
	, m_handle(handle)
{
}

IndexedKey::~IndexedKey()
{
}

bool IndexedKey::operator < (IndexedKey const& rhs)
{
	return m_time < rhs.m_time;
}

KeyHandle IndexedKey::NextHandle()
{
	static KeyHandle nextHandle = 0;
	// reserve the front two position
	return nextHandle = (nextHandle + 1) & 0x3FFFFFFF;
}

//////////////////////////////////////////////////////////////////////////
// IndexedRange

IndexedRange::IndexedRange(TimeType startTime /* = 0 */, TimeType length /* = 0 */, bool bReverse /* = false */, bool bLooked /* = false */)
	: IndexedKey(startTime)
	, m_length(length)
	, m_reverse(bReverse)
	, m_locked(bLooked)
{
}

IndexedRange::~IndexedRange()
{
}

void IndexedRange::multisetLength(TimeType len)
{
	m_length = len;
}

void IndexedRange::RestoreLengthByDelta(TimeType len)
{

}

IndexedKey::TimeType IndexedRange::MapToRange(TimeType t) const
{
	TimeType timeInRange = Clamp<TimeType>(t - m_time, 0, m_length);
	return m_reverse ? m_length - timeInRange : timeInRange;
}

IndexedKey::TimeType IndexedRange::MapFromRange(TimeType t) const
{
	t = Clamp<TimeType>(t, 0, m_length);
	return  m_reverse ? (m_length - t) + m_time : t + m_time;
}