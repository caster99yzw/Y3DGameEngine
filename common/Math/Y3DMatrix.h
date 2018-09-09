#pragma once
#include "common/Y3DTypes.h"
#include "common/Math/Y3DVector.h"


namespace Y3D
{
	//////////////////////////////////////////////////////////////////////////
	//
	// Column major matrix, column major storage
	// Must be careful use matrix in DirectX that use left multiply convention
	//
	//////////////////////////////////////////////////////////////////////////

	//Template declaration 
	template <class T, UINT32 Row, UINT32 Col>
	class _Matrix;

	//////////////////////////////////////////////////////////////////////////
	//
	// Template partial specialization matrix Version_3_3
	//
	//////////////////////////////////////////////////////////////////////////

	template <class T>
	class _Matrix<T, 3, 3>
	{
	public: // Static and Macro

		static _Matrix const ZERO;
		static _Matrix const IDENTITY;

		static constexpr UINT32 Row = 3;
		static constexpr UINT32 Col = 3;
		static constexpr UINT32 Count = Row * Col;


	public: //	Constructions and Destructions

		constexpr _Matrix() = default;
		_Matrix(_Vector<T, Col> const& c0, _Vector<T, Col> const& c1, _Vector<T, Col> const& c2) : M{ c0, c1, c2 } {}
		_Matrix(_Vector<_Vector<T, Col>, Row> const& _mat) : M(_mat) {}
		_Matrix(_Matrix const& mat) : M(mat.M) {}

		// Promise T is value type
		_Matrix(T const& _m) 
			: M{ { _m, T(0), T(0) }
			, { T(0), _m, T(0) }
			, { T(0), T(0), _m } } {}
		
		// Column major matrix
		_Matrix(T const& m11, T const& m21, T const& m31
			, T const& m12, T const& m22, T const& m32
			, T const& m13, T const& m23, T const& m33)
			: M{ { m11, m21, m31 }, { m12, m22, m32 }, { m13, m23, m33 } } {}
		

	public: // Attributes

		union
		{
			struct
			{
				T	_11, _21, _31,
					_12, _22, _32,
					_13, _23, _33;
			};
			_Vector<_Vector<T, Col>, Row> M;
		};
		
	public: // Operations

		constexpr _Matrix& operator = (_Matrix const& rhs)
		{
			_11 = rhs._11; _21 = rhs._21; _31 = rhs._31;
			_12 = rhs._12; _22 = rhs._22; _32 = rhs._32;
			_13 = rhs._13; _23 = rhs._23; _33 = rhs._33;
			return *this;
		}

		constexpr _Matrix const& operator += (_Matrix const& rhs)
		{
			_11 += rhs._11; _21 += rhs._21; _31 += rhs._31;
			_12 += rhs._12; _22 += rhs._22; _32 += rhs._32;
			_13 += rhs._13; _23 += rhs._23; _33 += rhs._33;
			return *this;
		}

		constexpr _Matrix const& operator -= (_Matrix const& rhs)
		{
			_11 -= rhs._11; _21 -= rhs._21; _31 -= rhs._31;
			_12 -= rhs._12; _22 -= rhs._22; _32 -= rhs._32;
			_13 -= rhs._13; _23 -= rhs._23; _33 -= rhs._33;
			return *this;
		}

		constexpr _Matrix const& operator *= (T const& rhs)
		{
			_11 *= rhs; _21 *= rhs; _31 *= rhs;
			_12 *= rhs; _22 *= rhs; _32 *= rhs;
			_13 *= rhs; _23 *= rhs; _33 *= rhs;
			return *this;
		}

		constexpr _Matrix const& operator /= (T const& rhs)
		{
			T reciprocal = T(1) / lhs;
			_11 *= reciprocal; _21 *= reciprocal; _31 *= reciprocal;
			_12 *= reciprocal; _22 *= reciprocal; _32 *= reciprocal;
			_13 *= reciprocal; _23 *= reciprocal; _33 *= reciprocal;
			return *this;
		}

