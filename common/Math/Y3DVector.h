#pragma once
#include "common/Math/Y3DMathFunc.h"
#include "common/Types.h"
#include <xtr1common>
#include <assert.h>


namespace Y3D
{
	//Template declaration 
	template <class T, uint32 Count>
	class _Vector;

	//////////////////////////////////////////////////////////////////////////
	//
	//Template partial specialization vector Version_1
	//
	//////////////////////////////////////////////////////////////////////////

	template <class T>
	class _Vector<T, 1>
	{
	public: // Static and Macro

		static constexpr uint32 Count = 1;
		static _Vector const IDENTITY;

	public: //	Constructions and Destructions

		constexpr _Vector() = default;
		constexpr _Vector(T const& _x) : v{ _x } {}
		constexpr _Vector(_Vector const& rhs) : v{ rhs.x } {}

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

		constexpr _Vector& operator = (_Vector const& rhs)
		{ 
			x = rhs.x; 
			return *this;
		}

		constexpr _Vector const& operator += (_Vector const& rhs)
		{
			x += rhs.x;
			return *this;
		}

		constexpr _Vector const& operator -= (_Vector const& rhs)
		{
			x -= rhs.x;
			return *this;
		}

		constexpr _Vector const& operator *= (T const& rhs)
		{
			x *= rhs;
			return *this;
		}

		constexpr _Vector const& operator *= (_Vector const& rhs)
		{
			x *= rhs.x;
			return *this;
		}

		constexpr _Vector const& operator /= (T const& rhs)
		{
			x /= rhs;
			return *this;
		}

		constexpr _Vector const& operator /= (_Vector const& rhs)
		{
			x /= rhs.x;
			return *this;
		}

		constexpr T const& operator [] (uint32 index) const
		{ 
			assert(index < Count);
			return v[index];
		}

		constexpr T& operator [] (uint32 index)
		{ 
			assert(index < Count);
			return v[index]; 
		}

		constexpr _Vector operator - () const
		{ 
			return _Vector(-x);
		}

		constexpr T Magnitude() const 
		{ 
			static_assert(std::is_floating_point_v<T>, "Magnitude() for floating point types only."); 
			return Sqrt(MagnitudeSquared());
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
			static_assert(std::is_floating_point_v<T>, "Normalize() for floating point types only.");
			*this /= Magnitude();
		}

		constexpr _Vector Normalized() const
		{
			static_assert(std::is_floating_point_v<T>, "Normalized() for floating point types only.");
			T reciprocal = T(1) / Magnitude();
			return _Vector(x * reciprocal);
		}

		constexpr void Clamp()
		{
			for (uint32 i = 0; i < 1; i++)
			{
				if (v[i] > T(1.f - 1e-5f))
				{
					v[i] = T(1.f);
					continue;
				}
				else if (v[i] < T(-1.f + 1e-5f))
				{
					v[i] = T(-1.f);
					continue;
				}
			}
		}
	};

	template <class T>
	_Vector<T, 1> const _Vector<T, 1>::IDENTITY = _Vector(T(0));


	template <class T>
	constexpr bool operator == (_Vector<T, 1> const& lhs, _Vector<T, 1> const& rhs)
	{ 
		return lhs.x == rhs.x;
	}

	template <class T>
	constexpr bool operator != (_Vector<T, 1> const& lhs, _Vector<T, 1> const& rhs)
	{ 
		return lhs.x != rhs.x; 
	}

