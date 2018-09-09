#pragma once

class Vector1
{
public:
	explicit Vector1(float inV) : v(inV) {}
	float v;
};

template <typename Container, int index0>
class Vector1Converter
{
public:

	float data[3];

	//float& operator[] (int index) { return v[index]; }
	operator Vector1() const
	{
		return Vector1( data[index0] );
	}
};

class Vector2
{
public:
	explicit Vector2(float inV1, float inV2) 
		: v{ inV1, inV2 }
	{}
	float v[2];
};

template <typename Container, int index0, int index1>
class Vector2Converter
{
public:

	float data[3];

	//float& operator[] (int index) { return v[index]; }
	operator Vector2() const
	{
		return Vector2( data[index0], data[index1] );
	}
};

class Vector3
{
public:

	Vector3()
		: xyz{ 1.f, 2.f, 3.f }
	{}

	//float& operator[] (int index) { return xyz[index]; }

	union
	{
		Vector1Converter<Vector3, 0>		x;
		Vector1Converter<Vector3, 1>		y;
		Vector1Converter<Vector3, 2>		z;
		Vector2Converter<Vector3, 0, 1>		xy;
		Vector2Converter<Vector3, 1, 0>		yx;
		Vector2Converter<Vector3, 1, 2>		yz;
		Vector2Converter<Vector3, 2, 1>		zy;
		Vector2Converter<Vector3, 0, 2>		xz;
		Vector2Converter<Vector3, 2, 0>		zx;
		struct { float _x, _y, _z; } xyz;
	};
};