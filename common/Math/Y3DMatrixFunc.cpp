#include "Y3DMatrixFunc.h"

namespace Y3D
{
	//////////////////////////////////////////////////////////////////////////
	//
	// Matrix common manipulation;
	//
	//////////////////////////////////////////////////////////////////////////

	// Left transform version is vec * mat
	// Tight transform version is mat * vec
	Vec3F32 _Mat4_Transform_Left(Vec3F32 const& vec, Mat4F32 const& mat)
	{
		return Vec3F32(
			Dot(Vec4F32(vec, 1), Vec4F32(mat._11, mat._21, mat._31, mat._41)),
			Dot(Vec4F32(vec, 1), Vec4F32(mat._12, mat._22, mat._32, mat._42)),
			Dot(Vec4F32(vec, 1), Vec4F32(mat._13, mat._23, mat._33, mat._43))
		);
	}

	Vec3F32 _Mat4_Transform_Right(Mat4F32 const& mat, Vec3F32 const& vec)
	{
		return Vec3F32(
			Dot(Vec4F32(mat._11, mat._12, mat._13, mat._14), Vec4F32(vec, 1)),
			Dot(Vec4F32(mat._21, mat._22, mat._23, mat._24), Vec4F32(vec, 1)),
			Dot(Vec4F32(mat._31, mat._32, mat._33, mat._34), Vec4F32(vec, 1))
		);
	}

	Vec3F32 _Mat4_Transform_Left_Direct(Vec3F32 const& vec, Mat4F32 const& mat)
	{
		return Vec3F32(
			Dot(vec, Vec3F32(mat._11, mat._21, mat._31)),
			Dot(vec, Vec3F32(mat._12, mat._22, mat._32)),
			Dot(vec, Vec3F32(mat._13, mat._23, mat._33))
		);
	}

	Vec3F32 _Mat4_Transform_Right_Direct(Mat4F32 const& mat, Vec3F32 const& vec)
	{
		return Vec3F32(
			Dot(Vec3F32(mat._11, mat._12, mat._13), vec),
			Dot(Vec3F32(mat._21, mat._22, mat._23), vec),
			Dot(Vec3F32(mat._31, mat._32, mat._33), vec)
		);
	}

	// Mat3 is _11,_12,_13,_21,_22,_23,_31,_32,_33 in mat4
	// Vec3 is _41,_42,_43 in mat3
	Mat3F32 _Get_Mat3_From_Mat4(Mat4F32 const& mat)
	{
		const float s = mat[0][0];
		return Mat3F32(
			mat.M[0][0], mat.M[0][1], mat.M[0][2],
			mat.M[1][0], mat.M[1][1], mat.M[1][2],
			mat.M[2][0], mat.M[2][1], mat.M[2][2]
		);
	}

	Vec3F32 _Get_Vec3_From_Mat4(Mat4F32 const& mat)
	{
		return Vec3F32(mat.M[3][0], mat.M[3][1], mat.M[3][2]);
	}

	// mat4's _11,_12,_13,_21,_22,_23,_31,_32,_33 is mat3
	// mat4's is _41,_42,_43 is vec3
	Mat4F32 _multiset_Mat3_Vec3_To_Mat4(Mat3F32 const& mat, Vec3F32 const& vec)
	{
		return Mat4F32(
			mat[0][0], mat[0][1], mat[0][2], 0.f,
			mat[1][0], mat[1][1], mat[1][2], 0.f,
			mat[2][0], mat[2][1], mat[2][2], 0.f,
			vec[0], vec[1], vec[2], 1.f
		);
	}

	// Generate mat3 by (vec3).transpose * vec3
	Mat3F32 _multiset_Vec3Tr_Vec3_To_Mat3(Vec3F32 const& vecTp, Vec3F32 const& vec)
	{
		return Mat3F32(
			vecTp[0] * vec[0], vecTp[0] * vec[1], vecTp[0] * vec[2],
			vecTp[1] * vec[0], vecTp[1] * vec[1], vecTp[1] * vec[2],
			vecTp[2] * vec[0], vecTp[2] * vec[1], vecTp[2] * vec[2]
		);
	}

