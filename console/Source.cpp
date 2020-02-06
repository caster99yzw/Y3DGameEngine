#include "stl/Template/IdentityFunctor.h"
#include "stl/Template/UniquePtr.h"
#include "stl/Template/Less.h"
#include "stl/Template/IsTrivially.h"
#include "stl/Template/Invoke.h"
#include "stl/Template/AndOrNot.h"
#include "stl/Containers/Allocator.h"
#include "stl/Containers/Deque.h"
#include "stl/Containers/Array.h"
#include <vector>
#include <type_traits> 
#include <iostream> 
#include <memory>
//#include "SGISTL/Allocator.h"

#include "common/fastmath/Vector3.h"
#include "common/DurationForProfile.h"
#include "common/BitContainer.h"

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

void TestBitAndDuration()
{
	y3dcommon::DurationForProfile duration;
	uint32 x = 0;
	for (int i = 0; i < 100000; ++i)
	{
		x++;
		x = x * 3 + x / 2;
	}
	duration.Stop();
	std::cout << duration.Duration_Nanoseconds() << std::endl;
	std::cout << duration.Duration_Microseconds() << std::endl;
	std::cout << duration.Duration_Milliseconds() << std::endl;
	std::cout << duration.Duration_Seconds() << std::endl;

	y3dcommon::BitContainer bits = y3dcommon::MakeBitContainer<48>();
	y3dcommon::BitContainerIndex index0(24);
	bool ret = true;

	bits.Reset();
	ret = bits.Test(index0);
	bits.Set(index0);
	ret = bits.Test(index0);
	bits.Filp(index0);
	ret = bits.Test(index0);
	bits.Set();
	ret = bits.Test(index0);
	bits.Filp();
	ret = bits.Test(index0);

	bits.Set(y3dcommon::BitContainerIndex(10));
	bits.Set(y3dcommon::BitContainerIndex(20));
	bits.Set(y3dcommon::BitContainerIndex(30));
	bits.Set(y3dcommon::BitContainerIndex(40));
	std::cout << bits.SetCount() << std::endl;
	std::cout << bits.ResetCount() << std::endl;
}


int main()
{
	Vector3 vvvvv;
	
	Vector2 xy = vvvvv.xy;
	Vector2 yz = vvvvv.yz;
	Vector2 zx = vvvvv.zx;

	struct A
	{
		float a;
		int   b;
	};

	Array<int> temp1(10, 1);
	Array<float> temp2(10, 2.f);
	Array<A> temp3(10, A{3.f, 4});
	Array<char> temp4(10, 'g');
	Array<A *> temp5(10, nullptr);

	TestBitAndDuration();

	system("pause");

	return 0;
}