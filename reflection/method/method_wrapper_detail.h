#pragma once
#include "method_wrapper.h"
#include "method_invoke.h"
#include "reflection/parameter/parameter.h"

namespace reflection
{

template <typename FuncType>
std::enable_if_t<HasClassType<FuncType>::value, Class> DeclaringClassImpl(FuncType func)
{
	return Class::Get<typename FunctionTraits<FuncType>::ClassType>();
}

template <typename FuncType>
std::enable_if_t<!HasClassType<FuncType>::value, Class> DeclaringClassImpl(FuncType func)
{
	return Class();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template <typename FuncType, typename... ArgTs>
struct ArgumentsMatching : std::integral_constant<bool, sizeof...(ArgTs) == FunctionTraits<FuncType>::ParameterCount>
{};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template <typename FuncType, typename ParamInfosType>
class MethodWrapper;


template <typename FuncType, typename... ParamWrapperTs>
class MethodWrapper<FuncType, ParameterInfos<ParamWrapperTs...>> : public MethodWrapperBase
{
	static_assert(ArgumentsMatching<FuncType, ParamWrapperTs...>::value,
		"the number of wrappers must be same as the number of parameter types");

	constexpr static std::size_t count = FunctionTraits<FuncType>::ParameterCount;
	using Invoker = MethodInvoker<FuncType, InvokerType<FuncType>, common::MakeIndexSequence<count>>;

public:
	MethodWrapper(std::string const &inName, FuncType inFunc, 
				  ParameterInfos<ParamWrapperTs...> inParameterInfos)
		: name(inName)
		, func(inFunc) 
		, parameterInfos(inParameterInfos)
		, parameters(CreateParameterArray(inParameterInfos))
	{
		Initialize();
	}
	
	bool Vaild() const override { return true; }
	std::string const &Name() const override { return name; };
	Class DeclaringClass() const override { return DeclaringClassImpl(func); };
	common::array_view<Parameter> Parameters() const override
	{
		return common::array_view<Parameter>(parameters.data(), parameters.size());
	}

	Variant Invoke(Variant &obj) const override
	{
		return InvokeImpl(obj);
	}
	Variant Invoke(Variant &obj, Argument &arg0) const override
	{
		return InvokeImpl(obj, arg0);
	}
	Variant Invoke(Variant &obj, Argument &arg0, Argument &arg1) const override
	{
		return InvokeImpl(obj, arg0, arg1);
	}
	Variant Invoke(Variant &obj, Argument &arg0, Argument &arg1, Argument &arg2) const override
	{
		return InvokeImpl(obj, arg0, arg1, arg2);
	}
	Variant Invoke(Variant &obj, Argument &arg0, Argument &arg1, Argument &arg2, Argument &arg3) const override
	{
		return InvokeImpl(obj, arg0, arg1, arg2, arg3);
	}
	Variant Invoke(Variant &obj, Argument &arg0, Argument &arg1, Argument &arg2, Argument &arg3, Argument &arg4) const override
	{
		return InvokeImpl(obj, arg0, arg1, arg2, arg3, arg4);
	}
	Variant Invoke(Variant &obj, Argument &arg0, Argument &arg1, Argument &arg2, Argument &arg3, Argument &arg4, Argument &arg5) const override
	{
		return InvokeImpl(obj, arg0, arg1, arg2, arg3, arg4, arg5);
	}
	Variant Invoke(Variant &obj, std::vector<Argument> &args) const override
	{
		constexpr size_t count = FunctionTraits<FuncType>::ParameterCount;
		return InvokeImplHelper(obj, args, common::MakeIndexSequence<count>());
	}

private:
	template <typename... ArgTs>
	std::enable_if_t<ArgumentsMatching<FuncType, ArgTs...>::value, Variant> 
	InvokeImpl(Variant &obj, ArgTs const&... arguments) const
	{
		return Invoker::Invoke(func, obj, arguments...);
	}

	template <typename... ArgTs>
	std::enable_if_t<!ArgumentsMatching<FuncType, ArgTs...>::value, Variant> 
	InvokeImpl(Variant &obj, ArgTs const&... arguments) const
	{
		return Variant();
	}

	template <std::size_t... Ns>
	Variant InvokeImplHelper(Variant &obj, std::vector<Argument> &args, common::IndexSequence<Ns...>) const
	{
		if (args.size() == sizeof...(Ns))
			return Invoker::Invoke(func, obj, args[Ns]...);
		else
			return Variant();
	}

	std::string name;
	FuncType func;
	ParameterInfos<ParamWrapperTs...> parameterInfos;
	std::array<Parameter, sizeof...(ParamWrapperTs)> parameters;
};

}

