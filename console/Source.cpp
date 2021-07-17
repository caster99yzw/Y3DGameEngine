#include "stl/Template/IdentityFunctor.h"
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

#include "reflection/registration/registration.h"
#include "common/unique_ptr.h"
#include "common/shared_ptr.h"

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

template <std::size_t... Ns>
void func(common::IndexSequence<Ns...>)
{
	std::cout << "Hellp" << std::endl;
}

struct TestForClass
{
	int a = 10;
	const int aa = 10000;
	float b = 3.f;
	bool c = true;

	TestForClass() = default;


	TestForClass(int ina, float inb, bool inc)
		: a(ina), b(inb), c(inc) {}

	TestForClass(int ina, float inb)
		: a(ina), b(inb) {}

	bool Check() const
	{
		return (a != 0 && b != 0.f && c == false);
	}
	void Inc()
	{
		++a;
		b += 1.f;
		c = true;
	}

	void HaHa(int in, int inn) const
	{
		std::cout << a << " "
			<< b << " "
			<< c << " "
			<< in << " "
			<< inn << std::endl;
	}
	void HaHa() const
	{
		std::cout << a << " "
			<< b << " "
			<< c << std::endl;
	}

};


struct Base
{
	~Base() = default;
	common::unique_ptr<int> b;
	common::shared_ptr<int> c;
};

struct Derived : public Base
{
	common::unique_ptr<int> d;
	common::shared_ptr<int> e;
};

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

	func(common::MakeIndexSequence<10>());

	reflection::Registation::CustomClasss<TestForClass>("myclass")
		.Constructor()
		.Constructor<int, float, bool>()
		.Constructor<int, float>()
		.Method("mymethod", &TestForClass::Check)
		.Method("mymethod1", &TestForClass::Inc)
		.Method("mymethod2", static_cast<void(TestForClass::*)(int,int) const>(&TestForClass::HaHa))
		.Method("mymethod2", static_cast<void(TestForClass::*)() const>(&TestForClass::HaHa))
		.Property("myproperty", &TestForClass::a);

	reflection::Class cls = reflection::Class::Get("myclass");
	reflection::Variant obj = cls.Create({ reflection::Argument(500), reflection::Argument(5.f) });
	cls.Invoke("mymethod2", obj);
	cls.Invoke("mymethod1", obj);
	cls.Invoke("mymethod2", obj, { reflection::Argument(333), reflection::Argument(666) });

	reflection::Property p = cls.GetProperty("myproperty");
	p.SetValue(obj, reflection::Argument(100));
	cls.Invoke("mymethod2", obj);
	reflection::Variant b = p.Value(obj);
	int value = b.Value<int>();
	assert(value == 100);

	common::unique_ptr<Base> base;
	common::unique_ptr<Derived> derived;
	system("pause");
	return 0;
}