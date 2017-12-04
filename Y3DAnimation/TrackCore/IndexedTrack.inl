//#include "IndexedTrack.h"

template <typename _K, typename _E>
TrackBase<_K, _E>::TrackBase()
	: m_strName()
	, m_arrTrackData()
	, m_userData(nullptr)
{
}

template <typename _K, typename _E>
TrackBase<_K, _E>::~TrackBase()
{
}

template <typename _K, typename _E>
char const* TrackBase<_K, _E>::GetName() const
{
	return m_strName.c_str();
}

template <typename _K, typename _E>
void TrackBase<_K, _E>::SetName(char const* name)
{
	m_strName = name;
}

//template <typename _K, typename _E>
//void TrackBase<_K, _E>::Clear()
//{
//	m_arrTrackData.clear();
//}
		
template <typename _K, typename _E>
IndexedKey* TrackBase<_K, _E>::CreateKey(TimeType t)
{
	INT32 idx = FindIndexByTime(t);

	if (idx < m_arrTrackData.size())
	{
		TrackElement e(t, KeyType());
		m_arrTrackData.insert(idx, std::move(e));
	}

	return &m_arrTrackData[idx];
}

template <typename _K, typename _E>
KeyHandle TrackBase<_K, _E>::DuplicateKey(KeyHandle h)
{
	TrackElement* e = GetTrackElementByHandle(h);
	if (!e) return InvalidHandle;

	//	put back original frame
	UINT32 newIdx = (UINT32)(e - m_arrTrackData._Get_data()) + 1;
	TrackElement newEle(0/*e->GetKeyTime*/, 0/*e->value*/);
	auto handle = newEle.GetHandle();
	m_arrTrackData.insert(newIdx, std::move(newEle));
	return handle;
}


template <typename _K, typename _E>
bool TrackBase<_K, _E>::RemoveKey(KeyHandle h)
{
	INT32 idx = FindIndexByHandle(h);
	return RemoveKeyAtIndex(idx);
}

template <typename _K, typename _E>
bool TrackBase<_K, _E>::RemoveKeyAtIndex(UINT32 index)
{
	if (index < 0 || m_arrTrackData.size() < index)
		return false;

	m_arrTrackData.erase(index);
	return true;
}

template <typename _K, typename _E>
bool TrackBase<_K, _E>::UpdateKeyTime(KeyHandle h, TimeType time)
{
	TrackElement* pEle = GetTrackElementByHandle(h);
	if (!pEle) return false;

	INT32 idx = (INT32)(pEle - m_arrTrackData._Get_data());
	INT32 newIdx = FindIndexByTime(time);

	TrackElement newEle = std::move(*pEle);
	newEle.SetKeyTimeUnsafe(time);

	if (idx < newIdx)
	{
		--newIdx;
		for (INT32 i = idx; i < newIdx; ++i)
			m_arrTrackData[i] = std::move(m_arrTrackData[i + 1]);
	}
	
	if (idx > newIdx)
	{
		for (INT32 i = idx; i > newIdx; --i)
			m_arrTrackData[i] = std::move(m_arrTrackData[i - 1]);
	}

	m_arrTrackData[newIdx] = std::move(pEle);
	return true;
}

template <typename _K, typename _E>
bool TrackBase<_K, _E>::UpdateKeyDeltaTime(KeyHandle h, TimeType deltaTime)
{
	TrackElement* pEle = GetTrackElementByHandle(h);
	if (!pEle) return false;

	TimeType time = pEle->GetKeyTime() + deltaTime;
	return UpdateKeyTime(time);
}

