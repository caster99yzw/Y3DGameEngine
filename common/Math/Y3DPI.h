#pragma once

#include "common/type.h"


// The constexpr declaration and definition must be together

namespace Y3D
{
	const float64 Y3D_PI_F64		= 3.1415926535897932385;
	const float64 Y3D_PI_TWO_F64	= 6.2831853071795864769;
	const float64 Y3D_PI_HALF_F64	= 1.5707963267948966192;
	const float64 Y3D_PI_INV_F64	= 0.3183098861837906715;

	const float32 Y3D_PI_F32		= 3.1415926535897932385f;
	const float32 Y3D_PI_TWO_F32	= 6.2831853071795864769f;
	const float32 Y3D_PI_HALF_F32	= 1.5707963267948966192f;
	const float32 Y3D_PI_INV_F32	= 0.3183098861837906715f;

	template <class T>
	constexpr static T DEGREE_2_RADIAN(T const& degree)
	{
		return degree * static_cast<T>(.017453292519943295769236907684);
	}

	template <class T>
	constexpr static T RADIAN_2_DEGREE(T const& radian)
	{
		return radian * static_cast<T>(57.2957795130823208767981548141);
	}

	template <class T>
	class Degree;

	template <class T>
	class Radian;

	template <class T>
	class Degree
	{
	public: //	Constructions and Destructions

		constexpr Degree() = default;
		constexpr explicit Degree(T const& val) : degVaule(val) {}
		Degree(Radian<T> const& rad);

	public: // Attributes

		T degVaule = 0;

	public: // Operations

		constexpr Degree operator - () const 
		{ 
			return Degree(-degVaule);
		}

		constexpr Degree& operator += (Degree const& rhs) { degVaule += rhs.degVaule; return *this; }

		constexpr Degree& operator -= (Degree const& rhs) { degVaule -= rhs.degVaule; return *this; }

		constexpr Degree& operator *= (T const& rhs) { degVaule *= rhs; return *this; }

		constexpr Degree& operator /= (T const& rhs) { degVaule /= rhs; return *this; }

		// Manipulation about convert between radian and degree
		Radian<T>		ToRadina();
		void			FromRadian(Radian<T> const& rad);
	};

	template <class T>
	constexpr bool operator == (Degree<T> const& lhs, Degree<T> const& rhs)
	{ 
		return lhs.degVaule == rhs.degVaule;
	}

	template <class T>
	constexpr bool operator != (Degree<T> const& lhs, Degree<T> const& rhs)
	{
		return lhs.degVaule != rhs.degVaule;
	}

	template <class T>
	constexpr bool operator < (Degree<T> const& lhs, Degree<T> const& rhs)
	{
		return lhs.degVaule < rhs.degVaule;
	}

	template <class T>
	constexpr bool operator <= (Degree<T> const& lhs, Degree<T> const& rhs)
	{
		return lhs.degVaule <= rhs.degVaule;
	}

	template <class T>
	constexpr bool operator > (Degree<T> const& lhs, Degree<T> const& rhs)
	{
		return lhs.degVaule > rhs.degVaule;
	}

	template <class T>
	constexpr bool operator >= (Degree<T> const& lhs, Degree<T> const& rhs)
	{
		return lhs.degVaule >= rhs.degVaule;
	}

	template <class T>
	constexpr Degree<T> operator + (Degree<T> const& lhs, Degree<T> const& rhs) 
	{ 
		return Degree<T>(lhs) += rhs; 
	}

	template <class T>
	constexpr Degree<T> operator - (Degree<T> const& lhs, Degree<T> const& rhs)
	{
		return Degree<T>(lhs) -= rhs;
	}

	template <class T>
	constexpr Degree<T> operator * (Degree<T> const& lhs, T const& rhs)
	{
		return Degree<T>(lhs) *= rhs;
	}
	template <class T>
	constexpr Degree<T> operator / (Degree<T> const& lhs, T const& rhs)
	{
		return Degree<T>(lhs) /= rhs;
	}

	template <class T>
	class Radian
	{
	public: //	Constructions and Destructions

		constexpr Radian() = default;
		constexpr explicit Radian(T const& val) : radValue(val) {}
		Radian(Degree<T> const& deg);

	public: // Attributes

		T radValue = 0;

	public: // Operations

		constexpr Radian operator - () const
		{
			return Radian(-degVaule);
		}

		constexpr Radian& operator += (Radian const& rhs) { radValue += rhs.radValue; return *this; }

		constexpr Radian& operator -= (Radian const& rhs) { radValue -= rhs.radValue; return *this; }

		constexpr Radian& operator *= (T const& rhs) { radValue *= rhs; return *this; }

		constexpr Radian& operator /= (T const& rhs) { radValue /= rhs; return *this; }

		// Manipulation about convert between radian and degree
		Degree<T>		ToDegree();
		void			FromDegree(Degree<T> const& deg);
	};

	template <class T>
	constexpr bool operator == (Radian<T> const& lhs, Radian<T> const& rhs)
	{
		return lhs.radValue == rhs.radValue;
	}

	template <class T>
	constexpr bool operator != (Radian<T> const& lhs, Radian<T> const& rhs)
	{
		return lhs.radValue != rhs.radValue;
	}

	template <class T>
	constexpr bool operator < (Radian<T> const& lhs, Radian<T> const& rhs)
	{
		return lhs.radValue < rhs.radValue;
	}

	template <class T>
	constexpr bool operator <= (Radian<T> const& lhs, Radian<T> const& rhs)
	{
		return lhs.radValue <= rhs.radValue;
	}

	template <class T>
	constexpr bool operator > (Radian<T> const& lhs, Radian<T> const& rhs)
	{
		return lhs.radValue > rhs.radValue;
	}

	template <class T>
	constexpr bool operator >= (Radian<T> const& lhs, Radian<T> const& rhs)
	{
		return lhs.radValue >= rhs.radValue;
	}

	template <class T>
	constexpr Radian<T> operator + (Radian<T> const& lhs, Radian<T> const& rhs)
	{
		return Radian<T>(lhs) += rhs;
	}

	template <class T>
	constexpr Radian<T> operator - (Radian<T> const& lhs, Radian<T> const& rhs)
	{
		return Radian<T>(lhs) -= rhs;
	}

	template <class T>
	constexpr Radian<T> operator * (Radian<T> const& lhs, T const& rhs)
	{
		return Radian<T>(lhs) *= rhs;
	}

	template <class T>
	constexpr Radian<T> operator / (Radian<T> const& lhs, T const& rhs)
	{
		return Radian<T>(lhs) /= rhs;
	}

	template <class T>
	Degree<T>::Degree(Radian<T> const& rad)
	{
		degVaule = RADIAN_2_DEGREE(deg.radValue);
	}

	template <class T>
	Radian<T>::Radian(Degree<T> const& deg)
	{
		radValue = DEGREE_2_RADIAN(deg.degVaule);
	}

	template <class T>
	void Degree<T>::FromRadian(Radian<T> const& rad)
	{
		degVaule = RADIAN_2_DEGREE(rad.radValue);
	}

	template <class T>
	Radian<T> Degree<T>::ToRadina()
	{
		return Radian<T>(degVaule);
	}

	template <class T>
	void Radian<T>::FromDegree(Degree<T> const& deg)
	{
		degVaule = DEGREE_2_RADIAN(deg.degVaule);
	}

	template <class T>
	Degree<T> Radian<T>::ToDegree()
	{
		return Degree<T>(radValue);
	}

	using RadianF32 = Radian<float32>;
	using DegreeF32 = Degree<float32>;
}