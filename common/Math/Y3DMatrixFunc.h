#pragma once

#include "Y3DMatrix.h"
#include "Y3DPI.h"
#include "Y3DQuaternion.h"

namespace Y3D
{
	//////////////////////////////////////////////////////////////////////////
	//
	// Matrix common manipulation;
	//
	//////////////////////////////////////////////////////////////////////////

	// Left transform version is vec * mat
	// Tight transform version is mat * vec
	Vec3F32 _Mat4_Transform_Left(Vec3F32 const& vec, Mat4F32 const& mat);
	Vec3F32 _Mat4_Transform_Right(Mat4F32 const& mat, Vec3F32 const& vec);

	Vec3F32 _Mat4_Transform_Left_Direct(Vec3F32 const& vec, Mat4F32 const& mat);
	Vec3F32 _Mat4_Transform_Right_Direct(Mat4F32 const& mat, Vec3F32 const& vec);

	// Mat3 is _11,_12,_13,_21,_22,_23,_31,_32,_33 in mat4
	// Vec3 is _41,_42,_43 in mat3
	Mat3F32 _Get_Mat3_From_Mat4(Mat4F32 const& mat);
	Vec3F32 _Get_Vec3_From_Mat4(Mat4F32 const& mat);

	// mat4's _11,_12,_13,_21,_22,_23,_31,_32,_33 is mat3
	// mat4's is _41,_42,_43 is vec3
	Mat4F32 _multiset_Mat3_Vec3_To_Mat4(Mat3F32 const& mat, Vec3F32 const& vec);

	// Generate mat3 by (vec3).transpose * vec3
	Mat3F32 _multiset_Vec3Tr_Vec3_To_Mat3(Vec3F32 const& vecTp, Vec3F32 const& vec);
	// Generate mat4 by (vec4).transpose * vec4
	Mat4F32 _multiset_Vec4Tr_Vec4_To_Mat4(Vec4F32 const& vecTp, Vec4F32 const& vec);

	// Calculate the different between two matrix
	// If right matrix is orthogonal, use transpose version
	// If right matrix is not orthogonal, use inverse version
	Mat4F32 _Tranform_Left_Transpose(Mat4F32 const& mat, Mat4F32 const& matTp);
	Mat4F32 _Tranform_Left_Inverse(Mat4F32 const& mat, Mat4F32 const& matInv);

	Mat4F32 _Tranform_Right_Transpose(Mat4F32 const& matTp, Mat4F32 const& mat);
	Mat4F32 _Tranform_Right_Inverse(Mat4F32 const& matInv, Mat4F32 const& mat);

	//
	// Transform uses left version 
	// Transform direct is different from transform, it's translation partition is invalid
	// Get translation partition in mat4
	// Get Rotation partition in mat3
	//
	Vec3F32 Transform(Mat4F32 const& mat, Vec3F32 const& vec);
	Vec3F32 TransformDirect(Mat4F32 const& mat, Vec3F32 const& vec);
	Vec3F32 GetTranslation(Mat4F32 const& mat);
	Mat3F32 GetRotation(Mat4F32 const& mat);

	//////////////////////////////////////////////////////////////////////////
	//
	// Matrix initialize 
	//
	//////////////////////////////////////////////////////////////////////////

	// Transform matrix initialize
	Mat4F32 InitiateTransform(Vec3F32 const& dir, Vec3F32 const& up, Vec3F32 const& pos);
	
	// Translation matrix initialize
	Mat4F32 InitiateTranslationMatrix(FLOAT32 x, FLOAT32 y, FLOAT32 z);
	Mat4F32 InitiateTranslationMatrix(Vec3F32 tv);

	// Scale matrix initialize
	Mat4F32 InitiateScaleMatrix(FLOAT32 s);
	Mat4F32 InitiateScaleMatrix(FLOAT32 sx, FLOAT32 sy, FLOAT32 sz);
	Mat4F32 InitiateScaleMatrix(Vec3F32 const& sv);
	
