#pragma once

namespace reflection
{

class Variant;
class Class;

class DestructorWrapperBase
{
public:
	virtual bool Valid() const;
	virtual Class DeclaringClass() const;
	virtual bool Invoke(Variant &obj) const;
};

}


