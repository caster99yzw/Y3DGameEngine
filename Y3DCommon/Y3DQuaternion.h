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

	class Quaternion
	{
	public: // Static and Macro

		static Quaternion const ZERO;
		static Quaternion const IDENTITY;

	public: //	Constructions and Destructions

		Quaternion() = default;
		Quaternion(FLOAT32 const& _w, FLOAT32 const& _x, FLOAT32 const& _y, FLOAT32 const& _z) : q{ _w, _x, _y, _z } {}
		Quaternion(Quaternion const& rhs) : q{ rhs.w, rhs.x, rhs.y, rhs.z } {}
		Quaternion(FLOAT32 const& _w, Vec3F32 const& rhs) : q{ _w, rhs.x, rhs.y, rhs.z } {}

	public: // Attributes

		union
		{
			FLOAT32 q[4];
			struct
			{
				FLOAT32 w, x, y, z;
			}; 
		};

	public: // Operations

		inline Quaternion& operator = (Quaternion const& rhs)
		{
			w = rhs.w;  x = rhs.x; y = rhs.y; z = rhs.z;
			return *this;
		}

		inline Quaternion const& operator += (Quaternion const& rhs)
		{
			w += rhs.w;  x += rhs.x; y += rhs.y; z += rhs.z;
			return *this;
		}

		inline Quaternion const& operator -= (Quaternion const& rhs)
		{
			w -= rhs.w;  x -= rhs.x; y -= rhs.y; z -= rhs.z;
			return *this;
		}

		inline Quaternion const& operator *= (FLOAT32 rhs)
		{
			w *= rhs; x *= rhs; y *= rhs; z *= rhs;
			return *this;
		}

		inline Quaternion const& operator /= (FLOAT32 rhs)
		{
			FLOAT32 reciprocal = 1.f / rhs;
			*this *= reciprocal;
			return *this;
		}

		inline Quaternion const& operator *= (Quaternion const& rhs)
		{
			Quaternion lhs = *this;
			w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
			x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
			y = lhs.w * rhs.y + lhs.x * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z;
			z = lhs.w * rhs.z + lhs.x * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x;
			return *this;
		}

		inline Quaternion operator - () const
		{
			return Quaternion(-w, -x, -y, -z);
		}

		inline FLOAT32 & W()
		{
			return w;
		}

		inline FLOAT32 const& W() const
		{
			return w;
		}

		inline Vec3F32 V()
		{
			return Vec3F32(x, y, z);
		}

		inline Vec3F32 const V() const
		{
			return Vec3F32(x, y, z);
		}

		inline FLOAT32 Magnitude() const
		{
			return std::sqrt(MagnitudeSquared());
		}

		inline Quaternion Conjugate(Quaternion const& quat) const
		{
			return Quaternion(quat.w, -quat.x, -quat.y, -quat.z);
		}

		inline Quaternion Inverse(Quaternion const& quat) const
		{
			return Conjugate(*this);
		}

		inline FLOAT32 MagnitudeSquared() const;

		// Rotate vec By using quaternion and its conjugate
		Vec3F32 Rotate(Vec3F32 const& vec) const;
		// Rotate vec directly By using quaternion definition
		Vec3F32 RotateDirect(Vec3F32 const& vec) const;

		// Before cos manipulation, we must guarantee all quaternion should be normalized
		FLOAT32 Angle(Quaternion quat);

		inline void Normalize()
		{
			*this /= Magnitude();
		}

	};

	Quaternion const Quaternion::ZERO = Quaternion(0.f, 0.f, 0.f, 0.f);

	Quaternion const Quaternion::IDENTITY = Quaternion(1.f, 0.f, 0.f, 0.f);

	inline Quaternion operator + (Quaternion const& lhs, Quaternion const& rhs)
	{
		return (Quaternion(lhs) += rhs);
	}

	inline Quaternion operator - (Quaternion const& lhs, Quaternion const& rhs)
	{
		return (Quaternion(lhs) -= rhs);
	}

	inline Quaternion operator * (Quaternion const& lhs, FLOAT32 rhs)
	{
		return (Quaternion(lhs) *= rhs);
	}

	inline Quaternion operator * (FLOAT32 lhs, Quaternion const& rhs)
	{
		return (Quaternion(rhs) *= lhs);
	}

	inline Quaternion operator / (Quaternion const& lhs, FLOAT32 rhs)
	{
		return (Quaternion(lhs) /= rhs);
	}

	// Quaternion Product
	inline Quaternion operator * (Quaternion const& lhs, Quaternion const& rhs)
	{
		return (Quaternion(lhs) *= rhs);
	}

	inline bool operator == (Quaternion const& lhs, Quaternion const& rhs)
	{
		return (lhs.w == rhs.w && lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z);
	}

	inline bool operator != (Quaternion const& lhs, Quaternion const& rhs)
	{
		return (lhs.w != rhs.w || lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z);
	}

	// q_ResulFLOAT32 = q * v * q*
	Vec3F32 _Rotate_Vector(Quaternion const& quat, Vec3F32 const& vec, Quaternion const& quatConjugate)
	{
		Quaternion vQuat = Quaternion(0.f, vec);
		vQuat = quat * vQuat * quatConjugate;
		return Vec3F32(vQuat.x, vQuat.y, vQuat.z);
	}

	// q_ResulFLOAT32 = (quat.w^2 - ||quat.xyz||^2) * vec + 2 * Dot(quat.xyz, vec) * quat.xyz + 2 * quat.w * Cross(quat, vec)
	Vec3F32 _Rotate_Vector(Quaternion const& quat, Vec3F32 const& vec)
	{
		FLOAT32 a0 = quat.W() * quat.W() - quat.V().MagnitudeSquared();
		FLOAT32 a1 = 2.f - Dot(quat.V(), vec);
		FLOAT32 a2 = 2.f * quat.W();

		return (a0 * vec + a1 * quat.V() + a2 * Cross(quat.V(), vec));
	}

	FLOAT32 Dot(Quaternion const& lhs, Quaternion const& rhs)
	{
		return (lhs.w * rhs.w + lhs.x * rhs.x + lhs.y + rhs.y + lhs.z * rhs.z);
	}

	// Spherical Lerp
	Quaternion Slerp(Quaternion const& lhs, Quaternion const& rhs, FLOAT32 fraction)
	{
		FLOAT32 fCos = Dot(lhs, rhs);
		fCos = (fCos < 0.f) ? -fCos : fCos;		

		if (fCos < 0.9995f)
		{
			FLOAT32 theta_0 = ::acos(fCos);
			FLOAT32 theta = theta_0 * fraction;  
			Quaternion rhsNew = rhs - lhs * fCos;
			rhsNew.Normalize();             
			return (lhs * ::cos(theta) + rhsNew * ::sin(theta));
		}
		else
		{
			// There are two situations:
			// 1. "q1" and "q2" are very close (fCos ~= +1)
			// 2. "q1" and "q2" are almost inverse of each other (fCos ~= -1).
			// So we can do a linear interpolation safely.
			Quaternion q = (1.f - fraction) * lhs + fraction * rhs;
			q.Normalize();
			return q;
		}
	}

	// Normalized Lerp
	Quaternion Nlerp(Quaternion const& lhs, Quaternion const& rhs, FLOAT32 fraction)
	{
		Quaternion quat = (Dot(lhs, rhs) < 0.f) ? -rhs : rhs;

		return Quaternion((1.f - fraction) * lhs.w + fraction * rhs.w,
			(1.f - fraction) * lhs.x + fraction * rhs.x,
			(1.f - fraction) * lhs.y + fraction * rhs.y, 
			(1.f - fraction) * lhs.z + fraction * rhs.z);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// The definition of member function
	//
	//////////////////////////////////////////////////////////////////////////
	FLOAT32 Quaternion::MagnitudeSquared() const
	{
		return Dot(*this, *this);
	}

	FLOAT32 Quaternion::Angle(Quaternion quat)
	{
		quat.Normalize();
		(*this).Normalize();
		return Dot(*this, quat);
	}

	// Rotate vec By using quaternion and its conjugate
	Vec3F32 Quaternion::Rotate(Vec3F32 const& vec) const
	{
		Quaternion qConjugate = Conjugate(*this);
		return _Rotate_Vector(*this, vec, qConjugate);
	}
	 
	// Rotate vec directly By using quaternion definition
	Vec3F32 Quaternion::RotateDirect(Vec3F32 const& vec) const
	{ 
		return _Rotate_Vector(*this, vec);
	}
}