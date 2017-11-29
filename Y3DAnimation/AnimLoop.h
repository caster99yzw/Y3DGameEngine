#pragma once
#include "Helper.h"

struct TimePairMap
{
	//Pair<>
};

class AnimLoop
{
public:

	typedef AnimTimeType	TimeType;

							AnimLoop();
							AnimLoop(AnimLoop const& other);
							AnimLoop(AnimLoop && other);
							~AnimLoop();

	AnimLoop&				operator =  (AnimLoop const& other);
	AnimLoop&				operator =  (AnimLoop && other);

	bool					Save() const { return true; };
	bool					Load() { return true; };
	bool					Serialize() { return true; }; 	//	This function is used in UI, such as unity

	TimeType				Map(TimeType t) const;
	TimeType				MapContinuous(TimeType curTime, TimeType prevTime, bool& bLooped) const;
	
	TimeType				MapToLoop(TimeType t, UINT32& count) const;

	// Loop, Range, global Time, asset time

	void					InverseMap();
	FLOAT32					LengthScale(); // assetLength * LengthScale = actual length;

	void					SplitTimeSpan();
	void					UnrollTimeSpan();

	void					SetStartTime(TimeType t);
	void					SetEndTime(TimeType t);
	void					SetLength(TimeType t);

	TimeType				GetStartTime();
	TimeType				GetEndTime();
	TimeType				GetLength();
	
private:

	TimeType				m_startTime;
	TimeType				m_endTime;
	TimeType				m_fLength;
};

