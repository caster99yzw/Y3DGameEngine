#pragma once
#include "TrackCore/IndexedKey.h"

//////////////////////////////////////////////////////////////////////////
//
//	IndexedTrack 
//
//////////////////////////////////////////////////////////////////////////

class IndexedTrack
{
public:

	typedef AnimTimeType			TimeType;

	virtual							~IndexedTrack() = default;

	virtual bool					Save() const = 0;
	virtual bool					Load() = 0;
	virtual bool					Serialize() = 0; 	//	This function is used in UI, such as unity

														//virtual void					Clear() = 0;
	virtual UINT32					GetTrackKeyType() = 0;
	virtual bool					IsRangeTrack() const = 0;

	virtual bool					IsCompositeRange() const = 0;
	virtual bool					SplitRangeKey() = 0;

	virtual char const*				GetName() const = 0;
	virtual void					multisetName(char const* name) = 0;

public:

	virtual IndexedKey*				CreateKey(TimeType t) = 0;
	virtual KeyHandle				DuplicateKey(KeyHandle h) = 0;

	virtual bool					RemoveKey(KeyHandle h) = 0;
	virtual bool					RemoveKeyAtIndex(UINT32 index) = 0;

	virtual bool					UpdateKeyTime(KeyHandle h, TimeType time) = 0;
	virtual bool					UpdateKeyDeltaTime(UINT32 index, TimeType deltaTime) = 0;

	virtual void					KeyTimeChanged(KeyHandle h) = 0;

	//	Group

	//	Find Index
	virtual INT32					FindIndexByTime(FLOAT32 t) const = 0;
	virtual INT32					FindIndexByHandle(KeyHandle h) const = 0;
	virtual INT32					FindIndexByHandle(KeyHandle h, FLOAT32 low, FLOAT32 high) const = 0;

	//	Find Key
	virtual	IndexedKey*				GetKeyAtIndex(UINT32 index) = 0;
	virtual	IndexedKey const*		GetKeyAtIndex(UINT32 index) const = 0;
	virtual	IndexedKey*				GetKeyAtHandle(KeyHandle h) = 0;
	virtual	IndexedKey const*		GetKeyAtHandle(KeyHandle h) const = 0;

	virtual UINT32					Num() const = 0;
	virtual void					multisetNum(UINT32 num) const = 0;
};