	// Generate mat4 by (vec4).transpose * vec4
	Mat4F32 _multiset_Vec4Tr_Vec4_To_Mat4(Vec4F32 const& vecTp, Vec4F32 const& vec)
	{
		return Mat4F32(
			vecTp[0] * vec[0], vecTp[0] * vec[1], vecTp[0] * vec[2], vecTp[0] * vec[3],
			vecTp[1] * vec[0], vecTp[1] * vec[1], vecTp[1] * vec[2], vecTp[1] * vec[3],
			vecTp[2] * vec[0], vecTp[2] * vec[1], vecTp[2] * vec[2], vecTp[2] * vec[3],
			vecTp[3] * vec[0], vecTp[3] * vec[1], vecTp[3] * vec[2], vecTp[3] * vec[3]
		);
	}

	// Calculate the different between two orientation matrix
	// If right matrix is orthogonal, use transpose version
	// If right matrix is not orthogonal, use inverse version
	Mat4F32 _Tranform_Left_Transpose(Mat4F32 const& mat, Mat4F32 const& matTp)
	{
		Mat3F32 m3RT = _Get_Mat3_From_Mat4(matTp).Transposed();
		Mat3F32 m3L = _Get_Mat3_From_Mat4(mat) * m3RT;

		return _multiset_Mat3_Vec3_To_Mat4(m3L, Vec3F32(0.f));
	}

	Mat4F32 _Tranform_Left_Inverse(Mat4F32 const& mat, Mat4F32 const& matInv)
	{
		Mat3F32 m3RT = _Get_Mat3_From_Mat4(matInv).Inversed();
		Mat3F32 m3L = _Get_Mat3_From_Mat4(mat) * m3RT;

		return _multiset_Mat3_Vec3_To_Mat4(m3L, Vec3F32(0.f));
	}

	Mat4F32 _Tranform_Right_Transpose(Mat4F32 const& matTp, Mat4F32 const& mat)
	{
		Mat3F32 m3LT = _Get_Mat3_From_Mat4(matTp).Transposed();
		Mat3F32 m3R = m3LT * _Get_Mat3_From_Mat4(mat);

		return _multiset_Mat3_Vec3_To_Mat4(m3R, Vec3F32(0.f));
	}

	Mat4F32 _Tranform_Right_Inverse(Mat4F32 const& matInv, Mat4F32 const& mat)
	{
		Mat3F32 m3LT = _Get_Mat3_From_Mat4(matInv).Transposed();
		Mat3F32 m3R = m3LT * _Get_Mat3_From_Mat4(mat);

		return _multiset_Mat3_Vec3_To_Mat4(m3R, Vec3F32(0.f));
	}

	//
	// Transform uses left version 
	// Transform direct is different from transform, it's translation partition is invalid
	// Get translation partition in mat4
	// Get Rotation partition in mat3
	//
	Vec3F32 Transform(Mat4F32 const& mat, Vec3F32 const& vec)
	{
		// We use left-multiply convention in DirectX
		return _Mat4_Transform_Left(vec, mat);
	}

	Vec3F32 TransformDirect(Mat4F32 const& mat, Vec3F32 const& vec)
	{
		return _Mat4_Transform_Left_Direct(vec, mat);
	}

	Vec3F32 GetTranslation(Mat4F32 const& mat)
	{
		return _Get_Vec3_From_Mat4(mat);
	}

