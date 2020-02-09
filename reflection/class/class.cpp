#include "class.h"
#include "reflection/variant.h"
#include "reflection/parameter/parameter_compare.h"

namespace reflection
{

Class::Class(ClassData const *inData)
	: data(inData)
{
}

Class Class::Get(char const *className)
{
	return RegistrationManager::Instance().FindClassByCunstomName(className);
}

Variant Class::Create(std::vector<Argument> args /*= std::vector<Argument>()*/) const
{
	auto &constructors = data->getClassData().constructors;
	for (auto const &ctor : constructors)
	{
		if (ctor.Vaild() && ParameterComparisonHelper::Compare(ctor.Parameters(), args))
			// wrong: ctor.Invoke(args)!!!!
			return ctor.Invoke(std::move(args));
	}
	return Variant();
}

bool Class::Destory(Variant &obj) const
{
	auto &destructor = data->getClassData().destructor;
	if (destructor.Valid())
	{
		destructor.Invoke(obj);
		return true;
	}
	return false;
}

Variant Class::Invoke(char const *name, Variant &obj, std::vector<Argument> args /*= std::vector<Argument>()*/) const
{
	auto &methods = data->getClassData().methods;
	for (auto const &mth : methods)
	{
		if (mth.Valid() && mth.Name() == name && ParameterComparisonHelper::Compare(mth.Parameters(), args))
			// wrong: ctor.Invoke(args)!!!!
			return mth.Invoke(obj, std::move(args));
	}
	return Variant();
}

Method Class::GetMethod(char const *name)
{
	auto &methods = data->getClassData().methods;
	for (auto const &mth : methods)
	{
		if (mth.Name() == name)
			return mth;
	}
	return Method();
}

Property Class::GetProperty(char const *name)
{
	auto &properties = data->getClassData().properties;
	for (auto const &prop : properties)
	{
		if (prop.Name() == name)
			return prop;
	}
	return Property();
}

}