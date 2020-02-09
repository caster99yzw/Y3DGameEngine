#pragma once
#include "property_wrapper.h"
#include "property_accessor.h"

namespace reflection
{

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template <typename PropType, typename SetterPolicy>
class PropertyWrapper;


template <typename ClassT, typename A, typename SetterPolicy>
class PropertyWrapper<A(ClassT::*), SetterPolicy> : public PropertyWrapperBase
{
	using PropType = A(ClassT::*);
	using Accessor = PropertyAccessor<PropType, SetterPolicy>;

public:
	PropertyWrapper(std::string const &inName, PropType inProp)
		: name(inName)
		, prop(inProp)
	{
		Initialize();
	}

	bool Vaild() const override { return true; }
	std::string const &Name() const override { return name; }
	Class DeclaringClass() const override { return Class::Get<ClassT>(); }
	
	bool SetValue(Variant &obj, Argument &arg) const override
	{
		return Accessor::SetValue(obj, prop, arg);
	}
	Variant Value(Variant &obj) const override
	{
		return Accessor::Value(obj, prop);
	}

private:
	std::string name;
	PropType prop;
};

}

