
#include "Y3DCamera.h"

namespace Y3D
{
	class CameraEntity
	{
	public:

					CameraEntity();
					~CameraEntity();

	private:

		Camera*		m_pCamera;

	public:

		bool		CreateCamera(CameraDescBase const& desc);
	};
}