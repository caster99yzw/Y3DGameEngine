#include "AnimLoop.h"

//////////////////////////////////////////////////////////////////////////
// Global Functions

void loopMod(FLOAT32 t, UINT32& count, FLOAT64& residual)
{
	FLOAT64 _Int = floor(t);

	if (FloatIsZero<FLOAT64>(_Int)) _Int = -1.f;

	count = (UINT32)_Int;
	residual = t - count;
}

//////////////////////////////////////////////////////////////////////////
// AnimLoop

AnimLoop::AnimLoop()
{
}


AnimLoop::~AnimLoop()
{
}

AnimLoop::TimeType AnimLoop::MapToLoop(TimeType t, UINT32& count) const
{


	return 0;
}
