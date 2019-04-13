#include "Y3DCameraPerspective.h"


namespace Y3D
{
	inline CameraPerspective::CameraPerspective()
	{
		//
	}


	CameraPerspective::~CameraPerspective()
	{
		//
	}


	/* Calculate view frustum's 8 corners' position

	Return number of corners

	aCorners: out array about corners position, you can pass NULL to get only corner numbers
		0 - 3: Near plane, left-bottom, left-top, right-top, right-bottom
		4 - 7: Far plane, left-bottom, left-top, right-top, right-bottom
	bInViewSpace: in view space or in world space
	fNear, fFar: near and far distance
	*/
	uint32 CameraPerspective::CalcFrustumCorners(Vec3F32* aCorners, bool bInViewSpace, float32 fNear, float32 fFar)
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
		//float fPlaneHeight = (m_fRight - m_fLeft) * 0.5f;
		//float fPlaneHeight = (m_fRight - m_fLeft) * 0.5f;
		//float fPlaneWidth = (m_fTop - m_fBottom) * 0.5f;

		//
		//
		//
		//

		return 8;
	}
}
