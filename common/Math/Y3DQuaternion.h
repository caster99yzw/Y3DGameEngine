#pragma once

#include "Y3DVector.h"
#include <cmath>
#include <assert.h>

//////////////////////////////////////////////////////////////////////////
//
// Quaternion<T> is differenFLOAT32 from _Vector<T, 4>
// The Order of _Vector<T, 4> is <x, y, z, w>
// The Order of Quaternion<T> is <w, x, y, z>
//
//////////////////////////////////////////////////////////////////////////

namespace Y3D
{
	//////////////////////////////////////////////////////////////////////////
	//
	//class Quaternion 
	//
	//////////////////////////////////////////////////////////////////////////

	template <class T>
	class _Quaternion
	{
	public: // Static and Macro

		static _Quaternion const ZERO;
		static _Quaternion const IDENTITY;

	public: //	Constructions and Destructions

		constexpr _Quaternion() = default;
		constexpr _Quaternion(T const& _w, T const& _x, T const& _y, T const& _z) : q{ _w, _x, _y, _z } {}
		constexpr _Quaternion(T const& _w, _Vector<T, 3> const& rhs) : q{ _w, rhs.x, rhs.y, rhs.z } {}
		constexpr _Quaternion(_Quaternion const& rhs) : q{ rhs.w, rhs.x, rhs.y, rhs.z } {}

	public: // Attributes
		
		union
		{
			T q[4];

			struct
			{
				T w;
				union
				{
					_Vector<T, 3> axis;
					struct
					{
						T x;
						T y;
						T z;
					};
				};
			};
		};

	public: // Operations

		constexpr _Quaternion& operator = (_Quaternion const& rhs)
		{
			w = rhs.w;  x = rhs.x; y = rhs.y; z = rhs.z;
			return *this;
		}

		constexpr _Quaternion const& operator += (_Quaternion const& rhs)
		{
			w += rhs.w;  x += rhs.x; y += rhs.y; z += rhs.z;
			return *this;
		}

		constexpr _Quaternion const& operator -= (_Quaternion const& rhs)
		{
			w -= rhs.w;  x -= rhs.x; y -= rhs.y; z -= rhs.z;
			return *this;
		}

		constexpr _Quaternion const& operator *= (T const& rhs)
		{
			w *= rhs; x *= rhs; y *= rhs; z *= rhs;
			return *this;
		}

		constexpr _Quaternion const& operator /= (T const& rhs)
		{
			float32 reciprocal = 1.f / rhs;
			*this *= reciprocal;
			return *this;
		}

		constexpr _Quaternion const& operator *= (_Quaternion const& rhs)
		{
			_Quaternion lhs = *this;
			w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
			x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
			y = lhs.w * rhs.y + lhs.x * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z;
			z = lhs.w * rhs.z + lhs.x * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x;
			return *this;
		}

		constexpr _Quaternion operator - () const
		{
			return Quaternion(-w, -x, -y, -z);
		}

		constexpr T & operator[](uint32 index) 
		{ 
			assert(index < Count); 
			return v[index]; 
		}

		constexpr T const& operator[](uint32 index) const
		{
			assert(index < Count);
			return v[index];
		}
		
		constexpr T & W()
		{
			return w;
		}

		constexpr T const& W() const
		{
			return w;
		}

		constexpr _Vector<T, 3> & V()
		{
			return axis;
		}

		constexpr _Vector<T, 3> const& V() const
		{
			return axis;
		}

		constexpr T Magnitude() const
		{
			return Sqrt(MagnitudeSquared());
		}

		constexpr T MagnitudeSquared() const
		{
			return Dot(*this, *this);
		}

		constexpr _Quaternion Conjugate(_Quaternion const& quat) const
		{
			return _Quaternion(-quat.w, -quat.x, -quat.y, -quat.z);
		}

		constexpr _Quaternion Inverse(_Quaternion const& quat) const
		{
			return Conjugate(*this).Normalize();
		}

		constexpr void Normalize()
		{
			static_assert(std::is_floating_point_v<T>, "Magnitude() for floating point types only.");
			*this /= Magnitude();
		}

		constexpr _Quaternion Normalize()
		{
			static_assert(std::is_floating_point_v<T>, "Magnitude() for floating point types only.");
			T reciprocal = T(1) / Magnitude();
			return _Quaternion(w * reciprocal, x * reciprocal, y * reciprocal, z * reciprocal);
		}

		// Rotate vec By using quaternion and its conjugate
		//Vec3F32 Rotate(Vec3F32 const& vec) const;
		// Rotate vec directly By using quaternion definition
		//Vec3F32 RotateDirect(Vec3F32 const& vec) const;

		// Before cos manipulation, we must guarantee all quaternion should be normalized
		//FLOAT32 Angle(Quaternion quat);
	};
	
	template <class T>
	_Quaternion<T> const _Quaternion<T>::ZERO = _Quaternion<T>(0.f, 0.f, 0.f, 0.f);

	template <class T>
	_Quaternion<T> const _Quaternion<T>::IDENTITY = _Quaternion<T>(1.f, 0.f, 0.f, 0.f);

	template <class T>
	constexpr _Quaternion<T> operator + (_Quaternion<T> const& lhs, _Quaternion<T> const& rhs)
	{
		return _Quaternion<T>(lhs) += rhs;
	}

	template <class T>
	constexpr _Quaternion<T> operator - (_Quaternion<T> const& lhs, _Quaternion<T> const& rhs)
	{
		return _Quaternion<T>(lhs) -= rhs;
	}

