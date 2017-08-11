#pragma once

#include <xtr1common>
#include "Y3DTypes.h"

namespace Y3D
{
	//Template declaration 
	template <class T, UINT32 Count>
	class Y3DVector;

	//////////////////////////////////////////////////////////////////////////
	//
	//Template partial specialization Y3DVector version 1
	//
	//////////////////////////////////////////////////////////////////////////

	template <class T>
	class Y3DVector<T, 1>
	{
	public: // Static and Macro

		static constexpr UINT32 Count = 1;
		static Y3DVector const IDENTITY;

	public: //	Constructions and Destructions

		constexpr Y3DVector() = default;
		constexpr Y3DVector(T const& _x) : v{ _x } {}
		constexpr Y3DVector(Y3DVector const& rhs) : v{ rhs.x } {}

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

		constexpr T const& operator [] (UINT32 index) const
		{ 
			assert(index < Count);
			return v[index];
		}

		constexpr T& operator [] (UINT32 index)
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

		constexpr void Normalize()
		{
			static_assert(std::is_floating_point_v<T>, "Normalized() for floating point types only.");
			T reciprocal = 1.f / Magnitude();
			x *= reciprocal;
		}

		constexpr void Clamp()
		{
			for (UINT32 i = 0; i < 1; i++)
			{
				if (v[i] > 1.f - 1e-5f)
				{
					v[i] = 1.f;
					continue;
				}
				else if (v[i] < -1.f + 1e-5f)
				{
					v[i] = -1.f;
					continue;
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
	constexpr Y3DVector<T, 1> operator * (Y3DVector<T, 1> const& lhs, Y3DVector<T, 1> const& rhs)
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
	constexpr Y3DVector<T, 1> Lerp(Y3DVector<T, 1> const& lhs, Y3DVector<T, 1> const& rhs, T const& fraction)
	{
		return Y3DVector<T, 1>(lhs.x * (1 - fraction) + rhs.x * fraction);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//Template partial specialization Y3DVector version 2
	//
	//////////////////////////////////////////////////////////////////////////

	template <class T>
	class Y3DVector<T, 2>
	{
	public: // Static and Macro

		static Y3DVector const IDENTITY;
		static constexpr UINT32 Count = 2;

	public: //	Constructions and Destructions

		constexpr Y3DVector() = default;
		constexpr explicit Y3DVector(T const& m) : v{ m, m } {}
		constexpr Y3DVector(T const& _x, T const& _y) : v{ _x, _y } {}
		constexpr Y3DVector(Y3DVector const& rhs) : v{ rhs.x, rhs.y } {}

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

		constexpr T const& operator [] (UINT32 index) const
		{
			assert(index < Count);
			return v[index];
		}

		constexpr T& operator [] (UINT32 index)
		{
			assert(index < Count);
			return v[index];
		}

		constexpr Y3DVector operator - () const
		{
			return Y3DVector(-x, -y);
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

		constexpr void Normalize()
		{
			static_assert(std::is_floating_point_v<T>, "Normalized() for floating point types only.");
			T reciprocal = 1.f / Magnitude();
			x *= reciprocal; y *= reciprocal;
		}

		constexpr void Clamp()
		{
			for (UINT32 i = 0; i < Count; i++)
			{
				if (v[i] > 1.f - 1e-5f)
				{
					v[i] = 1.f;
					continue;
				}
				else if (v[i] < -1.f + 1e-5f)
				{
					v[i] = -1.f;
					continue;
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
	constexpr Y3DVector<T, 2> operator * (Y3DVector<T, 2> const& lhs, Y3DVector<T, 2> const& rhs)
	{
		return Y3DVector<T, 2>(lhs.x * rhs.x, lhs.y * rhs.y);
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
	constexpr Y3DVector<T, 2> Lerp(Y3DVector<T, 2> const& lhs, Y3DVector<T, 2> const& rhs, T const& fraction)
	{
		return Y3DVector<T, 2>(lhs.x * (1 - fraction) + rhs.x * fraction,
			lhs.y * (1 - fraction) + rhs.y * fraction);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//Template partial specialization Y3DVector version 3
	//
	//////////////////////////////////////////////////////////////////////////

	template <class T>
	class Y3DVector<T, 3>
	{
	public: // Static and Macro

		static Y3DVector const IDENTITY;
		static constexpr UINT32 Count = 3;

	public: //	Constructions and Destructions

		constexpr Y3DVector() = default;
		constexpr explicit Y3DVector(T const& m) : v{ m, m, m } {}
		constexpr Y3DVector(T const& _x, T const& _y, T const& _z) : v{ _x, _y, _z } {}
		constexpr Y3DVector(Y3DVector const& rhs) : v{ rhs.x, rhs.y, rhs.z } {}

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

		constexpr T const& operator [] (UINT32 index) const
		{
			assert(index < Count);
			return v[index];
		}

		constexpr T& operator [] (UINT32 index)
		{
			assert(index < Count);
			return v[index];
		}

		constexpr Y3DVector operator - () const
		{
			return Y3DVector(-x, -y, -z);
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

		constexpr void Normalize() 
		{
			static_assert(std::is_floating_point_v<T>, "Normalized() for floating point types only.");
			T reciprocal = 1.f / Magnitude();
			x *= reciprocal; y *= reciprocal; z *= reciprocal;
		}

		constexpr void Clamp()
		{
			for (UINT32 i = 0; i < Count; i++)
			{
				if (v[i] > 1.f - 1e-5f)
				{
					v[i] = 1.f;
					continue;
				}
				else if (v[i] < -1.f + 1e-5f)
				{
					v[i] = -1.f;
					continue;
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
	constexpr Y3DVector<T, 3> operator * (Y3DVector<T, 3> const& lhs, Y3DVector<T, 3> const& rhs)
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

	// Only three-dimensional space has the cross manipulation
	template <class T>
	constexpr Y3DVector<T, 3> Cross(Y3DVector<T, 3> const& lhs, Y3DVector<T, 3> const& rhs)
	{
		return Y3DVector<T, 3>(lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x);
	}

	template <class T>
	constexpr Y3DVector<T, 3> Lerp(Y3DVector<T, 3> const& lhs, Y3DVector<T, 3> const& rhs, T const& fraction)
	{
		return Y3DVector<T, 3>(lhs.x * (1 - fraction) + rhs.x * fraction,
			lhs.y * (1 - fraction) + rhs.y * fraction,
			lhs.z * (1 - fraction) + rhs.z * fraction);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//Template partial specialization Y3DVector version 4
	//
	//////////////////////////////////////////////////////////////////////////

	template <class T>
	class Y3DVector<T, 4>
	{
	public: // Static and Macro

		static Y3DVector const IDENTITY;
		static constexpr UINT32 Count = 4;

	public: //	Constructions and Destructions

		constexpr Y3DVector() = default;
		constexpr explicit Y3DVector(T const& m) : v{m, m, m, m} {}
		constexpr Y3DVector(T const& _x, T const& _y, T const& _z, T const& _w) : v{_x, _y, _z, _w} {}
		constexpr Y3DVector(Y3DVector const& rhs) : v{rhs.x, rhs.y, rhs.z, rhs.w} {}

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

		constexpr T const& operator [] (UINT32 index) const
		{
			assert(index < Count);
			return v[index];
		}

		constexpr T& operator [] (UINT32 index)
		{
			assert(index < Count);
			return v[index];
		}

		constexpr Y3DVector operator - () const
		{
			return Y3DVector(-x, -y, -z, -w);
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

		constexpr void Normalize()
		{
			static_assert(std::is_floating_point_v<T>, "Normalized() for floating point types only.");
			T reciprocal = 1.f / Magnitude();
			x *= reciprocal; y *= reciprocal; z *= reciprocal; w *= reciprocal;
		}

		constexpr void Clamp()
		{
			for (UINT32 i = 0; i < Count; i++)
			{
				if (v[i] > 1.f - 1e-5f)
				{
					v[i] = 1.f;
					continue;
				}
				else if (v[i] < -1.f + 1e-5f)
				{
					v[i] = -1.f;
					continue;
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
	constexpr Y3DVector<T, 4> operator * (Y3DVector<T, 4> const& lhs, Y3DVector<T, 4> const& rhs)
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
	constexpr Y3DVector<T, 4> Lerp(Y3DVector<T, 4> const& lhs, Y3DVector<T, 4> const& rhs, T const& fraction)
	{
		return Y3DVector<T, 4>(lhs.x * (1 - fraction) + rhs.x * fraction,
			lhs.y * (1 - fraction) + rhs.y * fraction,
			lhs.z * (1 - fraction) + rhs.z * fraction,
			lhs.w * (1 - fraction) + rhs.w * fraction);
	}

	using	V1F32 = Y3DVector<FLOAT32, 1>;
	using	V2F32 = Y3DVector<FLOAT32, 2>;
	using	V3F32 = Y3DVector<FLOAT32, 3>;
	using	V4F32 = Y3DVector<FLOAT32, 4>;

	using	V1F64 = Y3DVector<FLOAT64, 1>;
	using	V2F64 = Y3DVector<FLOAT64, 2>;
	using	V3F64 = Y3DVector<FLOAT64, 3>;
	using	V4F64 = Y3DVector<FLOAT64, 4>;

	using	V1I32 = Y3DVector<INT32, 1>;
	using	V2I32 = Y3DVector<INT32, 2>;
	using	V3I32 = Y3DVector<INT32, 3>;
	using	V4I32 = Y3DVector<INT32, 4>;

	using	V1U32 = Y3DVector<UINT32, 1>;
	using	V2U32 = Y3DVector<UINT32, 2>;
	using	V3U32 = Y3DVector<UINT32, 3>;
	using	V4U32 = Y3DVector<UINT32, 4>;
}