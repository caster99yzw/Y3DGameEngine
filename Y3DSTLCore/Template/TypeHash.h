#pragma once
#include "Template/IsPODType.h"
#include "Template/EnableIf.h"
#include "CoreTypes.h"

/**
* Combines two hash values to get a third.
* Note - this function is not commutative.
*/
inline UINT32 HashCombine(UINT32 A, UINT32 C)
{
	UINT32 B = 0x9e3779b9;
	A += B;

	A -= B; A -= C; A ^= (C >> 13);
	B -= C; B -= A; B ^= (A << 8);
	C -= A; C -= B; C ^= (B >> 13);
	A -= B; A -= C; A ^= (C >> 12);
	B -= C; B -= A; B ^= (A << 16);
	C -= A; C -= B; C ^= (B >> 5);
	A -= B; A -= C; A ^= (C >> 3);
	B -= C; B -= A; B ^= (A << 10);
	C -= A; C -= B; C ^= (B >> 15);

	return C;
}

//FORCEINLINE UINT32 PointerHash(const void* Key, UINT32 C = 0)
//{
//	auto PtrInt = reinterpret_cast<UINT32>(Key);
//
//	return HashCombine(PtrInt, C);
//}

inline UINT32 GetTypeHash(UINT8 const A)
{
	return A;
}

inline UINT32 GetTypeHash(INT8 const A)
{
	return A;
}

inline UINT32 GetTypeHash(UINT16 const A)
{
	return A;
}

inline UINT32 GetTypeHash(INT16 const A)
{
	return A;
}

inline UINT32 GetTypeHash(UINT32 const A)
{
	return A;
}

inline UINT32 GetTypeHash(UINT64 const A)
{
	return (UINT32)A + ((UINT32)(A >> 32) * 23);
}

inline UINT32 GetTypeHash(INT64 const A)
{
	return (UINT32)A + ((UINT32)(A >> 32) * 23);
}

inline UINT32 GetTypeHash(FLOAT32 Value)
{
	return GetTypeHash(*(UINT32*)&Value);
}

inline UINT32 GetTypeHash(FLOAT64 Value)
{
	return GetTypeHash(*(UINT64*)&Value);
}

inline UINT32 GetTypeHash(ANSICHAR const* S)
{
	UINT64 H = 0;
	for (; *S; ++S)
		H = 5 * H + *S;

	return GetTypeHash(H);
}

//FORCEINLINE UINT32 GetTypeHash(void const* A)
//{
//	return PointerHash(A);
//}
//
//FORCEINLINE UINT32 GetTypeHash(void* A)
//{
//	return PointerHash(A);
//}

template <typename EnumType>
FORCEINLINE typename EnableIf<IsEnum<EnumType>::Value, UINT32>::Type GetTypeHash(EnumType E)
{
	return GetTypeHash((__underlying_type(EnumType))E);
}

//namespace UE4TypeHash_Private
//{
//	template <uint32 ArgToCombine, uint32 ArgCount>
//	struct TGetTupleHashHelper
//	{
//		template <typename TupleType>
//		FORCEINLINE static uint32 Do(uint32 Hash, const TupleType& Tuple)
//		{
//			return TGetTupleHashHelper<ArgToCombine + 1, ArgCount>::Do(HashCombine(Hash, GetTypeHash(Tuple.template Get<ArgToCombine>())), Tuple);
//		}
//	};
//
//	template <uint32 ArgIndex>
//	struct TGetTupleHashHelper<ArgIndex, ArgIndex>
//	{
//		template <typename TupleType>
//		FORCEINLINE static uint32 Do(uint32 Hash, const TupleType& Tuple)
//		{
//			return Hash;
//		}
//	};
//}
//
//template <typename... Types>
//FORCEINLINE uint32 GetTypeHash(const TTuple<Types...>& Tuple)
//{
//	return UE4TypeHash_Private::TGetTupleHashHelper<1u, sizeof...(Types)>::Do(GetTypeHash(Tuple.template Get<0>()), Tuple);
//}
//
//FORCEINLINE uint32 GetTypeHash(const TTuple<>& Tuple)
//{
//	return 0;
//}
