#pragma once
#include <stdint.h>
#include <initializer_list>

typedef unsigned long	SIZE_T;

#define INDEX_NONE		-1

#define CONSTEXPR		constexpr
#define FORCEINLINE		__forceinline

using	UINT8			= unsigned char;
using	UINT16			= unsigned short int;
using	UINT32			= unsigned int;
using	UINT64			= unsigned long long;
using	INT8			= signed char;
using	INT16			= signed short int;
using	INT32			= signed int;
using	INT64			= signed long long;

using	FLOAT32			= float;
using	FLOAT64			= double;

using	ANSICHAR		= char;			// -                  8-bit fixed-width representation of 7-bit characters.
using	WIDECHAR		= wchar_t;		// - In-memory only.  ?-bit fixed-width representation of the platform's natural wide character set.  Could be different sizes on different platforms.

using	TYPE_OF_NULLPTR = decltype(nullptr);
										
//typedef uint8				CHAR8;		// An 8-bit character type - In-memory only.  8-bit representation.  Should really be char8_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
//typedef uint16				CHAR16;		// A 16-bit character type - In-memory only.  16-bit representation.  Should really be char16_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
//typedef uint32				CHAR32;		// A 32-bit character type - In-memory only.  32-bit representation.  Should really be char32_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
//typedef WIDECHAR			TCHAR;		// A switchable character  - In-memory only.  Either ANSICHAR or WIDECHAR, depending on a licensee's requirements.
//
//typedef SelectIntPointerType<uint32, uint64, sizeof(void*)>::TIntPointer UPTRINT;	// unsigned int the same size as a pointer
//typedef SelectIntPointerType<int32, int64, sizeof(void*)>::TIntPointer PTRINT;		// signed int the same size as a pointer
//typedef UPTRINT SIZE_T;																// unsigned int the same size as a pointer
//typedef PTRINT SSIZE_T;																// signed int the same size as a pointer
//
//typedef int32					TYPE_OF_NULL;
//typedef decltype(nullptr)		TYPE_OF_NULLPTR;