	// Scale matrix by arbitrary axis initialize
	Mat4F32 IntitateAlongAxisScaleMatrix(Vec3F32 const& axis, FLOAT32 s);

	// Rotate matrix initialize
	// Euler form : X,Y,Z three version
	// Axis-Angle form : one version 
	// quaternion form : one version
	Mat4F32 InitiateEulerXMatrix(RadianF32 const& angle);
	Mat4F32 InitiateEulerYMatrix(RadianF32 const& angle);
	Mat4F32 InitiateEulerZMatrix(RadianF32 const& angle);
	Mat4F32 InitiateAxisAngleMatrix(RadianF32 const& angle, Vec3F32 const& axis);
	Mat4F32 InitiateAxisAngleMatrix(RadianF32 const& angle, FLOAT32 x, FLOAT32 y, FLOAT32 z);
	Mat4F32 InitiateQuaternionMatrx(QuatF32 const& qt);

	// Reflection matrix initialize
	Mat4F32 InitiateReflectionMatrix(RadianF32 const& angle);
	
	//
	// Look at view-matrix desc:
	// eye: eye position in global space
	// lDir: eye front direction in global space
	// lUp: eye up direction in local space
	//
	Mat4F32 LookAtViewMatrix(Vec3F32 const& eye, Vec3F32 const& lDir, Vec3F32 const& lUp);

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
		Vec3F32 const& lFaceUp);


	//
	// Frustum Projection Matrix desc:
	// 1. Parameter is perspective projection describe
	// 2. Parameter is six bound in frustum
	//

	Mat4F32 FrustumProjectionMatrix(RadianF32 const& fFOV, FLOAT32  fRatio, FLOAT32 fNear, FLOAT32 fFar);
	Mat4F32 FrustumProjectionMatrix(FLOAT32 fLeft, FLOAT32 fRight, FLOAT32 fBottom, FLOAT32 fTop, FLOAT32 fNear, FLOAT32 fFar);

	//
	// Orthogonal Projection Matrix desc:
	// 1. Parameter is orthogonal projection describe
	// 2. Parameter are width and height in screen
	//
	Mat4F32 OrthogonalProjectionMatrix(FLOAT32 fLeft, FLOAT32 fRight, FLOAT32 fBottom, FLOAT32 fTop, FLOAT32 fNear, FLOAT32 fFar);
	Mat4F32 OrthogonalProjectionMatrix(FLOAT32 fWidth, FLOAT32 fHeight, FLOAT32 fNear, FLOAT32 fFar);

	//////////////////////////////////////////////////////////////////////////
	//
	// Matrix rotate manipulation
	//
	//////////////////////////////////////////////////////////////////////////

	// Multi Euler matrix
	Mat4F32 MultiEulerXMatrix(Mat4F32 const& mat, RadianF32 const& angle);
	Mat4F32 MultiEulerYMatrix(Mat4F32 const& mat, RadianF32 const& angle);
	Mat4F32 MultiEulerZMatrix(Mat4F32 const& mat, RadianF32 const& angle);

	//
	Mat4F32 RotateionFromToMatrix(Vec3F32 const& from, Vec3F32 const& to);


	// Decompose matrix to scale vec3, rot quaternion and pos vec3
	// First version is scale along by standard orthogonal axis, such X, Y, Z
	void DecomposeMatrix(Mat4F32 const& mat, Vec3F32 const& scale, QuatF32 const& rot, Vec3F32 const& pos);

	// Second version is scale along by arbitrary axis
	// mat = transpose(scaleRot) * scale * scaleRot * rot * sign * translate
	void DecomposeStrechedMatrix(
		Mat4F32 const& mat,
		Vec3F32 const& scale,
		QuatF32 const& scaleRot,
		QuatF32 const& rot,
		Vec3F32 const& pos,
		FLOAT32& sign);

	void DecomposeStrechedMatrixNoScale(Mat4F32 const& mat,	QuatF32 const& rot,	Vec3F32 const& pos,	FLOAT32& sign);
}