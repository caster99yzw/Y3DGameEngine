#pragma once
#include "reflection/core.h"

namespace reflection
{

namespace detail
{
struct SetValue {};
struct ReadOnly {};
}

class Variant;
class Argument;

template <typename PropType, typename SetterPolicy>
struct PropertyAccessor;


template <typename ClassT, typename A>
struct PropertyAccessor<A(ClassT::*), detail::SetValue>
{
	template <typename PropType>
	static bool SetValue(Variant &obj, PropType prop, Argument &arg)
	{
		ClassT *cls = obj.ValuePtr<ClassT>();
		cls->*prop = arg.Value<A>();
		return true;
	}

	template <typename PropType>
	static Variant Value(Variant &obj, PropType prop)
	{
		ClassT *cls = obj.ValuePtr<ClassT>();
		return Variant(cls->*prop);
	}

};

template <typename ClassT, typename A>
struct PropertyAccessor<A(ClassT::*), detail::ReadOnly>
{
	template <typename PropType>
	static bool SetValue(Variant &obj, PropType prop, Argument &arg)
	{
		return false;
	}

	template <typename PropType>
	static Variant Value(Variant &obj, PropType prop)
	{
		ClassT *cls = obj.ValuePtr<ClassT>();
		return Variant(cls->*prop);
	}
};




}

