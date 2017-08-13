
#include "CameraEntity.h"
#include "Y3DMacro.h"
#include "Y3DCameraPerspective.h"
#include "Y3DCameraOrtho.h"

namespace Y3D
{
	CameraEntity::CameraEntity()
		: m_pCamera(NULL_X)
	{

	}

	CameraEntity::~CameraEntity()
	{

	}

	BOOL CameraEntity::CreateCamera(CameraDescBase const& desc)
	{
		CameraDescType type = desc.GetType();

		if (type == Y3D_CAMERA_ORTHOGONAL)
			m_pCamera = new CameraOrtho;
		else if (type == Y3D_CAMERA_PERSPECTIVE)
			m_pCamera = new CameraPerspective;

		if (!m_pCamera)
			return false;

		m_pCamera->LoadFromDesc(desc);
		return true;
	}
}