#pragma once
#include "parameter_wrapper.h"
#include "parameter_infos.h"
#include "reflection/core.h"

namespace reflection
{

class Parameter
{
public:
	Parameter() = default;
	Parameter(ParameterWrapperBase const *inWrapper)
		: wrapper(inWrapper) {}

	std::string Name() const;
	uint32_t Index() const;

private:
	ParameterWrapperBase const *wrapper = nullptr;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template <typename ParamWrapperT>
Parameter CreateParameter(ParamWrapperT const &wrapper)
{
	return Parameter(&wrapper);
}

template <typename... ParamWrapperTs, std::size_t... Ns>
std::array<Parameter, sizeof...(ParamWrapperTs)>
CreateParameterArrayImpl(ParameterInfos<ParamWrapperTs...> const &infos, common::IndexSequence<Ns...>)
{
	return { CreateParameter(std::get<Ns>(infos.parameters))... };
}

template <typename... ParamWrapperTs>
std::array<Parameter, sizeof...(ParamWrapperTs)>
CreateParameterArray(ParameterInfos<ParamWrapperTs...> const &infos)
{
	return CreateParameterArrayImpl(infos, common::MakeIndexSequence<sizeof...(ParamWrapperTs)>());
}

}

