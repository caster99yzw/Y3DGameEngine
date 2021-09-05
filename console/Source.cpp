#include <iostream>
#include "reflection/registration/registration.h"
#include "common/core/unique_ptr.h"
#include "common/core/shared_ptr.h"
#include "common/core/type_traits.h"
#include "common/core/overload.h"

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
	using result0 = common::MetaQuote::Apply<int, double>;
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

int main()
{
	test_overload();
	test_type_list();
	system("pause");
	return 0;
}