		constexpr _Matrix const& operator *= (_Matrix const& rhs)
		{
			_11 = _11 * rhs._11 + _12 * rhs._21 + _13 * rhs._31;
			_21 = _21 * rhs._11 + _22 * rhs._21 + _23 * rhs._31;
			_31 = _31 * rhs._11 + _32 * rhs._21 + _33 * rhs._31;
			_12 = _11 * rhs._12 + _12 * rhs._22 + _13 * rhs._32;
			_22 = _21 * rhs._12 + _22 * rhs._22 + _23 * rhs._32;
			_32 = _31 * rhs._12 + _32 * rhs._22 + _33 * rhs._32;
			_13 = _11 * rhs._13 + _12 * rhs._23 + _13 * rhs._33;
			_23 = _21 * rhs._13 + _22 * rhs._23 + _23 * rhs._33;
			_33 = _31 * rhs._13 + _32 * rhs._23 + _33 * rhs._33;
			return *this;
		}

		constexpr _Vector<T, Col> const& operator[](UINT32 index) const
		{
			assert(index < Row);
			return M[index];
		} 

		constexpr _Vector<T, Col>& operator[](UINT32 index)
		{
			assert(index < Row);
			return M[index];
		}

		constexpr _Vector<T, Col> const GetRow(UINT32 index) const 
		{
			return _Vector<T, Col>(v[0][index], v[1][index], v[2][index]);
		}

		constexpr void multisetRow(UINT32 index, _Vector<T, Col> const& rv)
		{
			v[0][index] = rv.v[0];
			v[1][index] = rv.v[1];
			v[2][index] = rv.v[2];
		}

		constexpr _Vector<T, Row> const& GetColumn(UINT32 index) const 
		{
			return M[index];
		}

		constexpr void multisetCol(UINT32 index, _Vector<T, Row> const& rv)
		{
			v[index][0] = rv.v[0];
			v[index][1] = rv.v[1];
			v[index][2] = rv.v[2];
		}

		constexpr _Matrix operator-() const 
		{ 
			return _Matrix(-M);
		}

		constexpr void Transpose()
		{
			T m21 = M[0][1];
			T m31 = M[0][2];
			T m32 = M[1][2];

			M[0][1] = M[1][0]; M[0][2] = M[2][0]; M[1][2] = M[2][1];
			M[1][0] = m21; M[2][0] = m31; M[2][1] = m32;
		}

		constexpr _Matrix Transposed() const
		{
			return _Matrix(
				M[0][0], M[1][0], M[2][0],
				M[0][1], M[1][1], M[2][1],
				M[0][2], M[1][2], M[2][2]);
		}

		constexpr T Determinant() const
		{
			T	m11 = M[0][0], m21 = M[1][0], m31 = M[2][0],
				m12 = M[0][1], m22 = M[1][1], m32 = M[2][1],
				m13 = M[0][2], m23 = M[1][2], m33 = M[2][2];

			T	_2233 = m22 * m33,
				_2332 = m23 * m32,
				_2331 = m23 * m31,
				_2133 = m21 * m33,
				_2132 = m21 * m32,
				_2231 = m22 * m31;

			T	_2233_2332 = _2233 - _2332,
				_2331_2133 = _2331 - _2133,
				_2132_2231 = _2132 - _2231;

			return (m11 * _2233_2332 + m12 * _2331_2133 + m13 * _2132_2231);
		}

		constexpr _Matrix Inversed() const
		{
			T determinant = Determinant();

			// non-invertible
			if (determinant == T(0))
			{
				assert(false);
				return IDENTITY;
			}

			T inverseDeterminant = T(1) / determinant;

			return _Matrix(
				(_22 * _33 - _23 * _32) * inverseDeterminant,
				(_23 * _31 - _21 * _33) * inverseDeterminant,
				(_21 * _32 - _22 * _31) * inverseDeterminant,
				(_13 * _32 - _12 * _33) * inverseDeterminant,
				(_11 * _33 - _13 * _31) * inverseDeterminant,
				(_12 * _31 - _11 * _32) * inverseDeterminant,
				(_12 * _23 - _13 * _22) * inverseDeterminant,
				(_13 * _21 - _11 * _23) * inverseDeterminant,
				(_11 * _22 - _12 * _21) * inverseDeterminant);
		}


