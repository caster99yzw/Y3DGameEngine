#pragma once
#include "registration_manager.h"
#include "reflection/constructor/constructor_wrapper_detail.h"
#include "reflection/destructor/destructor_wrapper_detail.h"
#include "reflection/method/method_wrapper_detail.h"
#include "reflection/property/property_wrapper_detail.h"
#include "reflection/parameter/parameter_infos.h"

namespace reflection
{

template <typename ClassType, typename... ParamTs>
class Bind<detail::BindConstructor, ClassType, ParamTs...> : public Registation::CustomClasss<ClassType>
{
	using Base = Registation::CustomClasss<ClassType>;
	using DefaultReturnPolicy = detail::ReturnAsObject;

public:
	Bind(std::shared_ptr<RegistrationExecutor> &inExecutor)
		: Base(inExecutor)
	{
	}

	~Bind()
	{
		constructor = MakeDefaultConstructor();

		auto wrapper = common::MakeRref(std::move(constructor));
		auto callable = [wrapper]()
		{
			StoreItem(std::move(wrapper.value));
			StoreItem(std::make_unique<DestructorWrapper<ClassType>>());
		};
		// Since std::function has to be copyable, everything you put into it must also be copyable. 
		// And a move-only lambda does not meet that requirement.
		this->executor->AddRegistrationList(this, std::move(callable));
	}

private:
	std::unique_ptr<ConstructorWrapperBase> MakeDefaultConstructor()
	{
		using ParameterInfos_t = MakeParameterInfos<common::TypeList<ParamTs...>, detail::HasNoParamName>;
		return std::make_unique<ConstructorWrapper<ClassType, DefaultReturnPolicy,
			ParameterInfos_t,
			ParamTs...>>(ParameterInfos_t());
	}

	std::unique_ptr<ConstructorWrapperBase> constructor;
};

template <typename FuncType, typename ClassType>
class Bind<detail::BindMethod, FuncType, ClassType> : public Registation::CustomClasss<ClassType>
{
	using Base = Registation::CustomClasss<ClassType>;

public:
	Bind(std::shared_ptr<RegistrationExecutor> &inExecutor, std::string const &inName, FuncType inProp)
		: Base(inExecutor)
		, name(inName)
		, func(inProp)
	{
	}

	~Bind()
	{
		auto method = MakeDefaultMethod();

		auto wrapper = common::MakeRref(std::move(method));
		auto callable = [wrapper]()
		{
			StoreItem(std::move(wrapper.value));
		};
		this->executor->AddRegistrationList(this, std::move(callable));
	}
	
private:
	std::unique_ptr<MethodWrapperBase> MakeDefaultMethod()
	{
		using ParameterInfos_t = MakeParameterInfos<FuncType, detail::HasNoParamName>;
		return std::make_unique<MethodWrapper<FuncType, ParameterInfos_t>>(name, func, ParameterInfos_t());
	}

	std::string name;
	FuncType func;
};

template <typename PropType, typename ClassType>
class Bind<detail::BindProperty, PropType, ClassType> : public Registation::CustomClasss<ClassType>
{
	using Base = Registation::CustomClasss<ClassType>;
	using SetterPolicy = detail::SetValue;

public:
	Bind(std::shared_ptr<RegistrationExecutor> &inExecutor, std::string const &inName, PropType inProp)
		: Base(inExecutor)
		, name(inName)
		, prop(inProp)
	{
	}

	~Bind()
	{
		auto property = MakeDefaultProperty();

		auto wrapper = common::MakeRref(std::move(property));
		auto callable = [wrapper]()
		{
			StoreItem(std::move(wrapper.value));
		};
		this->executor->AddRegistrationList(this, std::move(callable));
	}

private:
	std::unique_ptr<PropertyWrapperBase> MakeDefaultProperty()
	{
		return std::make_unique<PropertyWrapper<PropType, SetterPolicy>>(name, prop);
	}

	std::string name;
	PropType prop;
};

}

