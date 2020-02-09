#pragma once
#include "reflection/array_view.h"
#include "reflection/argument/argument.h"
#include "reflection/parameter/parameter.h"

namespace reflection
{

struct ParameterComparisonHelper
{
	static bool Compare(common::array_view<Parameter> parameters, std::vector<Argument> &arguments)
	{
		std::size_t argscount = arguments.size();
		if (parameters.size() != argscount)
			return false;

		return true;
	}
};

}

