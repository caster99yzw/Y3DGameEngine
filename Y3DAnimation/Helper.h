#pragma once
#include "Y3DTypes.h"
#include "Y3DMathFunc.h"
using namespace Y3D;

typedef FLOAT32		AnimTimeType;
typedef UINT32		KeyHandle;

KeyHandle const InvalidHandle = KeyHandle(0x20000000);

namespace Y3D
{
	template <typename T1, typename T2>
	struct Pair
	{
	public:
		
		Pair() = default;
		Pair(T1 const& _1, T2 const& _2) : e1(_1), e2(_2) {}
		Pair(T1 && _1, T2 && _2) : e1(std::move(_1)), e2(std::move(_2)) {}

	public:
		T1 e1;
		T2 e2;
	};

	template<typename T>
	static const bool FloatIsZero(T f) { return 1e-7f > f && f > -1e-7f; }
}
