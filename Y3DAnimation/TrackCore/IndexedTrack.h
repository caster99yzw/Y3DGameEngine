#pragma once
#include "IndexTrackBase.h"
#include <string>
#include <vector>



//////////////////////////////////////////////////////////////////////////
//
//	TrackBase 
//
//////////////////////////////////////////////////////////////////////////

template<typename _K, typename _E>
class TrackBase : public IndexedTrack
{
public:
	
	typedef _E						TrackElement;
	typedef _K						KeyType;

public:

	explicit						TrackBase();
	virtual							~TrackBase();

	virtual bool					Save() const = 0;
	virtual bool					Load() = 0;
	virtual bool					Serialize() = 0; 	//	This function is used in UI, such as unity

	//virtual void					Clear() override;
	virtual UINT32					GetTrackKeyType() const = 0;
	virtual bool					IsRangeTrack() const = 0;

	virtual bool					IsCompositeRange() const = 0;
	virtual bool					SplitRangeKey() = 0;

	virtual char const*				GetName() const = 0;
	virtual void					SetName(char const* name) = 0;

public:

	virtual IndexedKey*				CreateKey(TimeType t) override;
	virtual KeyHandle				DuplicateKey(KeyHandle h) override;

	virtual bool					RemoveKey(KeyHandle h) override;
	virtual bool					RemoveKeyAtIndex(UINT32 index) override;

	virtual bool					UpdateKeyTime(KeyHandle h, TimeType time) override;
	virtual bool					UpdateKeyDeltaTime(KeyHandle h, TimeType deltaTime) override;

	virtual void					KeyTimeChanged(KeyHandle h) override;

	//	Group

	//	Find Index
	virtual INT32					FindIndexByTime(FLOAT32 t) const override;
	virtual INT32					FindIndexByHandle(KeyHandle h) const override;
	virtual INT32					FindIndexByHandle(KeyHandle h, FLOAT32 low, FLOAT32 high) const override;

	//	Find Key
	virtual	IndexedKey*				GetKeyAtIndex(UINT32 index) override;
	virtual	IndexedKey const*		GetKeyAtIndex(UINT32 index) const override;
	virtual	IndexedKey*				GetKeyAtHandle(KeyHandle h) override;
	virtual	IndexedKey const*		GetKeyAtHandle(KeyHandle h) const override;

	virtual UINT32					Num() const override;
	virtual void					SetNum(UINT32 num) const override;

	//	Min/Max time in track
	virtual TimeType				GetMinKeyTime() const;
	virtual TimeType				GetMaxKeyTime() const;

	virtual bool					UpdateKeyValue(KeyHandle h, void* pData);

	virtual TrackBase*				Clone() const = 0;

	inline void						ScaleTime(FLOAT32 factor);
	inline void						EstimateKeyFrameNum(UINT32 num);

	virtual KeyHandle				AddKeyNoOverride(TimeType t, KeyType const& key);
	virtual KeyHandle				AddOrUpdateKey(TimeType t, KeyType const& key);
	virtual KeyHandle				AddOrUpdateKey(TimeType t, KeyType && key);

	//	Binary search (promise the ordered array)
	inline UINT32					UpperBound(TimeType t) const;
	inline UINT32					LowerBound(TimeType t) const;

	inline TrackElement&			GetTrackElementAtIndex(UINT32 index);
	inline TrackElement const&		GetTrackElementAtIndex(UINT32 index) const;
	inline TrackElement*			GetTrackElementByHandle(KeyHandle h);
	inline TrackElement const*		GetTrackElementByHandle(KeyHandle h) const;

	//inline void						AddUnorderedFrames();
	//inline void						MoveTrackArray();

	inline void						GetRelatedTrackElementsBS(TimeType inTime, INT32& trackIdx1, INT32& trackIdx2, FLOAT32& tOut) const;
	
protected:

	inline UINT32					FindOrderedIndex() const;
	inline UINT32					FindOrderedIndexReverse() const;
	inline void						GetRelatedTrackElements(TimeType inTime, INT32& trackIdx1, INT32& trackIdx2, FLOAT32& tOut);
	inline void						SortByTime();

protected:

	std::string						m_strName;
	std::vector<TrackElement>		m_arrTrackData;
	
	void*							m_userData;
};

//#include "IndexedTrack.inl"