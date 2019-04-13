#pragma once
#include "EastingFunction.h"

namespace y3dmath
{
	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	struct LinearControlPoint
	{
		T a;
		T b;
	};

	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	struct QuadraticControlPoint
	{
		T a;
		T b;
		T c;
	};

	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	FORCEINLINE QuadraticControlPoint<T> Linear2Quadratic(LinearControlPoint<T> cp)
	{
		QuadraticControlPoint<T> result;
		result.a = cp.a;
		result.b = cp.a * 1 / 2 + cp.b * 1 / 2;
		result.c = cp.b;
	}

	template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	FORCEINLINE T Lerp(T lhs, T rhs, Alpha alpha)
	{
		return alpha.RemainingValue() * lhs + alpha.Value() * rhs;
	}

	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	FORCEINLINE T LinearBezier(LinearControlPoint<T> cp, Alpha alpha)
	{
		return Lerp(cp.a, cp.b, alpha);
	}

	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	FORCEINLINE T QuadraticBezier(QuadraticControlPoint<T> cp, Alpha alpha)
	{
		T ab = Lerp(cp.a, cp.b, alpha);
		T bc = Lerp(b, cp.c, alpha);
		return Lerp(ab, bc, alpha);
	}

	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	FORCEINLINE T QuadraticBezie2(QuadraticControlPoint<T> cp, Alpha alpha)
	{
		float32 s = alpha.RemainingValue();
		float32 t = alpha.Value();
		return s * s * cp.a + 2 * s * t * cp.b + t * t * cp.c;
	}


	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	struct BezierControlPoint
	{
		T a;
		T b;
		T c;
		T d;
	};

	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	FORCEINLINE BezierControlPoint<T> Linear2Berizer(LinearControlPoint<T> cp)
	{
		BezierControlPoint<T> result;
		result.a = cp.a;
		result.b = cp.a * 2 / 3 + cp.b * 1 / 3;
		result.c = cp.a * 1 / 3 + cp.b * 2 / 3;
		result.d = cp.b;
	}

	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>

	FORCEINLINE BezierControlPoint<T> Quadratic2Berizer(QuadraticControlPoint<T> cp)
	{
		BezierControlPoint<T> result;
		result.a = cp.a;
		result.b = cp.a * 2 / 3 + cp.b * 1 / 3;
		result.c = cp.b * 1 / 3 + cp.c * 2 / 3;
		result.d = cp.c;
	}

	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	struct HermitControlPoint
	{
		T a;
		T u;
		T v;
		T d;
	};

	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	FORCEINLINE T CubicBezier(BezierControlPoint<T> cp, Alpha alpha)
	{
		T ab = Lerp(cp.a, cp.b, alpha);
		T bc = Lerp(cp.b, cp.c, alpha);
		T cd = Lerp(cp.c, cp.d, alpha);
		T abc = Lerp(ab, bc, alpha);
		T bcd = Lerp(bc, cd, alpha);
		return Lerp(abc, bcd, alpha);
	}

	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	FORCEINLINE T CubicBezier2(BezierControlPoint<T> cp, Alpha alpha)
	{
		float32 s = alpha.RemainingValue();
		float32 t = alpha.Value();
		return s * s * s * cp.a + 3 * s * s * t * cp.b + 3 * s * t * t * cp.c + t * t * t * cp.d;
	}

	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	FORCEINLINE T CubicHermit(HermitControlPoint<T> cp, Alpha alpha)
	{
		float32 s = alpha.RemainingValue();
		float32 t = alpha.Value();
		return s * s * (1 + 2 * t) * cp.a  + t * t * (1 + 2 * s) * cp.d + s * s * t * cp.u - s * t * t * cp.v;
	}

	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	FORCEINLINE BezierControlPoint<T> Hermit2Bezier(HermitControlPoint<T> cp)
	{
		BezierControlPoint<T> result;
		result.a = cp.a;
		result.b = cp.a + cp.u / 3;
		result.c = cp.d - cp.v / 3;
		result.d = cp.d;
	}

	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	FORCEINLINE HermitControlPoint<T> Bezier2Hermit(BezierControlPoint<T> cp)
	{
		HermitControlPoint<T> result;
		result.a = cp.a;
		result.u = 3 * (cp.b - cp.a);
		result.v = 3 * (cp.d - cp.c);
		result.d = cp.d;
	}

	// These function for test
	FORCEINLINE Alpha SmoothStart2_Slow(Alpha alpha)
	{
		// using normalized Bezier in [0,1]
		return Alpha(QuadraticBezier(QuadraticControlPoint<float32>{ 0.f, 0.f, 1.f }, alpha));
	}

	FORCEINLINE Alpha SmoothEnd2_Slow(Alpha alpha)
	{
		// using normalized Bezier in [0,1]
		return Alpha(QuadraticBezier(QuadraticControlPoint<float32>{ 0.f, 0.f, 1.f }, alpha));
	}

	FORCEINLINE Alpha SmoothStart3_Slow(Alpha alpha)
	{
		// using normalized Bezier in [0,1]
		return Alpha(CubicBezier(BezierControlPoint<float32>{ 0.f, 0.f, 0.f, 1.f }, alpha));
	}

	FORCEINLINE Alpha SmoothEnd3_Slow(Alpha alpha)
	{
		// using normalized Bezier in [0,1]
		return Alpha(CubicBezier(BezierControlPoint<float32>{ 0.f, 0.f, 0.f, 1.f }, alpha));
	}

	FORCEINLINE Alpha SmoothStep3_Slow(Alpha alpha)
	{
		// using normalized Bezier in [0,1]
		return Alpha(CubicBezier(BezierControlPoint<float32>{ 0.f, 0.f, 0.f, 1.f }, alpha));
	}

	FORCEINLINE Alpha Hesitate3_Slow(Alpha alpha)
	{
		// using normalized Bezier in [0,1]
		return Alpha(CubicBezier(BezierControlPoint<float32>{ 0.f, 0.f, 0.f, 1.f }, alpha));
	}
}