template <typename _K, typename _E>
void TrackBase<_K, _E>::KeyTimeChanged(KeyHandle h)
{
	INT32 index = FindIndexByHandle(h);
	if (index == -1) return;

	if (index != 0 && m_arrTrackData[index - 1].GetKeyTime() >= m_arrTrackData[index].GetKeyTime())
	{
		//TrackElement e(std::move((m_arrTrackData[index]));
		//
		//TrackElement* pEle;// = lower_bound(m_arrTrackData.)
		//INT32 indexNew = (INT32)(pEle - m_arrTrackData._Get_data());

		for (INT32 i = index; i > indexNew; --i)
			m_arrTrackData[i] = std::move(m_arrTrackData[i - 1]);

		m_arrTrackData[indexNew] = e;
	}
	else if (index + 1 < Num() && m_arrTrackData[index].GetKeyTime() >= m_arrTrackData[index + 1].GetKeyTime())
	{
		//TrackElement e(std::move(m_arrTrackData[index]));
		//
		//TrackElement* pEle;// = lower_bound(m_arrTrackData.)
		//INT32 indexNew = (INT32)(pEle - m_arrTrackData._Get_data());

		for (INT32 i = index; i > indexNew; --i)
			m_arrTrackData[i] = std::move(m_arrTrackData[i - 1]);

		m_arrTrackData[indexNew] = e;
	}
}

template <typename _K, typename _E>
INT32 TrackBase<_K, _E>::FindIndexByTime(FLOAT32 t) const
{
	INT32 index = lower_bound(time);

	if (i < m_arrTrackData.size() && Equi(time, m_arrTrackData[i].GetKeyTime()))
		return i;
	else if (i > 0 && Equi(time, m_arrTrackData[i - 1].GetKeyTime()))
		return --i;
	else
		return -1;
}

template <typename _K, typename _E>
INT32 TrackBase<_K, _E>::FindIndexByHandle(KeyHandle h) const
{
	for (INT32 i = 0; i < m_arrTrackData.size(); ++i)
	{
		if (m_arrTrackData[i].GetHandle() == h)
			return i;
	}
	return -1;
}

template <typename _K, typename _E>
INT32 TrackBase<_K, _E>::FindIndexByHandle(KeyHandle h, FLOAT32 low, FLOAT32 high) const
{
	INT32 from = FindOrderedIndex(low);
	INT32 to = FindOrderedIndexReverse(high);

	for (INT32 i = from; i < to; ++i)
	{
		if (m_arrTrackData[i].GetHandle() == h)
			return i;
	}
	return -1;
}

template <typename _K, typename _E>
IndexedKey* TrackBase<_K, _E>::GetKeyAtIndex(UINT32 index)
{
	return const_cast<IndexedKey*>(
		static_cast<TrackBase<_K, _E> const>(*this).GetKeyAtIndex(index)
		);
}

template <typename _K, typename _E>
IndexedKey const* TrackBase<_K, _E>::GetKeyAtIndex(UINT32 index) const
{
	ASSERT(index > 0 && m_arrTrackData.size() > index);
	return &m_arrTrackData[index];
}

template <typename _K, typename _E>
IndexedKey* TrackBase<_K, _E>::GetKeyAtHandle(KeyHandle h) 
{
	return const_cast<IndexedKey*>(
		static_cast<TrackBase<_K, _E> const>(*this).GetKeyAtHandle(h)
		);
}

template <typename _K, typename _E>
IndexedKey const* TrackBase<_K, _E>::GetKeyAtHandle(KeyHandle h) const
{
	INT32 idx = FindIndexByHandle(h);
	return (idx != -1) ? &m_arrTrackData[i] : nullptr;
}

template <typename _K, typename _E>
UINT32 TrackBase<_K, _E>::Num() const
{
	return m_arrTrackData.size();
}

template <typename _K, typename _E>
void TrackBase<_K, _E>::SetNum(UINT32 num) const
{
	//m_arrTrackData.
}

template <typename _K, typename _E>
typename TrackBase<_K, _E>::TimeType TrackBase<_K, _E>::GetMinKeyTime() const
{
	ASSERT(m_arrTrackData.size() > 0);
	return m_arrTrackData[0].GetKeyTime();
}

template <typename _K, typename _E>
typename TrackBase<_K, _E>::TimeType TrackBase<_K, _E>::GetMaxKeyTime() const
{
	ASSERT(m_arrTrackData.size() > 0);
	return m_arrTrackData[m_arrTrackData.size() - 1].GetKeyTime();
}

