#pragma once
#include "../Helper.h"

#include "TrackCore/AnimLoop.h"

enum TrackKeyType
{
	TRACK_VECTOR3_FLOAT,
	TRACK_QUATERNION_FLOAT, 
	eGroupTrack,
	TRACK_TYPE_NUM = 0
};

//////////////////////////////////////////////////////////////////////////
//
//	IndexedKey 
//
//////////////////////////////////////////////////////////////////////////

class IndexedKey
{
public:

	typedef AnimTimeType	TimeType;

	explicit				IndexedKey(TimeType time = 0);
	explicit				IndexedKey(TimeType time, KeyHandle handle);

	virtual					~IndexedKey();

	TimeType				GetKeyTime() const { return m_time; }
	KeyHandle				GetHandle() const { return m_handle; }

	bool					operator < (IndexedKey const& rhs);

	virtual bool			Save() const { return true; };
	virtual bool			Load() { return true; };
	virtual bool			Serialize() { return true; }; 	//	This function is used in UI, such as unity

	void					SetKeyTimeUnsafe(TimeType t) { m_time = t; };

private:

	static KeyHandle		NextHandle();

protected:

	KeyHandle				m_handle;
	TimeType				m_time;
};

//////////////////////////////////////////////////////////////////////////
//
//	IndexedRange 
//
//////////////////////////////////////////////////////////////////////////

class IndexedRange : public IndexedKey
{
public:

	explicit				IndexedRange(TimeType startTime = 0, TimeType length = 0, bool bReverse = false, bool bLooked = false);

							~IndexedRange();

	virtual bool			Save() const override { return true; };
	virtual bool			Load() override { return true; };
	virtual bool			Serialize() override { return true; }; 	//	This function is used in UI, such as unity

	TimeType				GetCurrentLoopUnitLength() const { return 0; }
	TimeType				GetOriginalLoopUnitLength() const { return 0; }
	FLOAT32					GetLoopCount() const { return 0; }
	bool					IsScale() const { return 0; }


	void					SetLength(TimeType len);
	//void					ScaleToLength(TimeType len);
	//void					LoopToLength(TimeType len);
	void					RestoreLengthByDelta(TimeType len);

	// Mapping time between global time to range
	TimeType				MapToRange(TimeType t) const;
	TimeType				MapFromRange(TimeType t) const;

private:

	TimeType				m_length;
	bool					m_reverse;
	bool					m_locked;
	AnimLoop				m_loop;
};