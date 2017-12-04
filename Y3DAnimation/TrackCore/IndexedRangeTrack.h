#pragma once
#include "IndexedTrack.h"

//////////////////////////////////////////////////////////////////////////
//
//	Indexed Range Element Template
//
//////////////////////////////////////////////////////////////////////////

template <typename ElementType>
struct IndexedRangeElement : public IndexedRange
{
public:

	typedef ElementType				ValueType;

public:

	ValueType						Value;

public:

	inline							IndexedRangeElement();
	inline							IndexedRangeElement(TimeType const& t, ValueType const& k = ValueType());
	inline							IndexedRangeElement(IndexedRangeElement const& other);
	inline							IndexedRangeElement(IndexedRangeElement&& other);

	inline							~IndexedRangeElement();

	inline IndexedRangeElement&		operator = (IndexedRangeElement const& other);
	inline IndexedRangeElement&		operator = (IndexedRangeElement&& other);

	inline bool						operator < (IndexedRangeElement const& rhs);
};


//////////////////////////////////////////////////////////////////////////
//
//	Indexed Range Track Template
//
//////////////////////////////////////////////////////////////////////////

template <typename IndexedRangeType>
class IndexedRangeTrack : public TrackBase<IndexedRangeType, IndexedRangeElement<IndexedRangeType>>
{
	typedef	TrackBase<IndexedRangeType, IndexedRangeElement<IndexedRangeType>>	Super;

public:

	explicit						IndexedRangeTrack(INT32 KeyCount = 0);

public:

	virtual bool					Save() const override { return true };
	virtual bool					Load() override { return true };
	virtual bool					Serialize() override { return true }; 	//	This function is used in UI, such as unity

	virtual bool					IsRangeTrack() const override { return false };
	virtual bool					IsCompositeRange() const override { return false };
	virtual bool					SplitRangeKey() override { return false };

	//	override
	//	CreaterKey
	//	DuplicateKey
	virtual KeyHandle				AddOrUpdateKey(TimeType t, TimeType length, KeyType const& key);
	virtual KeyHandle				AddOrUpdateKey(TimeType t, TimeType length, KeyType && key);

	//	only for range track
	virtual KeyHandle				SplitRangeKey(KeyHandle h, TimeType time);

	inline bool						RangeAtTime(TimeType time, INT32* pIdx, TimeType* rangeTime);
	inline bool						RangeAtTimeLeftShifted(TimeType time, INT32* pIdx, TimeType* rangeTime);
	inline void						Swap(IndexedRangeTrack & other);

	virtual bool					AlignWithPreviousKey(KeyHandle h);
	virtual bool					AlignWithNextKey(KeyHandle h);
	virtual bool					AlignAllWithPreviousKey(KeyHandle h);
	virtual bool					AlignAllWithNextKey(KeyHandle h);
};

#include "IndexedRangeTrack.inl"