template <typename _K, typename _E>
bool TrackBase<_K, _E>::UpdateKeyValue(KeyHandle h, void* pData)
{
	if (!pData)
		return false;

	TrackElement* pEle = GetTrackElementByHandle(h);
	if (!pEle)
		return false;

	typename TrackElement::ValueType* pKey = (typename TrackElement::ValueType*)pData;
	pEle->Value = *pKey;
	return true;
}

template <typename _K, typename _E>
void TrackBase<_K, _E>::ScaleTime(FLOAT32 factor)
{
	if (factor == 0) return;

	for (INT32 i = 0; i < m_arrTrackData.size(); ++i)
	{
		m_arrTrackData[i].SetKeyTimeUnsafe(m_arrTrackData[i].GetKeyTime() * factor);
	}

	if (factor < 0)
	{
		for (INT32 i = 0; i < m_arrTrackData.size(); ++i)
		{
			//	swap
			TrackElement e = std::move(m_arrTrackData[m_arrTrackData.size()]);
			m_arrTrackData[m_arrTrackData.size()] = std::move(m_arrTrackData[i]);
			m_arrTrackData[i] = e;
		}
	}
}

template <typename _K, typename _E>
void TrackBase<_K, _E>::EstimateKeyFrameNum(UINT32 num)
{
	//m_arrTrackData.capacity(num);
}

template <typename _K, typename _E>
KeyHandle TrackBase<_K, _E>::AddKeyNoOverride(TimeType t, KeyType const& key)
{
	INT32 i;
	FindIndexByTime(t, i);
	TrackElement e;
	e.Value = key;
	e.SetKeyTimeUnsafe(t);
	auto handle = e.GetHandle();
	m_arrTrackData.insert(i, std::move(e));
	return handle;
}

template <typename _K, typename _E>
KeyHandle TrackBase<_K, _E>::AddOrUpdateKey(TimeType t, KeyType const& key)
{
	INT32 i;
	if (FindIndexByTime(t, i))
	{
		m_arrTrackData[i].Value = std::move(key);
		return m_arrTrackData[i].GetHandle();
	}
	else
	{
		TrackElement e;
		e.Value = key;
		e.SetKeyTimeUnsafe(t);
		auto handle = e.GetHandle();
		m_arrTrackData.insert(i, std::move(e));
		return handle;
	}

}

template <typename _K, typename _E>
KeyHandle TrackBase<_K, _E>::AddOrUpdateKey(TimeType t, KeyType && key)
{
	INT32 i;
	if (FindIndexByTime(t, i))
	{
		m_arrTrackData[i].Value = std::move(key);
		return m_arrTrackData[i].GetHandle();
	}
	else
	{
		TrackElement e;
		e.Value = std::move(key);
		e.SetKeyTimeUnsafe(t);
		auto handle = e.GetHandle();
		m_arrTrackData.insert(i, std::move(e));
		return handle;
	}
}

template <typename _K, typename _E>
UINT32 TrackBase<_K, _E>::UpperBound(TimeType t) const
{
	//return static_cast<UINT32>(
	//	UpperBound(m_arrTrackData._Get_data(), t, Less<TrackElement>()) - m_arrTrackData._Get_data())
	//	);
}

template <typename _K, typename _E>
UINT32 TrackBase<_K, _E>::LowerBound(TimeType t) const
{
	//return static_cast<UINT32>(
	//	LowerBound(m_arrTrackData._Get_data(), t, Less<TrackElement>()) - m_arrTrackData._Get_data())
	//	);
}

template <typename _K, typename _E>
typename TrackBase<_K, _E>::TrackElement& TrackBase<_K, _E>::GetTrackElementAtIndex(UINT32 index)
{
	return const_cast<typename TrackBase<_K, _E>::TrackElement&>(
		static_cast<TrackBase<_K, _E> const>(*this).GetTrackElementAtIndex(index)
		);
}

template <typename _K, typename _E>
typename TrackBase<_K, _E>::TrackElement const& TrackBase<_K, _E>::GetTrackElementAtIndex(UINT32 index) const
{
	return m_arrTrackData[index];
}

