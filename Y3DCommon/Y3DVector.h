#pragma once

#include <xtr1common>
#include "Y3DTypes.h"

namespace Y3D
{
	//template declaration 
	template <class T, U32 Count>
	class Y3DVector;

	//////////////////////////////////////////////////////////////////////////
	//
	//template partial specialization Y3DVector 1
	//
	//////////////////////////////////////////////////////////////////////////

	template <class T>
	class Y3DVector<T, 1>
	{
		static constexpr U32 Count = 1;
		static Y3DVector const IDENTITY;

	public: //	Constructions and Destructions

		constexpr Y3DVector() = default;
		constexpr explicit Y3DVector(T & m) : x(m) {}
		constexpr Y3DVector(T const& _x) : x(_x) {}
		constexpr Y3DVector(Y3DVector const& rhs) : x(rhs.x) {}

	public: // Attributes

		union 
		{
			T v[Count];
			struct 
			{
				T x;
			};
		};

	public: // Operations

		constexpr Y3DVector& operator = (Y3DVector const& rhs)
		{ 
			x = rhs.x; 
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator += (Y3DVector<T, Count> const& rhs)
		{
			x += rhs.x;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator -= (Y3DVector<T, Count> const& rhs)
		{
			x -= rhs.x;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator *= (T const& rhs)
		{
			x *= rhs;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator *= (Y3DVector<T, Count> const& rhs)
		{
			x *= rhs.x;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator /= (T const& rhs)
		{
			x /= rhs;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator /= (Y3DVector<T, Count> const& rhs)
		{
			x /= rhs.x;
			return *this;
		}

		constexpr T const& operator [] (U32 index) const
		{ 
			assert(index < Count);
			return v[index];
		}

		constexpr T& operator [] (U32 index)
		{ 
			assert(index < Count);
			return v[index]; 
		}

		constexpr Y3DVector operator - () const
		{ 
			return Y3DVector(-x);
		}

		constexpr T Magnitude() const 
		{ 
			static_assert(std::is_floating_point_v<T>, "Normalized() for floating point types only."); 
			return std::sqrt(MagnitudeSquared());
		}

		constexpr T MagnitudeSquared() const 
		{ 
			return Dot(*this, *this); 
		}

		constexpr T const* GetData() const
		{ 
			return v; 
		}

		constexpr void Normalize() const
		{
			static_assert(std::is_floating_point_v<T>, "Normalized() for floating point types only.");
			T reciprocal = 1.f / rhs;
			lhs.x *= reciprocal;
		}

		constexpr void Clamp()
		{
			for (int i = 0; i < 1; i++)
			{
				if (v[i] > 1.f - 1e-5f)
				{
					m[i] = 1.f;
					break;
				}
				else if (v[i] < -1.f + 1e-5f)
				{
					m[i] = -1.f;
					break;
				}
			}
		}
	};

	template <class T>
	Y3DVector<T, 1> const Y3DVector<T, 1>::IDENTITY = Y3DVector(static_cast<T>(0));

	template <class T>
	constexpr Y3DVector<T, 1> operator + (Y3DVector<T, 1> const& lhs, Y3DVector<T, 1> const& rhs)
	{ 
		return Y3DVector<T, 1>(lhs.x + rhs.x);
	}

	template <class T>
	constexpr Y3DVector<T, 1> operator - (Y3DVector<T, 1> const& lhs, Y3DVector<T, 1> const& rhs)
	{ 
		return Y3DVector<T, 1>(lhs.x - rhs.x);
	}

	// Convention that multiply by vector is one by one
	template <class T>
	constexpr Y3DVector<T, 1> operator * (Y3DVector<T, 1> const& llhs, Y3DVector<T, 1> const& rhs)
	{ 
		return Y3DVector<T, 1>(lhs.x * rhs.x);
	}

	template <class T>
	constexpr Y3DVector<T, 1> operator * (Y3DVector<T, 1> const& lhs, T const& rhs)
	{
		return Y3DVector<T, 1>(lhs.x * rhs);
	}

	template <class T>
	constexpr Y3DVector<T, 1> operator * (T const& lhs, Y3DVector<T, 1> const& rhs)
	{
		return Y3DVector<T, 1>(lhs * rhs.x);
	}
	
	// Convention that divide by vector is one by one
	template <class T>
	constexpr Y3DVector<T, 1> operator / (Y3DVector<T, 1> const& lhs, Y3DVector<T, 1> const& rhs)
	{
		return Y3DVector<T, 1>(lhs.x / rhs.x);
	}

	template <class T>
	constexpr Y3DVector<T, 1> operator / (Y3DVector<T, 1> const& lhs, T const& rhs)
	{
		T reciprocal = 1.f / rhs;
		return Y3DVector<T, 1>(lhs.x * reciprocal);
	}

	template <class T>
	constexpr bool operator == (Y3DVector<T, 1> const& lhs, Y3DVector<T, 1> const& rhs)
	{ 
		return (lhs.x == rhs.x);
	}

	template <class T>
	constexpr bool operator != (Y3DVector<T, 1> const& lhs, Y3DVector<T, 1> const& rhs)
	{ 
		return (lhs.x != rhs.x); 
	}

	template <class T>
	constexpr T Dot(Y3DVector<T, 1> const& lhs, Y3DVector<T, 1> const& rhs)
	{ 
		return (lhs.x * rhs.x); 
	}

	template <class T>
	constexpr Y3DVector<T, 1> Lerp(Y3DVector<T, 1> const& lhs, Y3DVector<T, 1> const& rhs, float fraction)
	{
		return Y3DVector<T, 1>(lhs.x * (1 - fraction) + rhs.x * fraction);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//template partial specialization Y3DVector 2
	//
	//////////////////////////////////////////////////////////////////////////

	template <class T>
	class Y3DVector<T, 2>
	{
		static Y3DVector const IDENTITY;
		static constexpr U32 Count = 2;

	public: //	Constructions and Destructions

		constexpr Y3DVector() = default;
		constexpr explicit Y3DVector(T & m) : x(m), y(m) {}
		constexpr Y3DVector(T const& _x, T const& _y) : x(_x), y(_y) {}
		constexpr Y3DVector(Y3DVector const& rhs) : x(rhs.x), y(rhs.y) {}

	public: // Attributes

		union
		{
			T v[Count];
			struct
			{
				T x, y;
			};
		};

	public: // Operations

		constexpr Y3DVector& operator = (Y3DVector const& rhs)
		{
			x = rhs.x; y = rhs.y;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator += (Y3DVector<T, Count> const& rhs)
		{
			x += rhs.x; y += rhs.y;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator -= (Y3DVector<T, Count> const& rhs)
		{
			x -= rhs.x; y -= rhs.y;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator *= (T const& rhs)
		{
			x *= rhs; y *= rhs;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator *= (Y3DVector<T, Count> const& rhs)
		{
			x *= rhs.x; y *= rhs.y;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator /= (T const& rhs)
		{
			T reciprocal = 1.f / rhs;
			x *= reciprocal; y *= reciprocal;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator /= (Y3DVector<T, Count> const& rhs)
		{
			x /= rhs.x; y /= rhs.y;
			return *this;
		}

		constexpr T const& operator [] (U32 index) const
		{
			assert(index < Count);
			return v[index];
		}

		constexpr T& operator [] (U32 index)
		{
			assert(index < Count);
			return v[index];
		}

		constexpr Y3DVector operator - () const
		{
			return Y3DVector(-x);
		}

		constexpr T Magnitude() const
		{
			static_assert(std::is_floating_point_v<T>, "Normalized() for floating point types only.");
			return std::sqrt(MagnitudeSquared());
		}

		constexpr T MagnitudeSquared() const
		{
			return Dot(*this, *this);
		}

		constexpr T const* GetData() const
		{
			return v;
		}

		constexpr void Normalize() const
		{
			static_assert(std::is_floating_point_v<T>, "Normalized() for floating point types only.");
			T reciprocal = 1.f / rhs;
			lhs.x *= reciprocal; lhs.y *= reciprocal;
		}

		constexpr void Clamp()
		{
			for (int i = 0; i < Count; i++)
			{
				if (v[i] > 1.f - 1e-5f)
				{
					m[i] = 1.f;
					break;
				}
				else if (v[i] < -1.f + 1e-5f)
				{
					m[i] = -1.f;
					break;
				}
			}
		}
	};

	template <class T>
	Y3DVector<T, 2> const Y3DVector<T, 2>::IDENTITY = Y3DVector(static_cast<T>(0), static_cast<T>(0));

	template <class T>
	constexpr Y3DVector<T, 2> operator + (Y3DVector<T, 2> const& lhs, Y3DVector<T, 2> const& rhs)
	{
		return Y3DVector<T, 2>(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	template <class T>
	constexpr Y3DVector<T, 2> operator - (Y3DVector<T, 2> const& lhs, Y3DVector<T, 2> const& rhs)
	{
		return Y3DVector<T, 2>(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	// Convention that multiply by vector is one by one
	template <class T>
	constexpr Y3DVector<T, 2> operator * (Y3DVector<T, 2> const& llhs, Y3DVector<T, 2> const& rhs)
	{
		return Y3DVector<T, 2>(lhs.x * rhs.x, llhs.y * rhs.y);
	}

	template <class T>
	constexpr Y3DVector<T, 2> operator * (Y3DVector<T, 2> const& lhs, T const& rhs)
	{
		return Y3DVector<T, 2>(lhs.x * rhs, lhs.y * rhs);
	}

	template <class T>
	constexpr Y3DVector<T, 2> operator * (T const& lhs, Y3DVector<T, 2> const& rhs)
	{
		return Y3DVector<T, 2>(lhs * rhs.x, lhs * rhs.y);
	}

	// Convention that divide by vector is one by one
	template <class T>
	constexpr Y3DVector<T, 2> operator / (Y3DVector<T, 2> const& lhs, Y3DVector<T, 2> const& rhs)
	{
		return Y3DVector<T, 2>(lhs.x / rhs.x, lhs.y / rhs.y);
	}

	template <class T>
	constexpr Y3DVector<T, 2> operator / (Y3DVector<T, 2> const& lhs, T const& rhs)
	{
		T reciprocal = 1.f / rhs;
		return Y3DVector<T, 2>(lhs.x * reciprocal, lhs.y * reciprocal);
	}

	template <class T>
	constexpr bool operator == (Y3DVector<T, 2> const& lhs, Y3DVector<T, 2> const& rhs)
	{
		return (lhs.x == rhs.x && lhs.y == rhs.y);
	}

	template <class T>
	constexpr bool operator != (Y3DVector<T, 2> const& lhs, Y3DVector<T, 2> const& rhs)
	{
		return (lhs.x != rhs.x || lhs.y != rhs.y);
	}

	template <class T>
	constexpr T Dot(Y3DVector<T, 2> const& lhs, Y3DVector<T, 2> const& rhs)
	{
		return (lhs.x * rhs.x + lhs.y * rhs.y);
	}

	template <class T>
	constexpr Y3DVector<T, 2> Lerp(Y3DVector<T, 2> const& lhs, Y3DVector<T, 2> const& rhs, float fraction)
	{
		return Y3DVector<T, 2>(lhs.x * (1 - fraction) + rhs.x * fraction,
			lhs.y * (1 - fraction) + rhs.y * fraction);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//template partial specialization Y3DVector 3
	//
	//////////////////////////////////////////////////////////////////////////

	template <class T>
	class Y3DVector<T, 3>
	{
		static Y3DVector const IDENTITY;
		static constexpr U32 Count = 3;

	public: //	Constructions and Destructions

		constexpr Y3DVector() = default;
		constexpr explicit Y3DVector(T & m) : x(m), y(m), z(m) {}
		constexpr Y3DVector(T const& _x, T const& _y, T const& _z) : x(_x), y(_y), z(_z) {}
		constexpr Y3DVector(Y3DVector const& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}

	public: // Attributes

		union
		{
			T v[Count];
			struct
			{
				T x, y, z;
			};
		};

	public: // Operations

		constexpr Y3DVector& operator = (Y3DVector const& rhs)
		{
			x = rhs.x; y = rhs.y; z = rhs.z;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator += (Y3DVector<T, Count> const& rhs)
		{
			x += rhs.x; y += rhs.y; z += rhs.z;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator -= (Y3DVector<T, Count> const& rhs)
		{
			x -= rhs.x; y -= rhs.y; z -= rhs.z;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator *= (T const& rhs)
		{
			x *= rhs; y *= rhs; z *= rhs;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator *= (Y3DVector<T, Count> const& rhs)
		{
			x *= rhs.x; y *= rhs.y; z *= rhs.z;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator /= (T const& rhs)
		{
			T reciprocal = 1.f / rhs;
			x *= reciprocal; y *= reciprocal; z *= reciprocal;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator /= (Y3DVector<T, Count> const& rhs)
		{
			x /= rhs.x; y /= rhs.y; z /= rhs.z;
			return *this;
		}

		constexpr T const& operator [] (U32 index) const
		{
			assert(index < Count);
			return v[index];
		}

		constexpr T& operator [] (U32 index)
		{
			assert(index < Count);
			return v[index];
		}

		constexpr Y3DVector operator - () const
		{
			return Y3DVector(-x);
		}

		constexpr T Magnitude() const
		{
			static_assert(std::is_floating_point_v<T>, "Normalized() for floating point types only.");
			return std::sqrt(MagnitudeSquared());
		}

		constexpr T MagnitudeSquared() const
		{
			return Dot(*this, *this);
		}

		constexpr T const* GetData() const
		{
			return v;
		}

		constexpr void Normalize() const
		{
			static_assert(std::is_floating_point_v<T>, "Normalized() for floating point types only.");
			T reciprocal = 1.f / rhs;
			lhs.x *= reciprocal; lhs.y *= reciprocal; lhs.z *= reciprocal;
		}

		constexpr void Clamp()
		{
			for (int i = 0; i < Count; i++)
			{
				if (v[i] > 1.f - 1e-5f)
				{
					m[i] = 1.f;
					break;
				}
				else if (v[i] < -1.f + 1e-5f)
				{
					m[i] = -1.f;
					break;
				}
			}
		}
	};

	template <class T>
	Y3DVector<T, 3> const Y3DVector<T, 3>::IDENTITY = Y3DVector(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0));

	template <class T>
	constexpr Y3DVector<T, 3> operator + (Y3DVector<T, 3> const& lhs, Y3DVector<T, 3> const& rhs)
	{
		return Y3DVector<T, 3>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	template <class T>
	constexpr Y3DVector<T, 3> operator - (Y3DVector<T, 3> const& lhs, Y3DVector<T, 3> const& rhs)
	{
		return Y3DVector<T, 3>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	// Convention that multiply by vector is one by one
	template <class T>
	constexpr Y3DVector<T, 3> operator * (Y3DVector<T, 3> const& llhs, Y3DVector<T, 3> const& rhs)
	{
		return Y3DVector<T, 3>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
	}

	template <class T>
	constexpr Y3DVector<T, 3> operator * (Y3DVector<T, 3> const& lhs, T const& rhs)
	{
		return Y3DVector<T, 3>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
	}

	template <class T>
	constexpr Y3DVector<T, 3> operator * (T const& lhs, Y3DVector<T, 3> const& rhs)
	{
		return Y3DVector<T, 3>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
	}

	// Convention that divide by vector is one by one
	template <class T>
	constexpr Y3DVector<T, 3> operator / (Y3DVector<T, 3> const& lhs, Y3DVector<T, 3> const& rhs)
	{
		return Y3DVector<T, 3>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
	}

	template <class T>
	constexpr Y3DVector<T, 3> operator / (Y3DVector<T, 3> const& lhs, T const& rhs)
	{
		T reciprocal = 1.f / rhs;
		return Y3DVector<T, 3>(lhs.x * reciprocal, lhs.y * reciprocal, lhs.z * reciprocal);
	}

	template <class T>
	constexpr bool operator == (Y3DVector<T, 3> const& lhs, Y3DVector<T, 3> const& rhs)
	{
		return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z);
	}

	template <class T>
	constexpr bool operator != (Y3DVector<T, 3> const& lhs, Y3DVector<T, 3> const& rhs)
	{
		return (lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z);
	}

	template <class T>
	constexpr T Dot(Y3DVector<T, 3> const& lhs, Y3DVector<T, 3> const& rhs)
	{
		return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z);
	}

	template <class T>
	constexpr Y3DVector<T, 3> Lerp(Y3DVector<T, 3> const& lhs, Y3DVector<T, 3> const& rhs, float fraction)
	{
		return Y3DVector<T, 3>(lhs.x * (1 - fraction) + rhs.x * fraction,
			lhs.y * (1 - fraction) + rhs.y * fraction,
			lhs.z * (1 - fraction) + rhs.z * fraction);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//template partial specialization Y3DVector 4
	//
	//////////////////////////////////////////////////////////////////////////

	template <class T>
	class Y3DVector<T, 4>
	{
		static Y3DVector const IDENTITY;
		static constexpr U32 Count = 4;

	public: //	Constructions and Destructions

		constexpr Y3DVector() = default;
		constexpr explicit Y3DVector(T & m) : x(m), y(m), z(m), w(m) {}
		constexpr Y3DVector(T const& _x, T const& _y, T const& _z, T const& _w) : x(_x), y(_y), z(_z), w(_w) {}
		constexpr Y3DVector(Y3DVector const& rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) {}

	public: // Attributes

		union
		{
			T v[Count];
			struct
			{
				T x, y, z, w;
			};
		};

	public: // Operations

		constexpr Y3DVector& operator = (Y3DVector const& rhs)
		{
			x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator += (Y3DVector<T, Count> const& rhs)
		{
			x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator -= (Y3DVector<T, Count> const& rhs)
		{
			x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator *= (T const& rhs)
		{
			x *= rhs; y *= rhs; z *= rhs; w *= rhs;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator *= (Y3DVector<T, Count> const& rhs)
		{
			x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator /= (T const& rhs)
		{
			T reciprocal = 1.f / rhs;
			x *= reciprocal; y *= reciprocal; z *= reciprocal; w *= reciprocal;
			return *this;
		}

		constexpr Y3DVector<T, Count> const& operator /= (Y3DVector<T, Count> const& rhs)
		{
			x /= rhs.x; y /= rhs.y; z /= rhs.z; w /= rhs.w;
			return *this;
		}

		constexpr T const& operator [] (U32 index) const
		{
			assert(index < Count);
			return v[index];
		}

		constexpr T& operator [] (U32 index)
		{
			assert(index < Count);
			return v[index];
		}

		constexpr Y3DVector operator - () const
		{
			return Y3DVector(-x);
		}

		constexpr T Magnitude() const
		{
			static_assert(std::is_floating_point_v<T>, "Normalized() for floating point types only.");
			return std::sqrt(MagnitudeSquared());
		}

		constexpr T MagnitudeSquared() const
		{
			return Dot(*this, *this);
		}

		constexpr T const* GetData() const
		{
			return v;
		}

		constexpr void Normalize() const
		{
			static_assert(std::is_floating_point_v<T>, "Normalized() for floating point types only.");
			T reciprocal = 1.f / rhs;
			lhs.x *= reciprocal; lhs.y *= reciprocal; lhs.z *= reciprocal; lhs.z *= reciprocal;
		}

		constexpr void Clamp()
		{
			for (int i = 0; i < Count; i++)
			{
				if (v[i] > 1.f - 1e-5f)
				{
					m[i] = 1.f;
					break;
				}
				else if (v[i] < -1.f + 1e-5f)
				{
					m[i] = -1.f;
					break;
				}
			}
		}
	};

	template <class T>
	Y3DVector<T, 4> const Y3DVector<T, 4>::IDENTITY = Y3DVector(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0));

	template <class T>
	constexpr Y3DVector<T, 4> operator + (Y3DVector<T, 4> const& lhs, Y3DVector<T, 4> const& rhs)
	{
		return Y3DVector<T, 4>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
	}

	template <class T>
	constexpr Y3DVector<T, 4> operator - (Y3DVector<T, 4> const& lhs, Y3DVector<T, 4> const& rhs)
	{
		return Y3DVector<T, 4>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
	}

	// Convention that multiply by vector is one by one
	template <class T>
	constexpr Y3DVector<T, 4> operator * (Y3DVector<T, 4> const& llhs, Y3DVector<T, 4> const& rhs)
	{
		return Y3DVector<T, 4>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
	}

	template <class T>
	constexpr Y3DVector<T, 4> operator * (Y3DVector<T, 4> const& lhs, T const& rhs)
	{
		return Y3DVector<T, 4>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
	}

	template <class T>
	constexpr Y3DVector<T, 4> operator * (T const& lhs, Y3DVector<T, 4> const& rhs)
	{
		return Y3DVector<T, 4>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
	}

	// Convention that divide by vector is one by one
	template <class T>
	constexpr Y3DVector<T, 4> operator / (Y3DVector<T, 4> const& lhs, Y3DVector<T, 4> const& rhs)
	{
		return Y3DVector<T, 4>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
	}

	template <class T>
	constexpr Y3DVector<T, 4> operator / (Y3DVector<T, 4> const& lhs, T const& rhs)
	{
		T reciprocal = 1.f / rhs;
		return Y3DVector<T, 4>(lhs.x * reciprocal, lhs.y * reciprocal, lhs.z * reciprocal, lhs.w * reciprocal);
	}

	template <class T>
	constexpr bool operator == (Y3DVector<T, 4> const& lhs, Y3DVector<T, 4> const& rhs)
	{
		return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w);
	}

	template <class T>
	constexpr bool operator != (Y3DVector<T, 4> const& lhs, Y3DVector<T, 4> const& rhs)
	{
		return (lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z || lhs.w != rhs.w);
	}

	template <class T>
	constexpr T Dot(Y3DVector<T, 4> const& lhs, Y3DVector<T, 4> const& rhs)
	{
		return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w);
	}

	template <class T>
	constexpr Y3DVector<T, 4> Lerp(Y3DVector<T, 4> const& lhs, Y3DVector<T, 4> const& rhs, float fraction)
	{
		return Y3DVector<T, 4>(lhs.x * (1 - fraction) + rhs.x * fraction,
			lhs.y * (1 - fraction) + rhs.y * fraction,
			lhs.z * (1 - fraction) + rhs.z * fraction,
			lhs.w * (1 - fraction) + rhs.w * fraction);
	}
}