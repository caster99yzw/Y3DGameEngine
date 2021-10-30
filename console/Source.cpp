#include <iostream>
#include "reflection/registration/registration.h"
#include "common/core/type_traits.h"
#include "common/core/overload.h"
#include "common/core/poly/poly.h"

struct overload
{
	int prop(int, int) const { return 1; }
	int prop(int, int) { return 2; }
	int prop(int) const { return 3; }
	int prop(int) { return 4; }
	int prop() const { return 5; }
	int prop() { return 6; }
};

int prop(int, int) { return 10; }
int prop(int) { return 11; }
int prop() { return 12; }

void test_overload()
{
	common::Signature<int(int, int) const>(&overload::prop);
	common::Signature<int(int, int)>(&overload::prop);
	common::Signature<int(int) const>(&overload::prop);
	common::Signature<int(int)>(&overload::prop);
	common::Signature<int() const>(&overload::prop);
	common::Signature<int()>(&overload::prop);

	std::cout << common::Signature<int(int, int)>(&prop)(1,1) << std::endl;
	std::cout << common::Signature<int(int)>(&prop)(1) << std::endl;
	std::cout << common::Signature<int()>(&prop)() << std::endl;
}

template <std::size_t N>
struct MetaElementEx
{
	template <typename List>
	using Apply = common::MetaApply<List, common::MetaElement<N>>;
};

void test_type_list()
{
	using result0 = common::MetaQuote<common::TypeList>::Apply<int, double>;
	using list0 = common::TypeList<int, short, long>;
	using list1 = common::TypeList<float, double>;
	using list2 = common::TypePushBack<list0, float, double>;
	using list3 = common::TypePushFront<list0, float, double>;
	using item0 = common::TypeAt<list0, 0>;
	using item1 = common::TypeAt<list1, 1>;
	using item2 = common::TypeAt<list2, 2>;
	using item3 = common::TypeAt<list3, 3>;

	using list4 = common::TypeList<list0, list1, list2, list3>;
	using list5 = common::TypeTransform<list4, MetaElementEx<0>>;
	using list6 = common::TypeJoin<common::TypeList<list0, list1>>;
	using list7 = common::TypeConcat<list0, list1>;
}

template <class Self>
bool negate(const Self& t)
{
	return static_cast<bool>(!t);
}

struct IJavaIterator
{
	template <typename Base>
	struct Interface : Base
	{
		bool Done() const { return common::PolyCall<0>(*this); }
		uint32_t Current() const { return common::PolyCall<1>(*this); }
		void Next() { common::PolyCall<2>(*this); }
		bool operator!() const { return common::PolyCall<3>(*this); }
	};

	template <typename T>
	using Members = common::PolyMembers<
		&T::Done,
		&T::Current,
		&T::Next,
		&negate<T>>;
};

struct DerivedJavaIterator : common::PolyExtends<IJavaIterator>
{
	template <typename Base>
	struct Interface : Base
	{
		void Derived() const { common::PolyCall<0>(*this); }
	};
	
	template <typename T>
	using Members = common::PolyMembers<&T::Derived>;
};

using JavaIterator = common::Poly<IJavaIterator>;
using JavaIteratorRef = common::Poly<IJavaIterator&>;
using JavaIteratorCRef = common::Poly<const IJavaIterator&>;
using Iterator = common::Poly<DerivedJavaIterator>;

struct AAA
{
	AAA()
		: i(std::make_shared<uint32_t>(100)) {}

	bool operator!() const { return true; }
	bool Done() const
	{
		return false;
	}
	uint32_t Current() const
	{
		return *i;
	}
	void Next() { std::cout << "ni ma!"; }

private:
	std::shared_ptr<uint32_t> i;
};

struct BBB : AAA
{
	void Derived() const
	{
		std::cout << "ni ba!";
	}
};

void test_poly()
{
	using T0 = common::MetaQuote<std::add_const>::Apply<int>;

	using AddConst = common::MetaQuote<std::add_const_t>::Apply<int>;
	using AddLRef = common::MetaQuote<std::add_lvalue_reference>::Apply<int>;
	using AddRRef = common::MetaQuote<std::add_lvalue_reference>::Apply<int>;


	using ret = common::AddCvrefOf<int, const int&&>;
	constexpr bool b0 = std::is_constructible_v<AAA, AAA>;
	constexpr bool b1 = std::is_constructible_v<AAA, AAA&>;
	constexpr bool b2 = std::is_constructible_v<AAA, AAA&&>;
	constexpr bool b3 = std::is_constructible_v<AAA, const AAA&>;
	constexpr bool b4 = common::impl::InterfaceMatching<AAA, IJavaIterator>::value;

	AAA local;

	JavaIterator iter = local;
	iter.Next();
	assert(common::PolyCast<AAA>(iter).Done() == iter.Done());
	assert(common::PolyCast<AAA>(iter).Current() == iter.Current());
	assert(!common::PolyCast<AAA>(iter) == !iter);

	JavaIteratorRef iter_ref = local;
	iter_ref->Next();
	assert(common::PolyCast<AAA>(iter_ref).Done() == iter_ref->Done());
	assert(common::PolyCast<AAA>(iter_ref).Current() == iter_ref->Current());
	assert(!common::PolyCast<AAA>(iter_ref) == !(*iter_ref));

	JavaIteratorCRef iter_cref = local;
	//iter_cref->Next();
	assert(common::PolyCast<AAA>(iter_cref).Done() == iter_cref->Done());
	assert(common::PolyCast<AAA>(iter_cref).Current() == iter_cref->Current());
	assert(!common::PolyCast<AAA>(iter_cref) == !(*iter_cref));

	BBB local_d;

	Iterator d_iter = local_d;
	assert(common::PolyCast<BBB>(d_iter).Done() == d_iter.Done());
	assert(common::PolyCast<BBB>(d_iter).Current() == d_iter.Current());
	assert(!common::PolyCast<BBB>(d_iter) == !d_iter);
	d_iter.Derived();
}

int main()
{
	test_overload();
	test_type_list();
	test_poly();
	system("pause");
	return 0;
}