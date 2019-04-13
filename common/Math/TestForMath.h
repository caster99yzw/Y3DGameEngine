#pragma once
#include "EastingFunction.h"
#include <iostream>

namespace y3dmath
{
	FORCEINLINE void ForPow()
	{
		for (float64 i = 0.f; i < 10.; i += 0.1)
		{
			std::cout << Pow(191., i) << "    ";
		}
		std::cout << std::endl;
	}
}

