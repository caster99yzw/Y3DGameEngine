#pragma once
#include "reflection/core.h"


namespace reflection
{

class Variant;
class Class;
class Argument;


class PropertyWrapperBase
{
public:
	virtual bool Vaild() const;
	virtual std::string const &Name() const;
	virtual Class DeclaringClass() const;

	virtual bool SetValue(Variant &obj, Argument &arg) const;
	virtual Variant Value(Variant &obj) const;

protected:
	void Initialize();
};

}