	Mat3F32 GetRotation(Mat4F32 const& mat)
	{
		// Attention: column major matrix or row major matrix
		return _Get_Mat3_From_Mat4(mat);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Matrix initialize 
	//
	//////////////////////////////////////////////////////////////////////////

	// Transform matrix initialize
	Mat4F32 InitiateTransform(Vec3F32 const& dir, Vec3F32 const& up, Vec3F32 const& pos)
	{
		Vec3F32 xAxis;
		Vec3F32 yAxis = up.Normalized();
		Vec3F32 zAxis = dir.Normalized();

		float32 aDirAndUp = Dot(yAxis, zAxis);
		if (aDirAndUp != 0)
		{
			xAxis = Cross(yAxis, xAxis).Normalized();
			yAxis = Cross(zAxis, xAxis).Normalized();
		}

		return Mat4F32(
			xAxis[0], xAxis[1], xAxis[2], 0.f,
			yAxis[0], yAxis[1], yAxis[2], 0.f,
			zAxis[0], zAxis[1], zAxis[2], 0.f,
			pos[0], pos[1], pos[2], 1.f
		);
	}

	// Translation matrix initialize
	Mat4F32 InitiateTranslationMatrix(float32 x, float32 y, float32 z)
	{
		Mat4F32 mat = Mat4F32::IDENTITY;

		mat[3][0] = x;
		mat[3][1] = y;
		mat[3][2] = z;

		return mat;
	}

	Mat4F32 InitiateTranslationMatrix(Vec3F32 tv)
	{
		return InitiateTranslationMatrix(tv.x, tv.y, tv.z);
	}

	// Scale matrix initialize
	Mat4F32 InitiateScaleMatrix(float32 s)
	{
		return InitiateScaleMatrix(s, s, s);
	}

	Mat4F32 InitiateScaleMatrix(float32 sx, float32 sy, float32 sz)
	{
		Mat4F32 mat = Mat4F32::IDENTITY;

		mat[0][0] = sx;
		mat[1][1] = sy;
		mat[2][2] = sz;

		return mat;
	}
	Mat4F32 InitiateScaleMatrix(Vec3F32 const& sv)
	{
		return InitiateScaleMatrix(sv.x, sv.y, sv.z);
	}

	// Scale matrix by arbitrary axis initialize
	Mat4F32 IntitateAlongAxisScaleMatrix(Vec3F32 const& axis, float32 s)
	{
		// M = R * S * (R)^-1
		// **
		// Define W is a unit-length direction along which the scaling, and W = Cross(U, V).
		// X0 = P + U * y0 + V * y1 + W * y2
		// X1 = P + U * y0 + V * y1 + W * y2 * s
		// **
		// I = U * U^T + V * V^T + W * W^T.
		// M = R * S * (R)^-1 
		//   = U * U^T + V * V^T + W * W^T * s
		//   = I + W * W^T * (s - 1)
		// **
		// M[i,j] = (s - 1) * W[i] * W[j] + (i == j ? 1: 0)

		//Mat3F32 mat = Mat3F32::IDENTITY;
		//Mat3F32 sMat = _multiset_Vec3Tr_Vec3_To_Mat3(axis, axis);
		//mat += (s - 1.f) * sMat;
		//return _multiset_Mat3_Vec3_To_Mat4(mat, { 0.f,0.f,0.f });

		return Mat4F32(
			(s - 1.f) * axis.x * axis.x + 1, (s - 1.f) * axis.x * axis.y, (s - 1.f) * axis.x * axis.z, 0.f,
			(s - 1.f) * axis.y * axis.x, (s - 1.f) * axis.y * axis.y + 1, (s - 1.f) * axis.y * axis.z, 0.f,
			(s - 1.f) * axis.z * axis.x, (s - 1.f) * axis.z * axis.y, (s - 1.f) * axis.z * axis.z + 1, 0.f,
			0.f, 0.f, 0.f, 1.f
		);
	}

	// Rotate matrix initialize
	// Euler form : X,Y,Z three version
	// Axis-Angle form : one version 
	// quaternion form : one version
	Mat4F32 InitiateEulerXMatrix(RadianF32 const& angle)
	{
		float32 fCosX = Cos(angle);
		float32 fSinX = Sin(angle);

		Mat4F32 mat = Mat4F32::IDENTITY;

		mat[1][1] = fCosX;
		mat[1][2] = -fSinX;
		mat[2][1] = fSinX;
		mat[2][2] = fCosX;

		return mat;
	}

	Mat4F32 InitiateEulerYMatrix(RadianF32 const& angle)
	{
		float32 fCosX = Cos(angle);
		float32 fSinX = Sin(angle);

		Mat4F32 mat = Mat4F32::IDENTITY;

		mat[0][0] = fCosX;
		mat[0][2] = fSinX;
		mat[2][0] = -fSinX;
		mat[2][2] = fCosX;

		return mat;
	}

	Mat4F32 InitiateEulerZMatrix(RadianF32 const& angle)
	{
		float32 fCosX = Cos(angle);
		float32 fSinX = Sin(angle);

		Mat4F32 mat = Mat4F32::IDENTITY;

		mat[0][0] = fCosX;
		mat[0][1] = -fSinX;
		mat[1][0] = fSinX;
		mat[1][1] = fCosX;

		return mat;
	}

	Mat4F32 InitiateAxisAngleMatrix(RadianF32 const& angle, float32 x, float32 y, float32 z)
	{
		return InitiateAxisAngleMatrix(angle, Vec3F32(x, y, z));
	}

	Mat4F32 InitiateAxisAngleMatrix(RadianF32 const& angle, Vec3F32 const& axis)
	{
		// M = Rot2yAxis * R * (Rot2yAxis)^-1
		//(1−cos φ)*rx*rx + cos φ,		(1−cos φ)*rx*ry − rz*sin φ,		(1−cos φ)*rx*rz + ry*sin φ
		//(1−cos φ)*rx*ry + rz*sin φ,	(1−cos φ)*ry*ry + cos φ,		(1−cos φ)*ry*rz − rx*sin φ
		//(1−cos φ)*rx*rz − ry*sin φ,	(1−cos φ)*ry*rz + rx*sin φ,		(1−cos φ)*rz*rz + cos φ
		//
		if (axis.MagnitudeSquared() == 0)
		{
			return Mat4F32::ZERO;
		}

		Vec3F32 axisNorm = axis.Normalized();

		float32 fCosX = Cos(angle);
		float32 fSinX = Sin(angle);
		float32 tfCosX = 1 - fCosX;

		float32 rx = axis.x;
		float32 ry = axis.y;
		float32 rz = axis.z;

		Mat4F32 mat;

		mat[0][0] = rx * rx * tfCosX + fCosX;
		mat[0][1] = rx * ry * tfCosX - rz * fSinX;
		mat[0][2] = rx * rz * tfCosX + ry * fSinX;
		mat[0][3] = 0.f;

		mat[1][0] = ry * rx * tfCosX - rz * fSinX;
		mat[1][1] = ry * ry * tfCosX + fCosX;
		mat[1][2] = ry * rz * tfCosX + rx * fSinX;
		mat[1][3] = 0.f;

		mat[2][0] = rz * rx * tfCosX - ry * fSinX;
		mat[2][1] = rz * ry * tfCosX + rx * fSinX;
		mat[2][2] = rz * rz * tfCosX + fCosX;
		mat[2][3] = 0.f;

		mat[3][0] = 0.f;
		mat[3][1] = 0.f;
		mat[3][2] = 0.f;
		mat[3][3] = 1.f;

		return mat;
	}

	Mat4F32 InitiateQuaternionMatrx(QuatF32 const& qt)
	{
		//Convert quaternion to matrix

		return Mat4F32::IDENTITY;
	}

	//
	// Look at view-matrix desc:
	// eye: eye position in global space
	// lDir: eye front direction in global space
	// lUp: eye up direction in local space
	//
	Mat4F32 LookAtViewMatrix(Vec3F32 const& eye, Vec3F32 const& lDir, Vec3F32 const& lUp)
	{
		if (lDir.MagnitudeSquared() == 0 || lUp.MagnitudeSquared() == 0)
		{
			return Mat4F32::IDENTITY;
		}

		Vec3F32 zAxis(lDir.Normalized());
		Vec3F32 xAxis(Cross(lUp, zAxis).Normalized());
		Vec3F32 yAxis(Cross(zAxis, xAxis));

		Mat4F32 mat;

		mat[0][0] = xAxis.x;
		mat[1][0] = xAxis.y;
		mat[2][0] = xAxis.z;
		mat[3][0] = 0.f;

		mat[0][1] = yAxis.x;
		mat[1][1] = yAxis.y;
		mat[2][1] = yAxis.z;
		mat[3][1] = 0.f;

		mat[0][2] = zAxis.x;
		mat[1][2] = zAxis.y;
		mat[2][2] = zAxis.z;
		mat[3][2] = 0.f;

		mat[0][3] = -Dot(xAxis, eye);
		mat[1][3] = -Dot(yAxis, eye);
		mat[2][3] = -Dot(zAxis, eye);
		mat[3][3] = 1.f;

		return mat;
	}

	//
	// Face at view-matrix desc:
	// gDir: front direction in global space
	// gUp: up direction in global space
	// lFaceDir: face front direction in local space
	// lFaceUp: face up direction in local space
	//
	Mat4F32 FaceAtViewMatrix(Vec3F32 const& gDir,
		Vec3F32 const& gUp,
		Vec3F32 const& lFaceDir,
		Vec3F32 const& lFaceUp)
	{
		if (gDir.MagnitudeSquared() == 0 ||
			gUp.MagnitudeSquared() == 0 ||
			lFaceDir.MagnitudeSquared() == 0 ||
			lFaceUp.MagnitudeSquared() == 0)
		{
			return Mat4F32::IDENTITY;
		}

		Mat4F32 matGlobal = InitiateTransform(gDir, gUp, Vec3F32::IDENTITY);
		Mat4F32 matLocal = InitiateTransform(gDir, gUp, Vec3F32::IDENTITY);

		return _Tranform_Left_Transpose(matGlobal, matLocal);
	}

	//
	// Frustum Projection Matrix desc:
	// 1. Parameter is perspective projection describe
	// 2. Parameter is six bound in frustum
	//
	Mat4F32 FrustumProjectionMatrix(RadianF32 const& fFOV, float32 fRatio, float32 fNear, float32 fFar)
	{
		float32 fTop = Tan(fFOV * 0.5f) * fNear;
		float32 fRight = fTop * fRatio;

		return FrustumProjectionMatrix(-fRight, fRight, -fTop, fTop, fNear, fFar);
	}

	Mat4F32 FrustumProjectionMatrix(float32 fLeft, float32 fRight, float32 fBottom, float32 fTop, float32 fNear, float32 fFar)
	{
		Mat4F32 mat;

		mat[0][0] = 2.f * fNear / (fRight - fLeft);
		mat[1][0] = 0.f;
		mat[2][0] = 0.f;
		mat[3][0] = 0.f;

		mat[0][1] = 0.f;
		mat[1][1] = 2.f * fNear / (fTop - fBottom);
		mat[2][1] = 0.f;
		mat[3][1] = 0.f;

		mat[0][2] = -(fRight + fLeft) / (fRight - fLeft);
		mat[1][2] = -(fTop + fBottom) / (fTop - fBottom);
		mat[2][2] = (fFar + fNear) / (fFar - fNear);
		mat[3][2] = 1.f;

		mat[0][3] = 0.f;
		mat[1][3] = 0.f;
		mat[2][3] = -2.f * fNear * fFar / (fTop - fBottom);
		mat[3][3] = 0.f;

		return mat;

	}

	//
	// Orthogonal Projection Matrix desc:
	// 1. Parameter is orthogonal projection describe
	// 2. Parameter are width and height in screen
	//
	Mat4F32 OrthogonalProjectionMatrix(float32 fLeft, float32 fRight, float32 fBottom, float32 fTop, float32 fNear, float32 fFar)
	{
		Mat4F32 mat;

		mat[0][0] = 2.f / (fRight - fLeft);
		mat[1][0] = 0.f;
		mat[2][0] = 0.f;
		mat[3][0] = 0.f;

		mat[0][1] = 0.f;
		mat[1][1] = 2.f / (fTop - fBottom);
		mat[2][1] = 0.f;
		mat[3][1] = 0.f;

		mat[0][2] = 0.f;
		mat[1][2] = 0.f;
		mat[2][2] = 2.f / (fTop - fBottom);
		mat[3][2] = 0.f;

		mat[0][3] = -(fRight + fLeft) / (fRight - fLeft);
		mat[1][3] = -(fTop + fBottom) / (fTop - fBottom);
		mat[2][3] = -(fNear) / (fFar - fNear);
		mat[3][3] = 1.f;

		return mat;
	}

	Mat4F32 OrthogonalProjectionMatrix(float32 fWidth, float32 fHeight, float32 fNear, float32 fFar)
	{
		float32 fLeft = fWidth * -0.5f;
		float32 fRight = fWidth * 0.5f;
		float32 fBottom = fHeight * -0.5f;
		float32 fTop = fHeight * 0.5f;

		return OrthogonalProjectionMatrix(fLeft, fRight, fBottom, fLeft, 0.f, fFar - fNear);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Matrix rotate manipulation
	//
	//////////////////////////////////////////////////////////////////////////

	// Multi Euler matrix
	Mat4F32 MultiEulerXMatrix(Mat4F32 const& mat, RadianF32 const& angle)
	{
		Mat4F32 rotXTM = InitiateEulerXMatrix(angle);
		return rotXTM * mat;
	}

	Mat4F32 MultiEulerYMatrix(Mat4F32 const& mat, RadianF32 const& angle)
	{
		Mat4F32 rotYTM = InitiateEulerYMatrix(angle);
		return rotYTM * mat;
	}

	Mat4F32 MultiEulerZMatrix(Mat4F32 const& mat, RadianF32 const& angle)
	{
		Mat4F32 rotZTM = InitiateEulerZMatrix(angle);
		return rotZTM * mat;
	}

	Mat4F32  RotateionFromToMatrix(Vec3F32 const& from, Vec3F32 const& to)
	{
		// Rotation form one vector to another
		// e + h*vx*vx,		h*vx*vy − vz,	hvxvz + vy,		0
		// h*vx*vy + vz,	e + h*vy*vy,	h*vy*vz − vx,	0
		// h*vx*vz − vy,	h*vy*vz + vx,	e + h*vz*vz,	0
		// 0,				0,				0,				1
		//
		if (from.MagnitudeSquared() == 0 || to.MagnitudeSquared() == 0)
		{
			return Mat4F32::ZERO;
		}

		Vec3F32 _from = from.Normalized();
		Vec3F32 _to = to.Normalized();

		Vec3F32 axis = Cross(_from, _to);
		float32 e = Dot(_from, _to);
		float32 h = 1.f / (1.f + e);

		float32 vx = _from.y * _to.z - _from.z * _to.y;
		float32 vy = _from.z * _to.x - _from.x * _to.z;
		float32 vz = _from.x * _to.y - _from.y * _to.x;

		Mat4F32 mat;

		mat[0][0] = e + h * vx * vx;
		mat[0][1] = h * vx * vy + vz;
		mat[0][2] = h * vx * vz - vy;
		mat[0][3] = 0;

		mat[1][0] = h * vx * vy - vz;
		mat[1][1] = e + h * vy * vy;
		mat[1][2] = h * vy * vz + vx;
		mat[1][3] = 0;

		mat[2][0] = h * vx * vz + vy;
		mat[2][1] = h * vy * vz - vx;
		mat[2][2] = e + h * vz * vz;
		mat[2][3] = 0;

		mat[3][0] = 0;
		mat[3][1] = 0;
		mat[3][2] = 0;
		mat[3][3] = 1;

		return mat;
	}
}

