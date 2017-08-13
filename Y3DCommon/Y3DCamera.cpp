#include "Y3DCamera.h"

#include "Y3DMacro.h"

namespace Y3D
{
	inline Camera::Camera()
		: userData(NULL_X)
	{
		//
	}


	Camera::~Camera()
	{
		//
	}

	void Camera::Reset()
	{
		//m_camPostProjectTM.
		//m_camViewTM
		//m_preMatViewTM
		//m_preMatProjectTM
		//m_preMatVPTM
	}

	void Camera::PushViewTM()
	{

	}

	void Camera::PopViewTM()
	{

	}

	void Camera::UpdateViewTM()
	{
		Vec3F32 vec;
		vec = vec.Normalized();
	}

	
	BOOL Camera::Transform(Vec3F32 const& vecIn, Vec3F32 const& vecOut)
	{
		//vecOut = m_camVPTM.Trans(vecIn);

		if (-1.f > vecOut.x || vecOut.x > 1.f ||
			-1.f > vecOut.y || vecOut.z > 1.f ||
			-1.f > vecOut.z || vecOut.z > 1.f)
			return false;
		
		return true;
	}

	BOOL Camera::InvTransform(Vec3F32 const& vecIn, Vec3F32 const& vecOut)
	{
		if (-1.f > vecIn.x || vecIn.x > 1.f ||
			-1.f > vecIn.y || vecIn.z > 1.f ||
			-1.f > vecIn.z || vecIn.z > 1.f)
			return false;

		//vecOut = m_camInvVPTM.Trans(vecIn);
		return true;
	}



}