		T const* Data() const
		{
			return M.Data()->Data();
		}
	};

	template <class T>
	_Matrix<T, 3, 3> const _Matrix<T, 3, 3>::ZERO = _Matrix(T(0));

	template <class T>
	_Matrix<T, 3, 3> const _Matrix<T, 3, 3>::IDENTITY = _Matrix(T(1));

	template <class T>
	constexpr bool operator == (_Matrix<T, 3, 3> const& lhs, _Matrix<T, 3, 3> const& rhs)
	{
		return (lhs.M[0][0] == rhs.M[0][0] && lhs.M[0][1] == rhs.M[0][1] && lhs.M[0][2] == rhs.M[0][2] 
			&& lhs.M[1][0] == rhs.M[1][0] && lhs.M[1][1] == rhs.M[1][1] && lhs.M[1][2] == rhs.M[1][2] 
			&& lhs.M[2][0] == rhs.M[2][0] && lhs.M[2][1] == rhs.M[2][1] && lhs.M[2][2] == rhs.M[2][2]);
	}

	template <class T>
	constexpr bool operator != (_Matrix<T, 3, 3> const& lhs, _Matrix<T, 3, 3> const& rhs)
	{
		return (lhs.M[0][0] != rhs.M[0][0] || lhs.M[0][1] != rhs.M[0][1] || lhs.M[0][2] != rhs.M[0][2]
			|| lhs.M[1][0] != rhs.M[1][0] || lhs.M[1][1] != rhs.M[1][1] || lhs.M[1][2] != rhs.M[1][2]
			|| lhs.M[2][0] != rhs.M[2][0] || lhs.M[2][1] != rhs.M[2][1] || lhs.M[2][2] != rhs.M[2][2]);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Template partial specialization matrix Version_4_4
	//
	//////////////////////////////////////////////////////////////////////////

	template <class T>
	class _Matrix<T, 4, 4>
	{
	public: // Static and Macro

		static _Matrix const ZERO;
		static _Matrix const IDENTITY;

		static constexpr UINT32 Row = 4;
		static constexpr UINT32 Col = 4;
		static constexpr UINT32 Count = Row * Col;


	public: //	Constructions and Destructions

		constexpr _Matrix() = default;
		_Matrix(_Vector<T, Col> const& c0, _Vector<T, Col> const& c1, _Vector<T, Col> const& c2, _Vector<T, Col> const& c3) : M{ c0, c1, c2, c3 } {}
		_Matrix(_Vector<_Vector<T, Col>, Row> const& _mat) : M(_mat) {}
		_Matrix(_Matrix const& mat) : M(mat.M) {}

		// Promise T is value type
		_Matrix(T const& _m)
			: M{ { _m, T(0), T(0), T(0) }
			, { T(0), _m, T(0), T(0) }
			, { T(0), T(0), _m, T(0) }
			, { T(0), T(0), T(0), _m } } {}

		// Column major matrix
		_Matrix(T const& m11, T const& m21, T const& m31, T const& m41
			, T const& m12, T const& m22, T const& m32, T const& m42
			, T const& m13, T const& m23, T const& m33, T const& m43
			, T const& m14, T const& m24, T const& m34, T const& m44)
			: M{ { m11, m21, m31, m41 },{ m12, m22, m32, m42 },{ m13, m23, m33, m43 },{ m14, m24, m34, m44 } } {}

	public: // Attributes

		union
		{
			struct
			{
				T	_11, _21, _31, _41,
					_12, _22, _32, _42,
					_13, _23, _33, _43,
					_14, _24, _34, _44;
			};
			_Vector<_Vector<T, Col>, Row> M;
		};

	public: // Operations

		constexpr _Matrix& operator = (_Matrix const& rhs)
		{
			_11 = rhs._11; _21 = rhs._21; _31 = rhs._31; _41 = rhs._41;
			_12 = rhs._12; _22 = rhs._22; _32 = rhs._32; _42 = rhs._42;
			_13 = rhs._13; _23 = rhs._23; _33 = rhs._33; _43 = rhs._43;
			_14 = rhs._14; _24 = rhs._24; _34 = rhs._34; _44 = rhs._44;
			return *this;
		}

		constexpr _Matrix const& operator += (_Matrix const& rhs)
		{
			_11 += rhs._11; _21 += rhs._21; _31 += rhs._31; _41 += rhs._41;
			_12 += rhs._12; _22 += rhs._22; _32 += rhs._32; _42 += rhs._42;
			_13 += rhs._13; _23 += rhs._23; _33 += rhs._33; _43 += rhs._43;
			_14 += rhs._14; _24 += rhs._24; _34 += rhs._34; _44 += rhs._44;
			return *this;
		}

		constexpr _Matrix const& operator -= (_Matrix const& rhs)
		{
			_11 -= rhs._11; _21 -= rhs._21; _31 -= rhs._31; _41 -= rhs._41;
			_12 -= rhs._12; _22 -= rhs._22; _32 -= rhs._32; _42 -= rhs._42;
			_13 -= rhs._13; _23 -= rhs._23; _33 -= rhs._33; _43 -= rhs._43;
			_14 -= rhs._14; _24 -= rhs._24; _34 -= rhs._34; _44 -= rhs._44;
			return *this;
		}

		constexpr _Matrix const& operator *= (T const& rhs)
		{
			_11 *= rhs._11; _21 *= rhs._21; _31 *= rhs._31; _41 *= rhs._41;
			_12 *= rhs._12; _22 *= rhs._22; _32 *= rhs._32; _42 *= rhs._42;
			_13 *= rhs._13; _23 *= rhs._23; _33 *= rhs._33; _43 *= rhs._43;
			_14 *= rhs._14; _24 *= rhs._24; _34 *= rhs._34; _44 *= rhs._44;
			return *this;
		}

		constexpr _Matrix const& operator /= (T const& rhs)
		{
			T reciprocal = T(1) / lhs;
			_11 *= reciprocal; _21 *= reciprocal; _31 *= reciprocal; _41 *= reciprocal;
			_12 *= reciprocal; _22 *= reciprocal; _32 *= reciprocal; _42 *= reciprocal;
			_13 *= reciprocal; _23 *= reciprocal; _33 *= reciprocal; _43 *= reciprocal;
			_14 *= reciprocal; _24 *= reciprocal; _34 *= reciprocal; _44 *= reciprocal;
			return *this;
		}

		constexpr _Matrix const& operator *= (_Matrix const& rhs)
		{
			_11 = _11 * rhs._11 + _12 * rhs._21 + _13 * rhs._31 + _14 * rhs._41;
			_21 = _21 * rhs._11 + _22 * rhs._21 + _23 * rhs._31 + _24 * rhs._41;
			_31 = _31 * rhs._11 + _32 * rhs._21 + _33 * rhs._31 + _34 * rhs._41;
			_41 = _41 * rhs._11 + _42 * rhs._21 + _43 * rhs._31 + _44 * rhs._41;
			_12 = _11 * rhs._12 + _12 * rhs._22 + _13 * rhs._32 + _14 * rhs._42;
			_22 = _21 * rhs._12 + _22 * rhs._22 + _23 * rhs._32 + _24 * rhs._42;
			_32 = _31 * rhs._12 + _32 * rhs._22 + _33 * rhs._32 + _34 * rhs._42;
			_41 = _41 * rhs._12 + _42 * rhs._22 + _43 * rhs._32 + _44 * rhs._42;
			_13 = _11 * rhs._13 + _12 * rhs._23 + _13 * rhs._33 + _14 * rhs._43;
			_23 = _21 * rhs._13 + _22 * rhs._23 + _23 * rhs._33 + _24 * rhs._43;
			_33 = _31 * rhs._13 + _32 * rhs._23 + _33 * rhs._33 + _34 * rhs._43;
			_31 = _41 * rhs._13 + _42 * rhs._21 + _43 * rhs._33 + _44 * rhs._43;
			_31 = _11 * rhs._14 + _12 * rhs._24 + _13 * rhs._34 + _14 * rhs._44;
			_31 = _21 * rhs._14 + _22 * rhs._24 + _23 * rhs._34 + _24 * rhs._44;
			_31 = _31 * rhs._14 + _32 * rhs._24 + _33 * rhs._34 + _34 * rhs._44;
			_31 = _41 * rhs._14 + _42 * rhs._24 + _43 * rhs._34 + _44 * rhs._44;

			return *this;
		}

		constexpr _Vector<T, Col> const& operator[](UINT32 index) const
		{
			assert(index < Row);
			return M[index];
		}

		constexpr _Vector<T, Col>& operator[](UINT32 index)
		{
			assert(index < Row);
			return M[index];
		}

		constexpr _Vector<T, Col> const GetRow(UINT32 index) const
		{
			return _Vector<T, Col>(v[0][index], v[1][index], v[2][index], v[3][index]);
		}

		constexpr void multisetRow(UINT32 index, _Vector<T, Col> const& rv)
		{
			v[0][index] = rv.v[0];
			v[1][index] = rv.v[1];
			v[2][index] = rv.v[2];
			v[3][index] = rv.v[3];
		}

		constexpr _Vector<T, Row> const& GetColumn(UINT32 index) const
		{
			return M[index];
		}

		constexpr void multisetCol(UINT32 index, _Vector<T, Row> const& rv)
		{
			v[index][0] = rv.v[0];
			v[index][1] = rv.v[1];
			v[index][2] = rv.v[2];
			v[index][3] = rv.v[3];
		}

		constexpr _Matrix operator-() const
		{
			return _Matrix(-M);
		}

		constexpr void Transpose()
		{
			T m21 = M[0][1];
			T m31 = M[0][2];
			T m41 = M[0][3];
			T m32 = M[1][2];
			T m42 = M[1][3];
			T m43 = M[2][3];

			M[0][1] = M[1][0]; M[0][2] = M[2][0]; M[0][3] = M[3][0];
			M[1][2] = M[2][1]; M[1][3] = M[3][1]; M[2][3] = M[3][2];

			M[1][0] = m21; M[2][0] = m31; M[3][0] = m41;
			M[2][1] = m32; M[3][1] = m42; M[3][2] = m43;
		}

		constexpr _Matrix Transposed() const
		{
			return _Matrix(
				M[0][0], M[1][0], M[2][0], M[3][0],
				M[0][1], M[1][1], M[2][1], M[3][1],
				M[0][2], M[1][2], M[2][2], M[3][2],
				M[0][3], M[1][3], M[2][3], M[3][3]);
		}

		constexpr T Determinant() const 
		{
			T	m11 = v[0][0], m21 = v[1][0], m31 = v[2][0], m41 = v[3][0],
				m12 = v[0][1], m22 = v[1][1], m32 = v[2][1], m42 = v[3][1],
				m13 = v[0][2], m23 = v[1][2], m33 = v[2][2], m43 = v[3][2],
				m14 = v[0][3], m24 = v[1][3], m34 = v[2][3], m44 = v[3][3];

			T	_1122_2112 = m11 * m22 - m21 * m12,
				_1132_3112 = m11 * m32 - m31 * m12,
				_1142_4112 = m11 * m42 - m41 * m12,
				_2132_3122 = m21 * m32 - m31 * m22,
				_2142_4122 = m21 * m42 - m41 * m22,
				_3142_4132 = m31 * m42 - m41 * m32,
				_1324_2314 = m13 * m24 - m23 * m14,
				_1334_3314 = m13 * m34 - m33 * m14,
				_1344_4314 = m13 * m44 - m43 * m14,
				_2334_3324 = m23 * m34 - m33 * m24,
				_2344_4324 = m23 * m44 - m43 * m24,
				_3344_4334 = m33 * m44 - m43 * m34;

			return (_1122_2112 * _3344_4334	- _1132_3112 * _2344_4324
				+ _1142_4112 * _2334_3324 + _2132_3122 * _1344_4314
				- _2142_4122 * _1334_3314 + _3142_4132 * _1324_2314);
		}

		constexpr _Matrix Inversed() const 
		{
			T	m11 = M[0][0], m21 = M[1][0], m31 = M[2][0], m41 = M[3][0],
				m12 = M[0][1], m22 = M[1][1], m32 = M[2][1], m42 = M[3][1],
				m13 = M[0][2], m23 = M[1][2], m33 = M[2][2], m43 = M[3][2],
				m14 = M[0][3], m24 = M[1][3], m34 = M[2][3], m44 = M[3][3];

			T	_1122_2112 = m11 * m22 - m21 * m12,
				_1132_3112 = m11 * m32 - m31 * m12,
				_1142_4112 = m11 * m42 - m41 * m12,
				_2132_3122 = m21 * m32 - m31 * m22,
				_2142_4122 = m21 * m42 - m41 * m22,
				_3142_4132 = m31 * m42 - m41 * m32,
				_1324_2314 = m13 * m24 - m23 * m14,
				_1334_3314 = m13 * m34 - m33 * m14,
				_1344_4314 = m13 * m44 - m43 * m14,
				_2334_3324 = m23 * m34 - m33 * m24,
				_2344_4324 = m23 * m44 - m43 * m24,
				_3344_4334 = m33 * m44 - m43 * m34;

			T determinant = (_1122_2112 * _3344_4334 - _1132_3112 * _2344_4324
				+ _1142_4112 * _2334_3324 + _2132_3122 * _1344_4314
				- _2142_4122 * _1334_3314 + _3142_4132 * _1324_2314);

			// non-invertible
			if (determinant = T(0))
			{
				assert(false);
				return _Matrix(T(1));
			}

			T inverseDeterminant = T(1) / determinant;

			return _Matrix(
				(m22 * _3344_4334 - m32 * _2344_4324 + m42 * _2334_3324) * inverseDeterminant,
				(-m21 * _3344_4334 + m31 * _2344_4324 - m41 * _2334_3324) * inverseDeterminant,
				(m24 * _3142_4132 - m34 * _2142_4122 + m44 * _2132_3122) * inverseDeterminant,
				(-m23 * _3142_4132 + m33 * _2142_4122 - m43 * _2132_3122) * inverseDeterminant,
				(-m12 * _3344_4334 + m32 * _1344_4314 - m42 * _1334_3314) * inverseDeterminant,
				(m11 * _3344_4334 - m31 * _1344_4314 + m41 * _1334_3314) * inverseDeterminant,
				(-m14 * _3142_4132 + m34 * _1142_4112 - m44 * _1132_3112) * inverseDeterminant,
				(m13 * _3142_4132 - m33 * _1142_4112 + m43 * _1132_3112) * inverseDeterminant,
				(m12 * _2344_4324 - m22 * _1344_4314 + m42 * _1324_2314) * inverseDeterminant,
				(-m11 * _2344_4324 + m21 * _1344_4314 - m41 * _1324_2314) * inverseDeterminant,
				(m14 * _2142_4122 - m24 * _1142_4112 + m44 * _1122_2112) * inverseDeterminant,
				(-m13 * _2142_4122 + m23 * _1142_4112 - m43 * _1122_2112) * inverseDeterminant,
				(-m12 * _2334_3324 + m22 * _1334_3314 - m32 * _1324_2314) * inverseDeterminant,
				(m11 * _2334_3324 - m21 * _1334_3314 + m31 * _1324_2314) * inverseDeterminant,
				(-m14 * _2132_3122 + m24 * _1132_3112 - m34 * _1122_2112) * inverseDeterminant,
				(m13 * _2132_3122 - m23 * _1132_3112 + m33 * _1122_2112) * inverseDeterminant);
		}


		T const* Data() const
		{
			return M.Data()->Data();
		}

	};

	template <class T>
	_Matrix<T, 4, 4> const _Matrix<T, 4, 4>::ZERO = _Matrix(T(0));

	template <class T>
	_Matrix<T, 4, 4> const _Matrix<T, 4, 4>::IDENTITY = _Matrix(T(1));

	template <class T>
	constexpr bool operator == (_Matrix<T, 4, 4> const& lhs, _Matrix<T, 4, 4> const& rhs)
	{
		return (lhs.M[0][0] == rhs.M[0][0] && lhs.M[0][1] == rhs.M[0][1] && lhs.M[0][2] == rhs.M[0][2] && lhs.M[0][3] == rhs.M[0][3]
			&& lhs.M[1][0] == rhs.M[1][0] && lhs.M[1][1] == rhs.M[1][1] && lhs.M[1][2] == rhs.M[1][2] && lhs.M[1][3] == rhs.M[1][3]
			&& lhs.M[2][0] == rhs.M[2][0] && lhs.M[2][1] == rhs.M[2][1] && lhs.M[2][2] == rhs.M[2][2] && lhs.M[2][3] == rhs.M[2][3]
			&& lhs.M[3][0] == rhs.M[3][0] && lhs.M[3][1] == rhs.M[3][1] && lhs.M[3][2] == rhs.M[3][2] && lhs.M[3][3] == rhs.M[3][3]);
	}

	template <class T>
	constexpr bool operator != (_Matrix<T, 4, 4> const& lhs, _Matrix<T, 4, 4> const& rhs)
	{
		return (lhs.M[0][0] != rhs.M[0][0] || lhs.M[0][1] != rhs.M[0][1] || lhs.M[0][2] != rhs.M[0][2] || lhs.M[0][3] != rhs.M[0][3]
			|| lhs.M[1][0] != rhs.M[1][0] || lhs.M[1][1] != rhs.M[1][1] || lhs.M[1][2] != rhs.M[1][2] || lhs.M[1][3] != rhs.M[1][3]
			|| lhs.M[2][0] != rhs.M[2][0] || lhs.M[2][1] != rhs.M[2][1] || lhs.M[2][2] != rhs.M[2][2] || lhs.M[2][3] != rhs.M[2][3]
			|| lhs.M[3][0] != rhs.M[3][0] || lhs.M[3][1] != rhs.M[3][1] || lhs.M[3][2] != rhs.M[3][2] || lhs.M[3][3] != rhs.M[3][3]);
	}

	template<class T, UINT32 Row, UINT32 Col>
	constexpr _Matrix<T, Row, Col> operator + (_Matrix<T, Row, Col> const& lhs, _Matrix<T, Row, Col> const& rhs) 
	{
		return _Matrix<T, Row, Col>(lhs) += rhs;
	}
	template<class T, UINT32 Row, UINT32 Col>
	constexpr _Matrix<T, Row, Col> operator - (_Matrix<T, Row, Col> const& lhs, _Matrix<T, Row, Col> const& rhs) 
	{
		return _Matrix<T, Row, Col>(lhs) -= rhs;
	}

	template<class T, UINT32 Row, UINT32 Col>
	constexpr _Matrix<T, Row, Col> operator *(_Matrix<T, Row, Col> const& lhs, T const& rhs) 
	{
		return _Matrix<T, Row, Col>(lhs) *= rhs;
	}
	template<class T, UINT32 Row, UINT32 Col>
	constexpr _Matrix<T, Row, Col> operator * (T const& lhs, _Matrix<T, Row, Col> const& rhs) 
	{
		return _Matrix<T, Row, Col>(rhs) *= lhs;
	}

	template<class T, UINT32 Row, UINT32 Col>
	constexpr _Matrix<T, Row, Col> operator * (_Matrix<T, Row, Col>  const& lhs, _Matrix<T, Row, Col> const& rhs)
	{
		return _Matrix<T, Row, Col>(rhs) *= lhs;
	}

	template<class T, UINT32 Row, UINT32 Col>
	constexpr _Matrix<T, Row, Col> operator / (_Matrix<T, Row, Col> const& lhs, T const& rhs) 
	{
		return _Matrix<T, Row, Col>(lhs) / rhs;
	}

	using Mat3F32 = _Matrix<FLOAT32, 3, 3>;
	using Mat3F64 = _Matrix<FLOAT64, 3, 3>;

	using Mat4F32 = _Matrix<FLOAT32, 4, 4>;
	using Mat4F64 = _Matrix<FLOAT64, 4, 4>;
}

