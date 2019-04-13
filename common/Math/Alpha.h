#pragma once
#include "MathFunction.h"

namespace y3dmath
{
	class Alpha
	{
	public:
		explicit Alpha(float32 inNormalizedValue)
			: value(Clamp(inNormalizedValue, 0.f, 1.f))
		{}
		explicit Alpha(float32 inValue, float32 min, float32 max)
			: value(Ratio(inValue, min, max))
		{}

		static Alpha One() { return Alpha(1.f); }
		static Alpha Zero() { return Alpha(0.f); }

		float32 Value() const { return value; }
		float32 RemainingValue() const { return 1.f - value; }

		friend Alpha operator+(Alpha lhs, Alpha rhs) { return Alpha(lhs.value + rhs.value); }
		friend Alpha operator-(Alpha lhs, Alpha rhs) { return Alpha(lhs.value - rhs.value); }
		friend Alpha operator*(Alpha lhs, Alpha rhs) { return Alpha(lhs.value * rhs.value); }
		friend Alpha operator/(Alpha lhs, Alpha rhs) { return Alpha(lhs.value / rhs.value); }

	private:
		float32 value = 0.f;
	};

	// math for alpha
	struct AlphaHelper
	{
		static Alpha Pow(Alpha value, int32 exponent)
		{
			return Alpha(y3dmath::Pow<float32>(value.Value(), exponent));
		}

		static Alpha RemainingPow(Alpha value, int32 exponent)
		{
			return Alpha(y3dmath::Pow<float32>(value.RemainingValue(), exponent));
		}
	};
}