template <typename _K, typename _E>
typename TrackBase<_K, _E>::TrackElement* TrackBase<_K, _E>::GetTrackElementByHandle(KeyHandle h)
{
	return const_cast<typename TrackBase<_K, _E>::TrackElement*>(
		static_cast<TrackBase<_K, _E> const>(*this).GetTrackElementByHandle(h)
		);
}

template <typename _K, typename _E>
typename TrackBase<_K, _E>::TrackElement const* TrackBase<_K, _E>::GetTrackElementByHandle(KeyHandle h) const
{
	INT32 i = FindIndexByHandle(h);

	if (i != -1)
		return &m_arrTrackData[i];
	else
		return nullptr;
}

//template <typename _K, typename _E>
//void TrackBase<_K, _E>::AddUnorderedFrames()
//{
//	//m_arrTrackData.
//	//for (int i = 0)
//
//	SortByTime();
//}
//
//template <typename _K, typename _E>
//void TrackBase<_K, _E>::MoveTrackArray()
//{
//	//
//}

template <typename _K, typename _E>
void TrackBase<_K, _E>::GetRelatedTrackElementsBS(TimeType inTime, INT32& trackIdx1, INT32& trackIdx2, FLOAT32& tOut) const
{

	ASSERT(m_arrTrackData.size() != 0);

	INT32 i = 0;
	if (inTime <= m_arrTrackData[0].GetKeyTime())
	{
		trackIdx1 = trackIdx2 = 0;
		tOut = 0;
		return;
	}

	if (inTime >= m_arrTrackData[m_arrTrackData.size() - 1].GetKeyTime())
	{
		trackIdx1 = trackIdx2 = m_arrTrackData.size() - 1;
		tOut = 1;
		return;
	}


	i = UpperBound(inTime);
	trackIdx1 = i - 1;
	trackIdx2 = i;
	tOut = FLOAT32(inTime - m_arrTrackData[trackIdx1].GetKeyTime()) / (m_arrTrackData[trackIdx2].GetKeyTime() - m_arrTrackData[trackIdx1].GetKeyTime());
}

template <typename _K, typename _E>
UINT32 TrackBase<_K, _E>::FindOrderedIndex() const 
{
	UINT32 i = 0;
	while (i < m_arrTrackData.size() && time > m_arrTrackData[i].GetKeyTime())
	{
		++i;
	}
	return i;
}

template <typename _K, typename _E>
UINT32 TrackBase<_K, _E>::FindOrderedIndexReverse() const
{
	UINT32 i = m_arrTrackData.size();
	while (i > 0 && time > m_arrTrackData[i - 1].GetKeyTime())
	{
		--i;
	}
	return i;
}

template <typename _K, typename _E>
void TrackBase<_K, _E>::GetRelatedTrackElements(TimeType inTime, INT32& trackIdx1, INT32& trackIdx2, FLOAT32& tOut)
{
	ASSERT(m_arrTrackData.size() != 0);

	INT32 i = 0;
	if (inTime <= m_arrTrackData[0].GetKeyTime())
	{
		trackIdx1 = trackIdx2 = 0;
		tOut = 0;
		return;
	}

	if (inTime >= m_arrTrackData[m_arrTrackData.size() - 1].GetKeyTime())
	{
		trackIdx1 = trackIdx2 = m_arrTrackData.size() - 1;
		tOut = 1;
		return;
	}

	while (i < m_arrTrackData.size() && time > m_arrTrackData[i].GetKeyTime())
	{
		++i;
	}
	trackIdx1 = i - 1;
	trackIdx2 = i;
	tOut = FLOAT32(inTime - m_arrTrackData[trackIdx1].GetKeyTime()) / (m_arrTrackData[trackIdx2].GetKeyTime() - m_arrTrackData[trackIdx1].GetKeyTime());

}

template <typename _K, typename _E>
void TrackBase<_K, _E>::SortByTime()
{
	Sort(m_arrTrackData.begin(), m_arrTrackData.end(), less<TrackElement>());
}