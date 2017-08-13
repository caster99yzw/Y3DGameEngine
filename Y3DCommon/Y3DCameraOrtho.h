#pragma once

#include "Y3DCamera.h"

namespace Y3D
{	
	//////////////////////////////////////////////////////////////////////////
	//
	//class CameraOrtho : projection matrix update
	//
	//////////////////////////////////////////////////////////////////////////

	class CameraOrtho : public Camera
	{
	public: // Static and Macro

	public: //	Constructions and Destructions

		inline				CameraOrtho();
							~CameraOrtho();

	public: // Attributes

		//userData

	private: // Operations

		virtual void		UpdateProjectionTM();

		virtual void		CreateViewFrustum();
		virtual void		UpdateWorldFrustum();

	public: // Operations

		virtual void		SaveToDesc(CameraDescBase & desc) override;

		virtual void		LoadFromDesc(CameraDescBase const& desc) override;

		virtual void		Release() override;
		
		// Manipulation about orthogonal camera
		FLOAT32				GetProjLeft();
		FLOAT32				GetProjTop();
		FLOAT32				GetProjRight();
		FLOAT32				GetProjBottom();
		FLOAT32				GetProjNear();
		FLOAT32				GetProjFar();

		/* Calculate view frustum's 8 corners' position

		Return number of corners

		aCorners: out array about corners position, you can pass NULL to get only corner numbers
			0 - 3: Near plane, left-bottom, left-top, right-top, right-bottom
			4 - 7: Far plane, left-bottom, left-top, right-top, right-bottom
		bInViewSpace: in view space or in world space
		fNear, fFar: near and far distance
		*/
		virtual UINT32		CalcFrustumCorners(Vec3F32* aCorners, BOOL bInViewSpace, FLOAT32 fNear, FLOAT32 fFar);
	};

}

