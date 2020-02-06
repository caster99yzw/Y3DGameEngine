#pragma once
#include "Types.h"

namespace y3dcommon
{
	namespace BitOperationImpl
	{
		//////////////////////////////////////////////////////////////////////////
		// Funtion CountSetImpl depends on Hamming Weight
		//////////////////////////////////////////////////////////////////////////

		inline uint8 CountSetImpl(uint8 value)
		{
			value = value - ((value >> 1) & 0x55);
			value = (value & 0x33) + ((value >> 2) & 0x33);
			return ((value + (value >> 4)) & 0x0F);
		}

		inline uint16 CountSetImpl(uint16 value)
		{
			value = value - ((value >> 1) & 0x5555);
			value = (value & 0x3333) + ((value >> 2) & 0x3333);
			return uint16_t(((value + (value >> 4)) & 0x0F0F) * 0x0101) >> 8;
		}

		inline uint32 CountSetImpl(uint32 value)
		{
			value = value - ((value >> 1) & 0x55555555);
			value = (value & 0x33333333) + ((value >> 2) & 0x33333333);
			return (((value + (value >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
		}

		inline uint64 CountSetImpl(uint64 value)
		{
			value = value - ((value >> 1) & 0x5555555555555555ull);
			value = (value & 0x3333333333333333ull) + ((value >> 2) & 0x3333333333333333ull);
			return (((value + (value >> 4)) & 0x0F0F0F0F0F0F0F0Full) * 0x0101010101010101ull) >> 56;
		}
	}

	struct BitContainerIndex
	{
		BitContainerIndex(uint32 inBitIndex)
			: bitOffset(inBitIndex / 32)
			, bitMask(1 << (32 - (inBitIndex % 32))) {}

		uint32 bitOffset;
		uint32 bitMask;
	};

	class BitContainer
	{
	public:
		BitContainer() 
			: size(0)
			, data(nullptr) {}

		BitContainer(uint32 size) 
			: size(size)
			, data(new uint32[size]) {}
		
		~BitContainer()
		{
			delete[] data;
		}

		uint32 Size() const { return size; }
		uint32 BitCount() const { return size * 32; }

		bool Valid(BitContainerIndex bitIndex)
		{
			return bitIndex.bitOffset < Size();
		}

		bool Test(BitContainerIndex bitIndex)
		{
			assert(bitIndex.bitOffset < Size());
			return (data[bitIndex.bitOffset] & bitIndex.bitMask) != 0;
		}

		bool All()
		{
			for (uint32 i = 0; i < size; ++i)
			{
				if (data[i] != 0xFFFFFFFF)
					return false;
			}
			return true;
		}

		bool None()
		{
			for (uint32 i = 0; i < size; ++i)
			{
				if (data[i] != 0x00000000)
					return false;
			}
			return true;
		}

		void Set()
		{
			for (uint32 i = 0; i < size; ++i)
				data[i] = 0xFFFFFFFF;
		}

		void Reset()
		{
			for (uint32 i = 0; i < size; ++i)
				data[i] = 0x00000000;
		}

		void Filp()
		{
			for (uint32 i = 0; i < size; ++i)
				data[i] = ~data[i];
		}

		void Set(BitContainerIndex bitIndex)
		{
			assert(bitIndex.bitOffset < Size());
			data[bitIndex.bitOffset] |= bitIndex.bitMask;
		}

		void Reset(BitContainerIndex bitIndex)
		{
			assert(bitIndex.bitOffset < Size());
			data[bitIndex.bitOffset] &= ~bitIndex.bitMask;
		}

		void Filp(BitContainerIndex bitIndex)
		{
			assert(bitIndex.bitOffset < Size());
			data[bitIndex.bitOffset] ^= bitIndex.bitMask;
		}

		uint32 SetCount()
		{
			uint32 count = 0;
			for (uint32 i = 0; i < Size(); ++i)
				count += BitOperationImpl::CountSetImpl(data[i]);
			return count;
		}

		uint32 ResetCount()
		{
			return BitCount() - SetCount();
		}

	private:
		uint32 size;
		uint32 *data = nullptr;
	};

	template <uint32 bitCount>
	inline constexpr BitContainer MakeBitContainer()
	{
		static_assert(bitCount <= std::numeric_limits<uint32>::max() - 32, "Number of bits exceeds the maximum number allowed");
		return BitContainer((bitCount + 32 - 1) / 32);
	};

	inline BitContainer MakeBitContainer(uint32 bitCount)
	{
		assert(bitCount <= std::numeric_limits<uint32>::max() - 32);
		return BitContainer((bitCount + 32 - 1) / 32);
	};
}

