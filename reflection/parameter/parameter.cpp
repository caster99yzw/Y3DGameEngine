#include "parameter.h"

namespace reflection
{

std::string Parameter::Name() const
{
	return wrapper->Name();
}

uint32_t Parameter::Index() const
{
	return wrapper->Index();
}

}

