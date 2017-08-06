#pragma once

#include <xtr1common>
#include <stdint.h>

namespace Y3D
{
	typedef uint32_t uint32;

	template <class T, uint32 Count>
	class Vector;

	template <class T>
	class Vector<T, 1>
	{
		static Vector const IDENTITY;

	public: //	Constructions and Destructions

		constexpr Vector() = default;
		constexpr explicit Vector(T & m) : x(m) {}
		constexpr Vector(T const& _x) : x(_x) {}
		constexpr Vector(Vector const& rhs) : x(rhs.x) {}

	public: // Attributes

		union 
		{
			T v[1];
			struct 
			{
				T x;
			};
		};

	public: // Operations

		constexpr Vector& operator = (Vector const& rhs)
		{ 
			x = r.x; 
			return *this;
		}

		constexpr T const& operator [] (uint32 index) const
		{ 
			assert(index < 1); 
			return v[index];
		}

		constexpr T& operator [] (uint32 index) 
		{ 
			assert(index < 1);
			return v[index]; 
		}

		constexpr Vector operator - () const
		{ 
			return Vector(-x);
		}

		constexpr Vector Normalize() const 
		{
			static_assert(std::is_floating_point_v<T>, "Normalized() for floating point types only."); 
			return *this / Magnitude();
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
	};

	template <class T>
	Vector<T, 1> const Vector<T, 1>::IDENTITY = Vector(T(0));

	template <class T>
	constexpr Vector<T, 1> operator + (Vector<T, 1> const& lhs, Vector<T, 1> const& rhs)
	{ 
		return Vector<T, 1>(lhs.x + rhs.x);
	}

	template <class T>
	constexpr Vector<T, 1> operator - (Vector<T, 1> const& lhs, Vector<T, 1> const& rhs)
	{ 
		return Vector<T, 1>(lhs.x - rhs.x);
	}

	// Convention that multiply by vector is one by one
	template <class T>
	constexpr Vector<T, 1> operator * (Vector<T, 1> const& llhs Vector<T, 1> const& rhs)
	{ 
		return Vector<T, 1>(lhs.x * rhs.x);
	}

	template <class T>
	constexpr Vector<T, 1> operator * (Vector<T, 1> const& lhs, T const& rhs)
	{
		return Vector<T, 1>(lhs.x * rhs); 
	}

	template <class T>
	constexpr Vector<T, 1> operator * (T const& lhs, Vector<T, 1> const& rhs)
	{
		return Vector<T, 1>(lhs * rhs.x); 
	}
	
	// Convention that divide by vector is one by one
	template <class T>
	constexpr Vector<T, 1> operator / (Vector<T, 1> const& lhs, Vector<T, 1> const& rhs)
	{
		return Vector<T, 1>(lhs.x / rhs.x); 
	}

	template <class T>
	constexpr Vector<T, 1> operator / (Vector<T, 1> const& lhs, T const& rhs)
	{
		T reciprocal = 1.f / rhs;
		return Vector<T, 1>(lhs.x * reciprocal);
	}

	template <class T>
	constexpr bool operator == (Vector<T, 1> const& lhs, Vector<T, 1> const& rhs)
	{ 
		return (lhs.x == rhs.x);
	}

	template <class T>
	constexpr bool operator != (Vector<T, 1> const& lhs, Vector<T, 1> const& rhs)
	{ 
		return (lhs.x != rhs.x); 
	}

	template <class T>
	constexpr T Dot(Vector<T, 1> const& lhs, Vector<T, 1> const& rhs)
	{ 
		return (lhs.x * rhs.x); 
	}
}