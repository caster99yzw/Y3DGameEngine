//Test Mathematics

#include "Y3DVector.h"
#include <iostream>

using namespace Y3D;

int main()
{

	//V3F32 f31 = { 3.f, 4.f, 5.f };
	//V3F32 f32 = { 8.f, 8.f, 8.f };


	V3F32 f31 = { 3.f, 4.f, 5.f };
	V3F32 f32 = { 8.f, 8.f, 8.f };
	//f32 = f31;
	
	f31.Normalize();
	f32.Normalize();


	f32 = Cross(f31, f32);

	for (UINT32 i = 0; i < V3F32::Count; ++i)
	{
		std::cout << f32.v[i] << " ";
	}
	
	system("pause");

	return 0;
}