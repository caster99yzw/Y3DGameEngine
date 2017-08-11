#pragma once

#include "Y3DTypes.h"

namespace Y3D
{
	const FLOAT32 Y3D_PI	= 3.1415926535f;
	const FLOAT32 Y3D_PI_DB	= 6.2831853072f;
	const FLOAT32 Y3D_PI_HF	= 1.5707963268f;
	const FLOAT32 Y3D_PI_QT	= 0.7853981634f;

	inline FLOAT32 const DEGREE_2_RADIAN(FLOAT32 degree)
	{
		return degree * Y3D_PI / 180.f;
	}

	inline FLOAT32 const RADIAN_2_DEGREE(FLOAT32 radian)
	{
		return radian * 180.f / Y3D_PI;
	}
}