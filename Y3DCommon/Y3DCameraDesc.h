#pragma once

#include "Y3DMatrix.h"

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

		inline void SetToDefault();
		inline BOOL IsValid() const;
		inline CameraDescType GetType() const;

	};

	class CameraOrthoDesc : public CameraDescBase
	{
		struct LTNVec3
		{
			FLOAT32		xLeft;
			FLOAT32		yTop;
			FLOAT32		zNear;

		};

		struct RBFVec3
		{
			FLOAT32		xRight;
			FLOAT32		yBottom;
			FLOAT32		zNear;
		};

		LTNVec3			vLTNPos;
		RBFVec3			vRBFPos;

	public:

		inline CameraOrthoDesc();

		inline void SetToDefault();
		inline BOOL IsValid() const;
	};

	class CameraPerspectiveDesc : public CameraDescBase
	{
	public:

		struct PersProjInfo
		{
			FLOAT32		fFOV;
			FLOAT32		fRatio;
			FLOAT32		zNear;
			FLOAT32		zFar;
		};

		PersProjInfo	pProjInfo;


	public:

		inline CameraPerspectiveDesc();

		inline void SetToDefault();
		inline BOOL IsValid() const;
	};

	inline CameraDescBase::CameraDescBase()
	{
		// Nothing
	}

	inline void CameraDescBase::SetToDefault()
	{
		//
		//
		//
	}

	inline BOOL CameraDescBase::IsValid() const
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

	inline void CameraOrthoDesc::SetToDefault()
	{
		CameraDescBase::SetToDefault();
		vLTNPos = { 0.f, 0.f, 0.f };
		vRBFPos = { 0.f, 0.f, 0.f };
	}

	inline BOOL CameraOrthoDesc::IsValid() const
	{
		//
	}

	inline CameraPerspectiveDesc::CameraPerspectiveDesc()
		: pProjInfo{ 0.f, 0.f, 0.f, 0.f }
	{
		type = Y3D_CAMERA_PERSPECTIVE;
	}

	inline void CameraPerspectiveDesc::SetToDefault()
	{
		CameraDescBase::SetToDefault();
		pProjInfo = { 0.f, 0.f, 0.f, 0.f };
	}

	inline BOOL CameraPerspectiveDesc::IsValid() const
	{
		//
	}
}