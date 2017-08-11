#pragma once

//////////////////////////////////////////////////////////////////////////
//
//Provide a .h to include all global macros
//
//////////////////////////////////////////////////////////////////////////

// Release Y3D objects
#define Y3DRELEASE(p) \
{ \
	if (p) \
	{ \
		(p)->Release(); \
		delete (p); \
		p = nullptr; \
	} \
}

// Release Y3D interfaces
#define Y3DRELEASEINTERFACE(p) \
{ \
	if (p) \
	{ \
		(p)->Release(); \
		p = nullptr; \
	} \
}

// Safe delete object
#define Y3DSAFEDELETE(p) \
{ \
	if (p) \
	{ \
		delete (p); \
		p = nullptr; \
	} \
}

// Safe delete object array
#define Y3DSAFEDELETEARRAY(p) \
{ \
	if (p) \
	{ \
		delete [](p); \
		p = nullptr; \
	} \
}


//namespace Y3D
//{
//	template<class T>
//	class Quaternion
//	{
//	public: // Static and Macro
//
//
//	public: //	Constructions and Destructions
//
//
//	public: // Attributes
//
//
//	public: // Operations
//
//	};
//}