
#include "IndexedKeyTrack.h"
#include "Interval.h"

//////////////////////////////////////////////////////////////////////////
//	Indexed Key Element

template <typename ElementType>
IndexedKeyElement<ElementType>::IndexedKeyElement()
	: IndexedKey()
	, Value()
{
}

template <typename ElementType>
IndexedKeyElement<ElementType>::IndexedKeyElement(TimeType const& t, ValueType const& k /* = ValueType() */)
	: IndexedKey(t)
	, Value(k)
{
}

template <typename ElementType>
IndexedKeyElement<ElementType>::IndexedKeyElement(IndexedKeyElement const& other)
	: IndexedKey(other)
	, Value(other.Value)
{
}

template <typename ElementType>
IndexedKeyElement<ElementType>::IndexedKeyElement(IndexedKeyElement&& other)
	: IndexedKey(other)
	, Value(std::move(other.Value))
{
}

template <typename ElementType>
IndexedKeyElement<ElementType>::~IndexedKeyElement()
{
}

template <typename ElementType>
IndexedKeyElement<ElementType>& IndexedKeyElement<ElementType>::operator = (IndexedKeyElement const& other)
{
	if (*this == other) return *this;

	*((IndexedKey*)this) = (IndexedKey const&)other;
	Value = other.Value;
	return *this;
}

template <typename ElementType>
IndexedKeyElement<ElementType>& IndexedKeyElement<ElementType>::operator = (IndexedKeyElement && other)
{
	if (*this == other) return *this;

	*((IndexedKey*)this) = (IndexedKey &&)other;
	Value = std::move(other.Value);
	return *this;
}

template <typename ElementType>
bool IndexedKeyElement<ElementType>::operator < (IndexedKeyElement const& other)
{
	return GetKeyTime() < rhs.GetKeyTime();
}

//////////////////////////////////////////////////////////////////////////
//	Indexed Key Track

template <typename IndexedKeyType>
IndexedKeyTrack<IndexedKeyType>::IndexedKeyTrack(INT32 KeyCount /* = 0 */)
	: Super(KeyCount)
{
}

template <typename IndexedKeyType>
void IndexedKeyTrack<IndexedKeyType>::Swap(IndexedKeyTrack & other)
{
	m_arrTrackData.swap(other.m_arrTrackData);
}

template <typename IndexedKeyType>
typename IndexedKeyTrack<IndexedKeyType>::KeyType IndexedKeyTrack<IndexedKeyType>::Evaluate(TimeType time) const
{
	INT32 ia, ib;
	FLOAT32 t;
	Super::GetRelatedTrackElementsBS(time, ia, ib, t);

	IndexedKeyTrack<IndexedKeyType>::KeyType ret;
	ret = Lerp(m_arrTrackData[ia].Value, m_arrTrackData[ib].Value, t);

	return ret;
}

template <typename IndexedKeyType> 
typename IndexedKeyTrack<IndexedKeyType>::KeyType IndexedKeyTrack<IndexedKeyType>::Evaluate(TimeType time, KeyType const& fallbackVal) const
{
	if (Super::Num() < 0) return fallbackVal;
	return Evaluate(time);
}

template <typename IndexedKeyType>
bool IndexedKeyTrack<IndexedKeyType>::FindIdxInTimeSpan(TimeType t1, TimeType t2, UINT32 openCloseFlag, INT32& idx1, INT32& idx2) const
{
	if (Super::Num() < 0)
	{
		idx1 = idx2 = 0;
		return false;
	}

	bool res = false;
	int i, j;

	GetInterval(m_arrTrackData, t1, t2, openCloseFlag, i, j);

	idx1 = 1;
	idx2 = j - 1;
	return res;
}