	template <class T>
	constexpr _Quaternion<T> operator * (_Quaternion<T> const& lhs, T const& rhs)
	{
		return _Quaternion<T>(lhs) *= rhs;
	}

	template <class T>
	constexpr _Quaternion<T> operator * (T const& lhs, _Quaternion<T> const& rhs)
	{
		return _Quaternion<T>(rhs) *= lhs;
	}

	template <class T>
	constexpr _Quaternion<T> operator / (_Quaternion<T> const& lhs, T const& rhs)
	{
		return _Quaternion<T>(lhs) /= rhs;
	}

	template <class T>
	constexpr _Quaternion<T> operator * (_Quaternion<T> const& lhs, _Quaternion<T> const& rhs)
	{
		return _Quaternion<T>(lhs) *= rhs;
	}

	template <class T>
	constexpr bool operator == (_Quaternion<T> const& lhs, _Quaternion<T> const& rhs)
	{
		return lhs.w == rhs.w &&
			lhs.x == rhs.x &&
			lhs.y == rhs.y &&
			lhs.z == rhs.z;
	}

	template <class T>
	constexpr bool operator != (_Quaternion<T> const& lhs, _Quaternion<T> const& rhs)
	{
		return lhs.w != rhs.w || 
			lhs.x != rhs.x ||
			lhs.y != rhs.y || 
			lhs.z != rhs.z;
	}

	//// q_ResulFLOAT32 = q * v * q*
	//Vec3F32 _Rotate_Vector(_Quaternion<T> const& quat, Vec3F32 const& vec, _Quaternion<T> const& quatConjugate)
	//{
	//	Quaternion vQuat = Quaternion(0.f, vec);
	//	vQuat = quat * vQuat * quatConjugate;
	//	return Vec3F32(vQuat.x, vQuat.y, vQuat.z);
	//}

	//// q_ResulFLOAT32 = (quat.w^2 - ||quat.xyz||^2) * vec + 2 * Dot(quat.xyz, vec) * quat.xyz + 2 * quat.w * Cross(quat, vec)
	//Vec3F32 _Rotate_Vector(Quaternion const& quat, Vec3F32 const& vec)
	//{
	//	FLOAT32 a0 = quat.W() * quat.W() - quat.V().MagnitudeSquared();
	//	FLOAT32 a1 = 2.f - Dot(quat.V(), vec);
	//	FLOAT32 a2 = 2.f * quat.W();

	//	return (a0 * vec + a1 * quat.V() + a2 * Cross(quat.V(), vec));
	//}

	template <class T>
	constexpr T Dot(_Quaternion<T> const& lhs, _Quaternion<T> const& rhs)
	{
		return (lhs.w * rhs.w + lhs.x * rhs.x + lhs.y + rhs.y + lhs.z * rhs.z);
	}

	// Spherical Lerp
	//Quaternion Slerp(Quaternion const& lhs, Quaternion const& rhs, FLOAT32 fraction)
	//{
	//	FLOAT32 fCos = Dot(lhs, rhs);
	//	fCos = (fCos < 0.f) ? -fCos : fCos;		

	//	if (fCos < 0.9995f)
	//	{
	//		FLOAT32 theta_0 = ::acos(fCos);
	//		FLOAT32 theta = theta_0 * fraction;  
	//		Quaternion rhsNew = rhs - lhs * fCos;
	//		rhsNew.Normalize();             
	//		return (lhs * ::cos(theta) + rhsNew * ::sin(theta));
	//	}
	//	else
	//	{
	//		// There are two situations:
	//		// 1. "q1" and "q2" are very close (fCos ~= +1)
	//		// 2. "q1" and "q2" are almost inverse of each other (fCos ~= -1).
	//		// So we can do a linear interpolation safely.
	//		Quaternion q = (1.f - fraction) * lhs + fraction * rhs;
	//		q.Normalize();
	//		return q;
	//	}
	//}

	//// Normalized Lerp
	//Quaternion Nlerp(Quaternion const& lhs, Quaternion const& rhs, FLOAT32 fraction)
	//{
	//	Quaternion quat = (Dot(lhs, rhs) < 0.f) ? -rhs : rhs;

	//	return Quaternion((1.f - fraction) * lhs.w + fraction * rhs.w,
	//		(1.f - fraction) * lhs.x + fraction * rhs.x,
	//		(1.f - fraction) * lhs.y + fraction * rhs.y, 
	//		(1.f - fraction) * lhs.z + fraction * rhs.z);
	//}

	//////////////////////////////////////////////////////////////////////////
	//
	// The definition of member function
	//
	//////////////////////////////////////////////////////////////////////////

	//FLOAT32 Quaternion::Angle(Quaternion quat)
	//{
	//	quat.Normalize();
	//	(*this).Normalize();
	//	return Dot(*this, quat);
	//}

	// Rotate vec By using quaternion and its conjugate
	//Vec3F32 Quaternion::Rotate(Vec3F32 const& vec) const
	//{
	//	Quaternion qConjugate = Conjugate(*this);
	//	return _Rotate_Vector(*this, vec, qConjugate);
	//}
	// 
	//	Rotate vec directly By using quaternion definition
	//Vec3F32 Quaternion::RotateDirect(Vec3F32 const& vec) const
	//{ 
	//	return _Rotate_Vector(*this, vec);
	//}

	using QuatF32 = _Quaternion<float32>;
	using QuatF64 = _Quaternion<float64>;
}