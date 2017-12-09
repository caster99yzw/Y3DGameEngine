#pragma once
#include "TrackCore/IndexedTrack.h"

//////////////////////////////////////////////////////////////////////////
//
//	Indexed Key Element Template
//
//////////////////////////////////////////////////////////////////////////

template <typename ElementType>
struct IndexedKeyElement : public IndexedKey
{
public:

	typedef ElementType				ValueType;

public:

	ValueType						Value;

public:

	inline							IndexedKeyElement();
	inline							IndexedKeyElement(TimeType const& t, ValueType const& k = ValueType());
	inline							IndexedKeyElement(IndexedKeyElement const& other);
	inline							IndexedKeyElement(IndexedKeyElement&& other);

	inline							~IndexedKeyElement();

	inline IndexedKeyElement&		operator = (IndexedKeyElement const& other);
	inline IndexedKeyElement&		operator = (IndexedKeyElement&& other);

	inline bool						operator < (IndexedKeyElement const& rhs);
};

//////////////////////////////////////////////////////////////////////////
//
//	Indexed Key Track Template
//
//////////////////////////////////////////////////////////////////////////

template <typename IndexedKeyType>
class IndexedKeyTrack : public TrackBase<IndexedKeyType, IndexedKeyElement<IndexedKeyType>>
{
	typedef	TrackBase<IndexedKeyType, IndexedKeyElement<IndexedKeyType>>	Super;

public:

	explicit						IndexedKeyTrack(INT32 KeyCount = 0);

public:

	virtual bool					Save() const override { return true; }
	virtual bool					Load() override { return true; }
	virtual bool					Serialize() override { return true; }	//	This function is used in UI, such as unity

	virtual bool					IsRangeTrack() const override { return false; }
	virtual bool					IsCompositeRange() const override { return false; }
	virtual bool					SplitRangeKey() override { return false; }

	inline KeyType					Evaluate(TimeType time) const;
	inline KeyType					Evaluate(TimeType time, KeyType const& fallbackVal) const;
	inline void						Swap(IndexedKeyTrack & other);

	inline bool						FindIdxInTimeSpan(TimeType t1, TimeType t2, UINT32 openCloseFlag, INT32& idx1, INT32& idx2) const;
};

#include "IndexedKeyTrack.inl"