#pragma once
#include "reflection/core.h"

namespace reflection
{
namespace detail
{
struct HasParamName {};
struct HasNoParamName {};
}

class ParameterWrapperBase
{
public:
	virtual std::string Name() const = 0;
	virtual uint32_t Index() const = 0;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template <typename NamePolicy, typename ParamT, std::size_t ParamIndex>
class ParameterWrapper;


template <typename ParamT, std::size_t ParamIndex>
class ParameterWrapper<detail::HasParamName, ParamT, ParamIndex> : public ParameterWrapperBase
{
public:
	ParameterWrapper() = default;
	ParameterWrapper(std::string const &inName)
		: name(inName) {}

	std::string Name() const override { return name; }
	uint32_t Index() const override { return static_cast<uint32_t>(ParamIndex); }

private:
	std::string name;
};


template <typename ParamT, std::size_t ParamIndex>
class ParameterWrapper<detail::HasNoParamName, ParamT, ParamIndex> : public ParameterWrapperBase
{
public:
	ParameterWrapper() = default;
	ParameterWrapper(std::string const &inName)
		: name(inName) {}

	std::string Name() const override { return name; }
	uint32_t Index() const override { return static_cast<uint32_t>(ParamIndex); }

private:
	std::string name;
};

}

