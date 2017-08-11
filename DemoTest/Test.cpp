//Test Mathematics

#include "Y3DQuaternion.h"
#include <iostream>

using namespace Y3D;

int main()
{

	//V3F32 f31 = { 3.f, 4.f, 5.f };
	//V3F32 f32 = { 8.f, 8.f, 8.f };
	
	
	//f32 = f31;
	
	//f31.Normalize();
	//f32.Normalize();


	V1F32 i(1);

	i.MagnitudeSquared();

	std::cout << i.x;

	for (UINT32 i = 0; i < V1F32::Count; ++i)
	{
		//std::cout << f33.v[i] << " ";
	}
	
	system("pause");

	return 0;
}