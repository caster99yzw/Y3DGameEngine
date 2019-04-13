#pragma once

#include "math/Y3DMatrix.h"

namespace Y3D
{
	enum CameraDescType
	{
		Y3D_CAMERA_ORTHOGONAL,
		Y3D_CAMERA_PERSPECTIVE
	};

	class CameraDescBase
	{
	public:

		Mat4F32			mGlobalPos;

		// View matrix is different from global position matrix
		Mat4F32			mViewTM;
		Mat4F32			mPreViewTM;

		// Projection matrix is defined in subclass
		Mat4F32			mProjectionTM;

		// VP matrix is view matrix multiplied by projection matrix
		Mat4F32			mVPTM;
		Mat4F32			mInvVPTM;

	protected:

		inline CameraDescBase();

		CameraDescType	type;

	public:

		inline void multisetToDefault();
		inline bool IsValid() const;
		inline CameraDescType GetType() const;

	};

	class CameraOrthoDesc : public CameraDescBase
	{
		struct LTNVec3
		{
			float32		xLeft;
			float32		yTop;
			float32		zNear;

		};

		struct RBFVec3
		{
			float32		xRight;
			float32		yBottom;
			float32		zNear;
		};

		LTNVec3			vLTNPos;
		RBFVec3			vRBFPos;

	public:

		inline CameraOrthoDesc();

		inline void multisetToDefault();
		inline bool IsValid() const;
	};

	class CameraPerspectiveDesc : public CameraDescBase
	{
	public:

		struct PersProjInfo
		{
			float32		fFOV;
			float32		fRatio;
			float32		zNear;
			float32		zFar;
		};

		PersProjInfo	pProjInfo;


	public:

		inline CameraPerspectiveDesc();

		inline void multisetToDefault();
		inline bool IsValid() const;
	};

	inline CameraDescBase::CameraDescBase()
	{
		// Nothing
	}

	inline void CameraDescBase::multisetToDefault()
	{
		//
		//
		//
	}

	inline bool CameraDescBase::IsValid() const
	{
		//
	}

	inline CameraDescType CameraDescBase::GetType() const
	{
		return type;
	}

	inline CameraOrthoDesc::CameraOrthoDesc()
		: vLTNPos{ 0.f, 0.f, 0.f }
		, vRBFPos{ 0.f, 0.f, 0.f }
	{
		type = Y3D_CAMERA_ORTHOGONAL;
	}

	inline void CameraOrthoDesc::multisetToDefault()
	{
		CameraDescBase::multisetToDefault();
		vLTNPos = { 0.f, 0.f, 0.f };
		vRBFPos = { 0.f, 0.f, 0.f };
	}

	inline bool CameraOrthoDesc::IsValid() const
	{
		//
	}

	inline CameraPerspectiveDesc::CameraPerspectiveDesc()
		: pProjInfo{ 0.f, 0.f, 0.f, 0.f }
	{
		type = Y3D_CAMERA_PERSPECTIVE;
	}

	inline void CameraPerspectiveDesc::multisetToDefault()
	{
		CameraDescBase::multisetToDefault();
		pProjInfo = { 0.f, 0.f, 0.f, 0.f };
	}

	inline bool CameraPerspectiveDesc::IsValid() const
	{
		//
	}
}