#pragma once
#include "Alpha.h"
#include "Interpolation.h"

namespace y3dmath
{
	FORCEINLINE Alpha SmoothStart2(Alpha value)
	{
		return AlphaHelper::Pow(value, 2);
	}

	FORCEINLINE Alpha SmoothStart3(Alpha value)
	{
		return AlphaHelper::Pow(value, 3);
	}

	FORCEINLINE Alpha SmoothStart4(Alpha value)
	{
		return AlphaHelper::Pow(value, 4);
	}

	FORCEINLINE Alpha SmoothStart5(Alpha value)
	{
		return AlphaHelper::Pow(value, 5);
	}

	FORCEINLINE Alpha SmoothEnd2(Alpha value)
	{
		return Alpha::One() - AlphaHelper::RemainingPow(value, 2);
	}

	FORCEINLINE Alpha SmoothEnd3(Alpha value)
	{
		return Alpha::One() - AlphaHelper::RemainingPow(value, 3);
	}

	FORCEINLINE Alpha SmoothEnd4(Alpha value)
	{
		return Alpha::One() - AlphaHelper::RemainingPow(value, 4);
	}

	FORCEINLINE Alpha SmoothEnd5(Alpha value)
	{
		return Alpha::One() - AlphaHelper::RemainingPow(value, 5);
	}

	using SmoothFuntion = Alpha (*)(Alpha);
	FORCEINLINE Alpha Mix(SmoothFuntion lhs, SmoothFuntion rhs, Alpha mixAlpha, Alpha value)
	{
		return (Alpha::One() - mixAlpha) * lhs(value) + mixAlpha * rhs(value);
	}

	// Don't exist Function SmmothStep4, because SmoothStep3's value is the same as SmoothStep4's value
	// Calculate (1-x)x^3 + (1-(1-x)^3)x and (1-x)x^2 + (1-(1-x)^2)x
	FORCEINLINE Alpha SmoothStep3(Alpha value)
	{
		return Mix(SmoothStart2, SmoothEnd2, value, value);
	}
	
	FORCEINLINE Alpha SmoothStep5(Alpha value)
	{
		return Mix(SmoothStart4, SmoothEnd4, value, value);
	}

	FORCEINLINE Alpha Hesitate3(Alpha value)
	{
		return Mix(SmoothEnd2, SmoothStart2, value, value);
	}
	
	FORCEINLINE Alpha Hesitate5(Alpha value)
	{
		return Mix(SmoothEnd4, SmoothStart4, value, value);
	}

	// Mix function even can generate smooth function used fraction, for example 2.2
	FORCEINLINE Alpha SmoothStart2_2(Alpha value)
	{
		return Mix(SmoothStart2, SmoothStart3, Alpha(0.2f), value);
	}

	FORCEINLINE Alpha SmoothEnd2_2(Alpha value)
	{
		return Mix(SmoothEnd2, SmoothEnd3, Alpha(0.2f), value);
	}
}

