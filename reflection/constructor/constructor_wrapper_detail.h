#pragma once
#include "constructor_wrapper.h"
#include "reflection/argument/argument.h"
#include "reflection/parameter/parameter.h"
#include "reflection/variant.h"

namespace reflection
{

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

namespace detail
{
struct ReturnAsObject {};
struct ReturnAsRawPoint {};
struct ReturnAsSharedPoint {};
}

template <typename ClassType, typename ReturnPolicy, typename... ParamTs>
struct ConstructorInvoker;


template <typename ClassType, typename... ParamTs>
struct ConstructorInvoker<ClassType, detail::ReturnAsObject, ParamTs...>
{
	using ReturnType = ClassType;

	template <typename... ArgTs>
	static Variant Invoke(ArgTs &&... arguments)
	{
		return Variant(ClassType(arguments. template Value<ParamTs>()...));
	};
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template <typename ClassType, typename ReturnPolicy, typename ParamInfosType, typename... ParamTs>
class ConstructorWrapper;


template <typename ClassType, typename ReturnPolicy, typename... ParamWrapperTs, typename... ParamTs>
class ConstructorWrapper<ClassType, ReturnPolicy, ParameterInfos<ParamWrapperTs...>, ParamTs...> : public ConstructorWrapperBase
{
	static_assert(common::IsSameListSize<common::type_list<ParamWrapperTs...>, common::type_list<ParamTs...>>::value,
		"the number of wrappers must be same as the number of parameter types");

	using Invoker = ConstructorInvoker<ClassType, ReturnPolicy, ParamTs...>;
	using InstanciatedType = typename Invoker::ReturnType;

public:
	ConstructorWrapper(ParameterInfos<ParamWrapperTs...> inParameterInfos)
		: parameterInfos(inParameterInfos)
		, parameters(CreateParameterArray(inParameterInfos))
	{
		Initialize();
	}

	bool Vaild() const override { return true; }
	Class DeclaringClass() const override { return Class::Get<InstanciatedType>(); }
	common::array_view<Parameter> Parameters() const
	{
		return common::array_view<Parameter>(parameters.data(), parameters.size());
	}

	Variant Invoke() const override 
	{
		return InvokeImpl();
	}
	Variant Invoke(Argument &arg0) const override
	{
		return InvokeImpl(arg0);
	}
	Variant Invoke(Argument &arg0, Argument &arg1) const override
	{
		return InvokeImpl(arg0, arg1);
	}
	Variant Invoke(Argument &arg0, Argument &arg1, Argument &arg2) const override
	{
		return InvokeImpl(arg0, arg1, arg2);
	}
	Variant Invoke(Argument &arg0, Argument &arg1, Argument &arg2, Argument &arg3) const override
	{
		return InvokeImpl(arg0, arg1, arg2, arg3);
	}
	Variant Invoke(Argument &arg0, Argument &arg1, Argument &arg2, Argument &arg3, Argument &arg4) const override
	{
		return InvokeImpl(arg0, arg1, arg2, arg3, arg4);
	}
	Variant Invoke(Argument &arg0, Argument &arg1, Argument &arg2, Argument &arg3, Argument &arg4, Argument &arg5) const override
	{
		return InvokeImpl(arg0, arg1, arg2, arg3, arg4, arg5);
	}
	Variant Invoke(std::vector<Argument> &args) const override
	{
		return InvokeImplHelper(args, common::MakeIndexSequence<sizeof...(ParamTs)>());
	}

private:
	template <typename... ArgTs>
	std::enable_if_t<common::IsSameListSize<common::type_list<ArgTs...>, common::type_list<ParamTs...>>::value, Variant> 
	InvokeImpl(ArgTs const&... arguments) const
	{
		return Invoker::Invoke(arguments...);
	}

	template <typename... ArgTs>
	std::enable_if_t<!common::IsSameListSize<common::type_list<ArgTs...>, common::type_list<ParamTs...>>::value, Variant> 
	InvokeImpl(ArgTs const&... arguments) const
	{
		return Variant();
	}

	template <std::size_t... Ns>
	Variant InvokeImplHelper(std::vector<Argument> &args, common::IndexSequence<Ns...>) const
	{
		if (args.size() == sizeof...(Ns))
			return Invoker::Invoke(args[Ns]...);
		else
			return Variant();
	}

	ParameterInfos<ParamWrapperTs...> parameterInfos;
	std::array<Parameter, sizeof...(ParamWrapperTs)> parameters;
};

}


