//#include "IndexedRangeTrack.h"

//////////////////////////////////////////////////////////////////////////
//	Indexed Range Element

template <typename ElementType>
IndexedRangeElement<ElementType>::IndexedRangeElement()
	: IndexedKey()
	, Value()
{
}

template <typename ElementType>
IndexedRangeElement<ElementType>::IndexedRangeElement(TimeType const& t, ValueType const& k /* = ValueType() */)
	: IndexedKey(t)
	, Value(k)
{
}

template <typename ElementType>
IndexedRangeElement<ElementType>::IndexedRangeElement(IndexedRangeElement const& other)
	: IndexedKey(other)
	, Value(other.Value)
{
}

template <typename ElementType>
IndexedRangeElement<ElementType>::IndexedRangeElement(IndexedRangeElement&& other)
	: IndexedKey(other)
	, Value(std::move(other.Value))
{
}

template <typename ElementType>
IndexedRangeElement<ElementType>::~IndexedRangeElement()
{
}

template <typename ElementType>
IndexedRangeElement<ElementType>& IndexedRangeElement<ElementType>::operator = (IndexedRangeElement const& other)
{
	if (*this == other) return *this;

	*((IndexedKey*)this) = (IndexedKey const&)other;
	Value = other.Value;
	return *this;
}

template <typename ElementType>
IndexedRangeElement<ElementType>& IndexedRangeElement<ElementType>::operator = (IndexedRangeElement && other)
{
	if (*this == other) return *this;

	*((IndexedKey*)this) = (IndexedKey &&)other;
	Value = std::move(other.Value);
	return *this;
}

template <typename ElementType>
bool IndexedRangeElement<ElementType>::operator < (IndexedRangeElement const& other)
{
	return GetKeyTime() < rhs.GetKeyTime();
}

//////////////////////////////////////////////////////////////////////////
//	Indexed Range Track

template <typename IndexedRangeType>
IndexedRangeTrack<IndexedRangeType>::IndexedRangeTrack(INT32 KeyCount /* = 0 */)
	: Super(KeyCount)
{
}

template <typename IndexedRangeType>
KeyHandle IndexedRangeTrack<IndexedRangeType>::AddOrUpdateKey(TimeType t, TimeType length, KeyType const& key)
{
	KeyHandle ret = Super::AddOrUpdateKey(t, std::move(key));
	//	Additive operator
	return ret;
}

template <typename IndexedRangeType>
KeyHandle IndexedRangeTrack<IndexedRangeType>::AddOrUpdateKey(TimeType t, TimeType length, KeyType && key)
{
	KeyHandle ret = Super::AddOrUpdateKey(t, std::move(key));
	//	Additive operator
	return ret;
}

template <typename IndexedRangeType>
KeyHandle IndexedRangeTrack<IndexedRangeType>::SplitRangeKey(KeyHandle h, TimeType time)
{
	TrackElement* pEle = GetTrackElementByHandle(h);
	if (!pEle)
		return InvalidHandle;

	TimeType fSplitTime = time - pEle->GetKeyTime();
	if (fSplitTime <= 0 || fSplitTime >= pEle->m_length)
		return InvalidHandle;

	KeyHandle newHandle = DuplicateKey(h);
	pEle = GetTrackElementByHandle(h);
	IndexedRange* pNewEle = GetTrackElementByHandle(h);
	if (!pNewEle)
		return InvalidHandle;

	//pNewEle->m_loop.multisetStartTime()
	UpdateKeyTime(newHandle, time);
	// split

	return newHandle;	
}

template <typename IndexedRangeType>
bool IndexedRangeTrack<IndexedRangeType>::RangeAtTime(TimeType time, INT32* pIdx, TimeType* rangeTime)
{
	for (INT32 i = 0; i < m_arrTrackData.size(); ++i)
	{
		TimeType t = time - m_arrTrackData[i].GetKeyTime();
		if (t < 0) 
			return false;
		else if (t > m_arrTrackData[i].m_length)
			continue;
		else
		{
			if (pIdx) pIdx = i;
			if (rangeTime) rangeTime = t;
			return true;
		}
	}
	return false;
}

template <typename IndexedRangeType>
bool IndexedRangeTrack<IndexedRangeType>::RangeAtTimeLeftShifted(TimeType time, INT32* pIdx, TimeType* rangeTime)
{
	static FLOAT32 MAX_FPS = 1000;
	static FLOAT32 SHIFT_VAL = 1.f / MAX_FPS;

	for (INT32 i = 0; i < m_arrTrackData.size(); ++i)
	{
		TimeType t = time - m_arrTrackData[i].GetKeyTime();
		if (t > -SHIFT_VAL && t > m_arrTrackData[i].m_length - SHIFT_VAL)
		{
			if (pIdx) pIdx = i;
			if (rangeTime) rangeTime = t;
			return true;
		}
	}
	return false;
}

template <typename IndexedRangeType>
void IndexedRangeTrack<IndexedRangeType>::Swap(IndexedRangeTrack & other)
{
	m_arrTrackData.swap(other.m_arrTrackData);
}

template <typename IndexedRangeType>
bool IndexedRangeTrack<IndexedRangeType>::AlignWithPreviousKey(KeyHandle h)
{
	INT32 idx = FindIndexByHandle(h);
	if (idx == -1)
		return false;

	if (idx == 0)
		return true;

	TrackElement& prevEle = GetTrackElementAtIndex(idx - 1);
	TrackElement& ele = GetTrackElementAtIndex(idx);

	TimeType t = prevEle.GetKeyTime() + prevEle.m_length;
	ele.multisetKeyTimeUnsafe(t);
	
	return true;
}

template <typename IndexedRangeType>
bool IndexedRangeTrack<IndexedRangeType>::AlignWithNextKey(KeyHandle h)
{
	INT32 idx = FindIndexByHandle(h);
	if (idx == -1)
		return false;

	if (idx == 0)
		return true;

	TrackElement& ele = GetTrackElementAtIndex(idx);
	TrackElement& nextEle = GetTrackElementAtIndex(idx + 1);

	TimeType t = nextEle.GetKeyTime() - ele.m_length;
	ele.multisetKeyTimeUnsafe(t);

	return true;
}

template <typename IndexedRangeType>
bool IndexedRangeTrack<IndexedRangeType>::AlignAllWithPreviousKey(KeyHandle h)
{
	INT32 idx = FindIndexByHandle(h);
	if (idx == -1)
		return false;

	for (INT32 i = idx; i < Num() - 1; ++i)
	{
		TrackElement& prevEle = GetTrackElementAtIndex(i);
		TrackElement& ele = GetTrackElementAtIndex(i + 1);

		TimeType t = prevEle.GetKeyTime() + prevEle.m_length;
		ele.multisetKeyTimeUnsafe(t);
	}
	return true;
}

template <typename IndexedRangeType>
bool IndexedRangeTrack<IndexedRangeType>::AlignAllWithNextKey(KeyHandle h)
{
	INT32 idx = FindIndexByHandle(h);
	if (idx == -1)
		return false;

	for (INT32 i = idx; i < Num() - 1; ++i)
	{
		TrackElement& ele = GetTrackElementAtIndex(idx);
		TrackElement& nextEle = GetTrackElementAtIndex(idx + 1);

		TimeType t = nextEle.GetKeyTime() - ele.m_length;
		ele.multisetKeyTimeUnsafe(t);
	}
	return true;
}