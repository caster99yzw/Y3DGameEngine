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

struct IJavaIterator
{
	template <typename Base>
	struct Interface : Base
	{
		bool Done() const { return common::poly_call<0>(*this); }
		uint32_t Current() const { return common::poly_call<1>(*this); }
		void Next() { common::poly_call<2>(*this); }
	};

	template <typename T>
	using Members = common::PolyMembers<&T::Done, &T::Current, &T::Next>;
};

using JavaIterator = common::Poly<IJavaIterator>;


struct AAA
{
	bool Done() const {}
	uint32_t Current() const {}
	void Next() {}
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
}

int main()
{
	test_overload();
	test_type_list();
	system("pause");
	return 0;
}