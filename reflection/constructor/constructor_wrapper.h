#pragma once
#include "reflection/core.h"
#include "reflection/array_view.h"

namespace reflection
{

class Variant;
class Class;
class Argument;
class Parameter;


class ConstructorWrapperBase
{
public:
	virtual bool Vaild() const;
	virtual Class DeclaringClass() const;
	virtual common::array_view<Parameter> Parameters() const;

	virtual Variant Invoke() const;
	virtual Variant Invoke(Argument &arg0) const;
	virtual Variant Invoke(Argument &arg0, Argument &arg1) const;
	virtual Variant Invoke(Argument &arg0, Argument &arg1, Argument &arg2) const;
	virtual Variant Invoke(Argument &arg0, Argument &arg1, Argument &arg2, Argument &arg3) const;
	virtual Variant Invoke(Argument &arg0, Argument &arg1, Argument &arg2, Argument &arg3, Argument &arg4) const;
	virtual Variant Invoke(Argument &arg0, Argument &arg1, Argument &arg2, Argument &arg3, Argument &arg4, Argument &arg5) const;
	virtual Variant Invoke(std::vector<Argument> &args) const;

protected:
	void Initialize();
	
private:
	std::string GenerateSignatrueName();
	std::string signature;
};

}

