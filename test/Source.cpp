#include "stl/Template/IdentityFunctor.h"
#include "stl/Template/UniquePtr.h"
#include "stl/Template/Less.h"
#include "stl/Template/IsTrivially.h"
#include "stl/Template/Invoke.h"
#include "stl/Template/AndOrNot.h"
#include "stl/Containers/Allocator.h"
#include "stl/Containers/Deque.h"
#include "stl/Containers/Vector.h"
#include <vector>
#include <type_traits> 
#include <iostream> 
#include <memory>
//#include "SGISTL/Allocator.h"

#include "common/fastmath/Vector3.h"

int compare(int a)
{
	return a;
}

struct FIdentityFunctor
{
	template <typename T>
	FORCEINLINE T&& operator()(T&& Val) const
	{
		return (T&&)Val;
	}
};

int main()
{
	Vector3 vvvvv;
	
	Vector2 xy = vvvvv.xy;
	Vector2 yz = vvvvv.yz;
	Vector2 zx = vvvvv.zx;


	system("pause");

	return 0;
}