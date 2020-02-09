#pragma once
#include "reflection/constructor/constructor.h"
#include "reflection/destructor/destructor.h"
#include "reflection/method/method.h"
#include "reflection/property/property.h"
#include "reflection/core.h"

namespace reflection
{

class Variant;
class Argument;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct ClassData
{
	struct InternalClassData
	{
		std::vector<Constructor> constructors;
		std::vector<Method> methods;
		std::vector<Property> properties;
		Destructor destructor;
	};

	using GetClassDataFunc = InternalClassData &(*)(void);
	GetClassDataFunc getClassData;
};

template <typename T>
ClassData::InternalClassData &GetClassData()
{
	static auto internalData = std::make_unique<ClassData::InternalClassData>();
	return *internalData;
}

template <typename T>
std::unique_ptr<ClassData> MakeClassData()
{
	return std::unique_ptr<ClassData>(new ClassData{ &GetClassData<T> });
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class Class
{
public:
	Class() = default;
	Class(ClassData const *inData);

	static Class Get(char const *className);

	template <typename T>
	static Class Get();

	template <typename T>
	static Class Get(T &&object);

	Variant Create(std::vector<Argument> args = std::vector<Argument>()) const;
	bool Destory(Variant &obj) const;

	Variant Invoke(char const *name, Variant &obj, std::vector<Argument> args = std::vector<Argument>()) const;

	Method GetMethod(char const *name);
	Property GetProperty(char const *name);

public:


private:
	ClassData const *data = nullptr;
	friend class RegistrationManager;
};

}

#include "class_impl.h"