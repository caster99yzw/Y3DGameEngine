#include "Y3DCameraOrtho.h"
#include "Y3DMacro.h"

namespace Y3D 
{
	inline CameraOrtho::CameraOrtho()
	{
		//
	}


	CameraOrtho::~CameraOrtho()
	{
		Release();
	}

	void CameraOrtho::SaveToDesc(CameraDescBase & desc)
	{

	}

	void CameraOrtho::LoadFromDesc(CameraDescBase const& desc)
	{

	}

	void CameraOrtho::Release()
	{
		// There don't use Y3DSafeDelete(static_cast<CameraOrthoDesc*>(userData))
		// because static_cast<CameraOrthoDesc*>(userData) = NULL_X is error
		if (userData)
		{
			delete static_cast<CameraOrthoDesc*>(userData);
			userData = NULL_X;
		}
	}

	void CameraOrtho::UpdateProjectionTM()
	{
		//Get Orthodox matrix
		//m_camProjectTM = GetMatrix();
		//Inverse 

		//m_camVPTM = m_camViewTM * m_camProjectTM;
		//Inverse 

		CreateViewFrustum();
	}

	void CameraOrtho::CreateViewFrustum()
	{
		//PLANE
	}

	void CameraOrtho::UpdateWorldFrustum()
	{
		//PLANE
	}

	/* Calculate view frustum's 8 corners' position

	Return number of corners

	aCorners: out array about corners position, you can pass NULL to get only corner numbers
		0 - 3: Near plane, left-bottom, left-top, right-top, right-bottom
		4 - 7: Far plane, left-bottom, left-top, right-top, right-bottom
	bInViewSpace: in view space or in world space
	fNear, fFar: near and far distance
	*/
	UINT32 CameraOrtho::CalcFrustumCorners(Vec3F32* aCorners, BOOL bInViewSpace, FLOAT32 fNear, FLOAT32 fFar)
	{
		if (!aCorners)
			return 8;

		Vec3F32 vPos, vZ, vX, vY;

		if (bInViewSpace)
		{
			vPos = Vec3F32::IDENTITY;
			vX = g_vAxisX;
			vY = g_vAxisY;
			vZ = g_vAxisZ;
		}
		else
		{
			// = GetPos();
			//
			//
			//
		}

		//float fPlaneHeight = (m_fRight - m_fLeft) * 0.5f;
		//float fPlaneWidth = (m_fTop - m_fBottom) * 0.5f;

		//
		//
		//
		//

		return 8;
	}
}