	template <class T>
	constexpr T Dot(_Vector<T, 1> const& lhs, _Vector<T, 1> const& rhs)
	{ 
		return lhs.x * rhs.x; 
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//Template partial specialization vector Version_2
	//
	//////////////////////////////////////////////////////////////////////////

	template <class T>
	class _Vector<T, 2>
	{
	public: // Static and Macro

		static _Vector const IDENTITY;
		static constexpr uint32 Count = 2;

	public: //	Constructions and Destructions

		constexpr _Vector() = default;
		constexpr explicit _Vector(T const& m) : v{ m, m } {}
		constexpr _Vector(T const& _x, T const& _y) : v{ _x, _y } {}
		constexpr _Vector(_Vector const& rhs) : v{ rhs.x, rhs.y } {}

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

		constexpr _Vector& operator = (_Vector const& rhs)
		{
			x = rhs.x; y = rhs.y;
			return *this;
		}

		constexpr _Vector const& operator += (_Vector const& rhs)
		{
			x += rhs.x; y += rhs.y;
			return *this;
		}

		constexpr _Vector const& operator -= (_Vector const& rhs)
		{
			x -= rhs.x; y -= rhs.y;
			return *this;
		}

		constexpr _Vector const& operator *= (T const& rhs)
		{
			x *= rhs; y *= rhs;
			return *this;
		}

		constexpr _Vector const& operator *= (_Vector const& rhs)
		{
			x *= rhs.x; y *= rhs.y;
			return *this;
		}

		constexpr _Vector const& operator /= (T const& rhs)
		{
			T reciprocal = T(1) / rhs;
			*this *= reciprocal;
			return *this;
		}

		constexpr _Vector const& operator /= (_Vector const& rhs)
		{
			x /= rhs.x; y /= rhs.y;
			return *this;
		}

		constexpr T const& operator [] (uint32 index) const
		{
			assert(index < Count);
			return v[index];
		}

		constexpr T& operator [] (uint32 index)
		{
			assert(index < Count);
			return v[index];
		}

		constexpr _Vector operator - () const
		{
			return _Vector(-x, -y);
		}

		constexpr T Magnitude() const
		{
			static_assert(std::is_floating_point_v<T>, "Magnitude() for floating point types only.");
			return Sqrt(MagnitudeSquared());
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
			static_assert(std::is_floating_point_v<T>, "Normalize() for floating point types only.");
			*this /= Magnitude();
		}

		constexpr _Vector Normalized() const
		{
			static_assert(std::is_floating_point_v<T>, "Normalized() for floating point types only.");
			T reciprocal = T(1) / Magnitude();
			return _Vector(x * reciprocal, y * reciprocal);
		}

		constexpr void Clamp()
		{
			for (uint32 i = 0; i < Count; i++)
			{
				if (v[i] > T(1.f - 1e-5f))
				{
					v[i] = T(1.f);
					continue;
				}
				else if (v[i] < T(-1.f + 1e-5f))
				{
					v[i] = T(-1.f);
					continue;
				}
			}
		}
	};

	template <class T>
	_Vector<T, 2> const _Vector<T, 2>::IDENTITY = _Vector(T(0), T(0));

	template <class T>
	constexpr bool operator == (_Vector<T, 2> const& lhs, _Vector<T, 2> const& rhs)
	{
		return lhs.x == rhs.x && 
			lhs.y == rhs.y;
	}

	template <class T>
	constexpr bool operator != (_Vector<T, 2> const& lhs, _Vector<T, 2> const& rhs)
	{
		return lhs.x != rhs.x ||
			lhs.y != rhs.y;
	}

	template <class T>
	constexpr T Dot(_Vector<T, 2> const& lhs, _Vector<T, 2> const& rhs)
	{
		return (lhs.x * rhs.x + lhs.y * rhs.y);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//Template partial specialization Vector version 3
	//
	//////////////////////////////////////////////////////////////////////////

	template <class T>
	class _Vector<T, 3>
	{
	public: // Static and Macro

		static _Vector const IDENTITY;
		static constexpr uint32 Count = 3;

	public: //	Constructions and Destructions

		constexpr _Vector() = default;
		constexpr explicit _Vector(T const& m) : v{ m, m, m } {}
		constexpr _Vector(T const& _x, T const& _y, T const& _z) : v{ _x, _y, _z } {}
		constexpr _Vector(_Vector const& rhs) : v{ rhs.x, rhs.y, rhs.z } {}

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

		constexpr _Vector& operator = (_Vector const& rhs)
		{
			x = rhs.x; y = rhs.y; z = rhs.z;
			return *this;
		}

		constexpr _Vector const& operator += (_Vector const& rhs)
		{
			x += rhs.x; y += rhs.y; z += rhs.z;
			return *this;
		}

		constexpr _Vector const& operator -= (_Vector const& rhs)
		{
			x -= rhs.x; y -= rhs.y; z -= rhs.z;
			return *this;
		}

		constexpr _Vector const& operator *= (T const& rhs)
		{
			x *= rhs; y *= rhs; z *= rhs;
			return *this;
		}

		constexpr _Vector const& operator *= (_Vector const& rhs)
		{
			x *= rhs.x; y *= rhs.y; z *= rhs.z;
			return *this;
		}

		constexpr _Vector const& operator /= (T const& rhs)
		{
			T reciprocal = T(1) / rhs;
			*this *= reciprocal;
			return *this;
		}

		constexpr _Vector const& operator /= (_Vector const& rhs)
		{
			x /= rhs.x; y /= rhs.y; z /= rhs.z;
			return *this;
		}

		constexpr T const& operator [] (uint32 index) const
		{
			assert(index < Count);
			return v[index];
		}

		constexpr T& operator [] (uint32 index)
		{
			assert(index < Count);
			return v[index];
		}

		constexpr _Vector operator - () const
		{
			return _Vector(-x, -y, -z);
		}

		constexpr T Magnitude() const
		{
			static_assert(std::is_floating_point_v<T>, "Magnitude() for floating point types only.");
			return Sqrt(MagnitudeSquared());
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
			static_assert(std::is_floating_point_v<T>, "Normalize() for floating point types only.");
			*this /= Magnitude();
		}

		constexpr _Vector Normalized() const
		{
			static_assert(std::is_floating_point_v<T>, "Normalized() for floating point types only.");
			T reciprocal = T(1) / Magnitude();
			return _Vector(x * reciprocal, y * reciprocal, z * reciprocal);
		}

		constexpr void Clamp()
		{
			for (uint32 i = 0; i < Count; i++)
			{
				if (v[i] > T(1.f - 1e-5f))
				{
					v[i] = T(1.f);
					continue;
				}
				else if (v[i] < T(-1.f + 1e-5f))
				{
					v[i] = T(-1.f);
					continue;
				}
			}
		}
	};

	template <class T>
	_Vector<T, 3> const _Vector<T, 3>::IDENTITY = _Vector(T(0), T(0), T(0));

	template <class T>
	constexpr bool operator == (_Vector<T, 3> const& lhs, _Vector<T, 3> const& rhs)
	{
		return lhs.x == rhs.x && 
			lhs.y == rhs.y && 
			lhs.z == rhs.z;
	}

	template <class T>
	constexpr bool operator != (_Vector<T, 3> const& lhs, _Vector<T, 3> const& rhs)
	{
		return lhs.x != rhs.x ||
			lhs.y != rhs.y || 
			lhs.z != rhs.z;
	}

	template <class T>
	constexpr T Dot(_Vector<T, 3> const& lhs, _Vector<T, 3> const& rhs)
	{
		return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z);
	}

	// Only three-dimensional space has the cross manipulation
	template <class T>
	constexpr _Vector<T, 3> Cross(_Vector<T, 3> const& lhs, _Vector<T, 3> const& rhs)
	{
		return _Vector<T, 3>(lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//Template partial specialization vector Version_4
	//
	//////////////////////////////////////////////////////////////////////////

	template <class T>
	class _Vector<T, 4>
	{
	public: // Static and Macro

		static _Vector const IDENTITY;
		static constexpr uint32 Count = 4;

	public: //	Constructions and Destructions

		constexpr _Vector() = default;
		constexpr explicit _Vector(T const& m) : v{m, m, m, m} {}
		constexpr _Vector(T const& _x, T const& _y, T const& _z, T const& _w) : v{ _x, _y, _z, _w } {}
		constexpr _Vector(_Vector const& rhs) : v{ rhs.x, rhs.y, rhs.z, rhs.w } {}
		constexpr _Vector(_Vector<T, 3> const& rhs, T const& _w) : v{ rhs.x, rhs.y, rhs.z, _w } {}

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

		constexpr _Vector& operator = (_Vector const& rhs)
		{
			x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
			return *this;
		}

		constexpr _Vector const& operator += (_Vector const& rhs)
		{
			x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w;
			return *this;
		}

		constexpr _Vector const& operator -= (_Vector const& rhs)
		{
			x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w;
			return *this;
		}

		constexpr _Vector const& operator *= (T const& rhs)
		{
			x *= rhs; y *= rhs; z *= rhs; w *= rhs;
			return *this;
		}

		constexpr _Vector const& operator *= (_Vector const& rhs)
		{
			x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w;
			return *this;
		}

		constexpr _Vector const& operator /= (T const& rhs)
		{
			T reciprocal = T(1) / rhs;
			*this *= reciprocal;
			return *this;
		}

		constexpr _Vector const& operator /= (_Vector const& rhs)
		{
			x /= rhs.x; y /= rhs.y; z /= rhs.z; w /= rhs.w;
			return *this;
		}

		constexpr T const& operator [] (uint32 index) const
		{
			assert(index < Count);
			return v[index];
		}

		constexpr T& operator [] (uint32 index)
		{
			assert(index < Count);
			return v[index];
		}

		constexpr _Vector operator - () const
		{
			return _Vector(-x, -y, -z, -w);
		}

		constexpr T Magnitude() const
		{
			static_assert(std::is_floating_point_v<T>, "Magnitude() for floating point types only.");
			return Sqrt(MagnitudeSquared());
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
			static_assert(std::is_floating_point_v<T>, "Normalize() for floating point types only.");
			*this /= Magnitude();
		}

		constexpr _Vector Normalized() const
		{
			static_assert(std::is_floating_point_v<T>, "Normalized() for floating point types only.");
			T reciprocal = T(1) / Magnitude();
			return _Vector(x * reciprocal, y * reciprocal, z * reciprocal, w * reciprocal);
		}

		constexpr void Clamp()
		{
			for (uint32 i = 0; i < Count; i++)
			{
				if (v[i] > T(1.f - 1e-5f))
				{
					v[i] = T(1.f);
					continue;
				}
				else if (v[i] < T(-1.f + 1e-5f))
				{
					v[i] = T(-1.f);
					continue;
				}
			}
		}
	};

	template <class T>
	_Vector<T, 4> const _Vector<T, 4>::IDENTITY = _Vector(T(0), T(0), T(0), T(0));

	template <class T>
	constexpr bool operator == (_Vector<T, 4> const& lhs, _Vector<T, 4> const& rhs)
	{
		return lhs.x == rhs.x &&
			lhs.y == rhs.y &&
			lhs.z == rhs.z && 
			lhs.w == rhs.w;
	}

	template <class T>
	constexpr bool operator != (_Vector<T, 4> const& lhs, _Vector<T, 4> const& rhs)
	{
		return lhs.x != rhs.x ||
			lhs.y != rhs.y ||
			lhs.z != rhs.z || 
			lhs.w != rhs.w;
	}

	template <class T>
	constexpr T Dot(_Vector<T, 4> const& lhs, _Vector<T, 4> const& rhs)
	{
		return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// The Common non-member function of all template specialization
	//
	//////////////////////////////////////////////////////////////////////////

	template <class T, uint32 nCount>
	constexpr _Vector<T, nCount> operator + (_Vector<T, nCount> const& lhs, _Vector<T, nCount> const& rhs)
	{ 
		return _Vector<T, nCount>(lhs) += rhs;
	}

	template <class T, uint32 nCount>
	constexpr _Vector<T, nCount> operator - (_Vector<T, nCount> const& lhs, _Vector<T, nCount> const& rhs)
	{
		return _Vector<T, nCount>(lhs) -= rhs;
	}

	// Convention that multiply by _Vector is one by one
	template <class T, uint32 nCount>
	constexpr _Vector<T, nCount> operator * (_Vector<T, nCount> const& lhs, _Vector<T, nCount> const& rhs)
	{
		return _Vector<T, nCount>(lhs) *= rhs;
	}

	template <class T, uint32 nCount>
	constexpr _Vector<T, nCount> operator * (_Vector<T, nCount> const& lhs, T const& rhs)
	{
		return _Vector<T, nCount>(lhs) *= rhs;
	}

	template <class T, uint32 nCount>
	constexpr _Vector<T, nCount> operator * (T const& lhs, _Vector<T, nCount> const& rhs)
	{
		return _Vector<T, nCount>(rhs) *= lhs;
	}
	
	// Convention that divide by _Vector is one by one
	template <class T, uint32 nCount>
	constexpr _Vector<T, nCount> operator / (_Vector<T, nCount> const& lhs, _Vector<T, nCount> const& rhs)
	{
		return _Vector<T, nCount>(lhs) /= rhs;
	}

	template <class T, uint32 nCount>
	constexpr _Vector<T, nCount> operator / (_Vector<T, nCount> const& lhs, T const& rhs)
	{
		return _Vector<T, nCount>(lhs) /= rhs;
	}

	template <class T, uint32 nCount>
	constexpr _Vector<T, nCount> Lerp(_Vector<T, nCount> const& lhs, _Vector<T, nCount> const& rhs, T const& fraction)
	{
		return (lhs * (1 - fraction) + rhs * fraction);
	}


	using	Vec1F32 = _Vector<float32, 1>;
	using	Vec2F32 = _Vector<float32, 2>;
	using	Vec3F32 = _Vector<float32, 3>;
	using	Vec4F32 = _Vector<float32, 4>;

	using	Vec1F64 = _Vector<float64, 1>;
	using	Vec2F64 = _Vector<float64, 2>;
	using	Vec3F64 = _Vector<float64, 3>;
	using	Vec4F64 = _Vector<float64, 4>;

	using	Vec1I32 = _Vector<int32, 1>;
	using	Vec2I32 = _Vector<int32, 2>;
	using	Vec3I32 = _Vector<int32, 3>;
	using	Vec4I32 = _Vector<int32, 4>;

	using	Vec1U32 = _Vector<uint32, 1>;
	using	Vec2U32 = _Vector<uint32, 2>;
	using	Vec3U32 = _Vector<uint32, 3>;
	using	Vec4U32 = _Vector<uint32, 4>;

	Vec3F32 const g_vAxisX = Vec3F32(1.f, 0.f, 0.f);
	Vec3F32 const g_vAxisY = Vec3F32(0.f, 1.f, 0.f);
	Vec3F32 const g_vAxisZ = Vec3F32(0.f, 0.f, 1.f);
}