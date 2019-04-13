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
		float32				GetProjLeft();
		float32				GetProjTop();
		float32				GetProjRight();
		float32				GetProjBottom();
		float32				GetProjNear();
		float32				GetProjFar();

		/* Calculate view frustum's 8 corners' position

		Return number of corners

		aCorners: out array about corners position, you can pass NULL to get only corner numbers
			0 - 3: Near plane, left-bottom, left-top, right-top, right-bottom
			4 - 7: Far plane, left-bottom, left-top, right-top, right-bottom
		bInViewSpace: in view space or in world space
		fNear, fFar: near and far distance
		*/
		virtual uint32		CalcFrustumCorners(Vec3F32* aCorners, bool bInViewSpace, float32 fNear, float32 fFar);
	};

}

