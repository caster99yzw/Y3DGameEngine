//Test Mathematics

#include "Y3DQuaternion.h"
#include <iostream>

#include <filesystem>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "List.h"

using namespace Y3D;
using namespace rapidjson;

int main()
{
	Vec3F32 f31 = { 3.f, 4.f, 5.f };
	Vec3F32 f32 = { 8.f, 8.f, 8.f };
	
	f31 = Lerp(f31, f32, 0.25f);

	//f31 = f32 + f31 * 2.f;
	//f31 = 2.f * f31;
	
	//f32 = f31;
	
	//f31.Normalize();
	//f32.Normalize();

	for (UINT32 i = 0; i < Vec3F32::Count; ++i)
	{
		std::cout << f31.v[i] << " ";
	}

	// 1. Parse a JSON string into DOM.
	const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
	Document d;
	d.Parse(json);

	// 2. Modify it by DOM.
	Value& s = d["stars"];
	s.SetInt(s.GetInt() + 1);

	// 3. Stringify the DOM
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	d.Accept(writer);

	// Output {"project":"rapidjson","stars":11}
	std::cout << buffer.GetString() << std::endl;
	
	system("pause");

	return 0;
}