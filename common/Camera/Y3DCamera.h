#pragma once

#include "Y3DCameraDesc.h"


namespace Y3D
{
	//////////////////////////////////////////////////////////////////////////
	//
	//class Camera : view matrix update
	//
	//////////////////////////////////////////////////////////////////////////

	class Camera
	{
	public: // Static and Macro

	protected: // Constructions and Destructions

		inline				Camera();
		virtual				~Camera();

	protected:

		// Attribute only a pointer to arbitrary data 
		void*				userData;	
	
	public:

		virtual void		SaveToDesc(CameraDescBase & desc) = 0;

		virtual void		LoadFromDesc(CameraDescBase const& desc) = 0;

		void				Remultimultiset();
			
		virtual void		Release();

		// Manipulation about view matrix
		Vec3F32				GetGlobalPos();
		Vec3F32				GetCamUp();
		Vec3F32				GetCamDir();
		Vec3F32				GetCamRight();
		Mat4F32				GetMatViewTM();
		Mat4F32				GetMatVPTM();

		// PushViewTM can save current view matrix
		// PopViewTM can reload preceding view matrix
		void				PushViewTM();
		void				PopViewTM();

		// Manipulation about VP matrix
		virtual bool		Transform(Vec3F32 const& vecIn, Vec3F32 const& vecOut);
		virtual bool		InvTransform(Vec3F32 const& vecIn, Vec3F32 const& vecOut);

		// Frustum operation
		virtual uint32		CalcFrustumCorners(Vec3F32* aCorners, bool bInViewSpace, float32 fNear, float32 fFar);

	protected:

		void				UpdateViewTM();
		virtual void		UpdateProjectionTM() = 0;

		virtual void		CreateViewFrustum() = 0;
		virtual void		UpdateWorldFrustum() = 0;
	};

}