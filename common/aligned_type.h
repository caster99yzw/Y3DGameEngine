#pragma once
#include "common/types.h"
#include "portable.h"

/**
 * Aligns a value to the nearest higher multiple of 'alignment', which must be a power of two.
 */
template <typename T>
FORCEINLINE constexpr T Align(T value, uint64_t alignment)
{
	static_assert(std::is_integral<T>::value || std::is_pointer<T>::value, "Align expects an integer or pointer type");

	return (T)(((uint64_t)value + alignment - 1) & ~(alignment - 1));
}

FORCEINLINE bool CheckAlignmentValid(uint64_t alignment, void const* memory)
{
	return (reinterpret_cast<uint64_t>(memory) & (alignment - 1)) == 0;
}

FORCEINLINE bool CheckAlignmentValid(uint64_t alignment, void* memory)
{
	return (reinterpret_cast<uint64_t>(memory) & (alignment - 1)) == 0;
}

template <typename T>
FORCEINLINE bool CheckAlignmentValid(void* memory)
{
	return CheckAlignmentValid(alignof(T), memory);
}

template <typename T>
FORCEINLINE bool CheckAlignmentValid(void const* memory)
{
	return CheckAlignmentValid(alignof(T), memory);
}

/**
* Used to declare an untyped array of data with compile-time alignment
*/
template <uint32_t Size, uint32_t Alignment>
struct AlignedBytes;

template <uint32_t Size>
struct AlignedBytes<Size, 1>
{
	struct alignas(1) Padding
	{
		uint8_t Pad[Size];
	};
	Padding Pad;
	void* Memory() { return Pad.Pad; }
	void const* Memory() const { return Pad.Pad; }
};

/**
 * A macro that implements AlignedBytes for a specific alignment
 */
#define IMPLEMENT_ALIGED_STORAGE(Alignment) \
template<uint32_t Size> \
struct AlignedBytes<Size, Alignment> \
{ \
	struct alignas(Alignment) Padding \
	{ \
		uint8_t Pad[Size]; \
	}; \
	Padding Pad; \
	void* Memory() { return Pad.Pad; } \
	void const* Memory() const { return Pad.Pad; } \
};

 /**
 * Implement AlignedBytes for 16, 8, 4, 2 alignments
 */
IMPLEMENT_ALIGED_STORAGE(16);
IMPLEMENT_ALIGED_STORAGE(8);
IMPLEMENT_ALIGED_STORAGE(4);
IMPLEMENT_ALIGED_STORAGE(2);

#undef IMPLEMENT_ALIGED_STORAGE

/**
 * An untyped array of data with compile-time alignment and size derived from another type
 */
template<typename ElementType>
struct TypeAlignedBytes : public AlignedBytes<sizeof(ElementType), alignof(ElementType)>
{
	using Base = AlignedBytes<sizeof(ElementType), alignof(ElementType)>;

	ElementType& TypedObject() { return *static_cast<ElementType*>(Base::Memory()); }
	ElementType const& TypedObject() const { return *static_cast<ElementType const*>(Base::Memory()); }

	template <typename... Parameters>
	void ConstructObject(Parameters&&... parameters)
	{
		new (Base::Memory()) ElementType(std::forward<Parameters>(parameters)...);
	}

	void DestructObject()
	{
		static_cast<ElementType*>(Base::Memory())->~ElementType();
	}
};


