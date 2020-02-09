#include "registration_manager.h"
#include "reflection/class/class.h"
#include <iostream>

namespace reflection
{

reflection::RegistrationManager &RegistrationManager::Instance()
{
	static RegistrationManager *manager = nullptr;
	if (!manager)
		manager = new RegistrationManager;

	return *manager;
}

reflection::ClassData *RegistrationManager::AddItem(std::unique_ptr<ClassData> inClass)
{
	auto *ret = inClass.get();
	classData.emplace_back(std::move(inClass));
	return ret;
}

bool RegistrationManager::AddItem(std::unique_ptr<ConstructorWrapperBase> inConstructor)
{
	Class declaringClass = inConstructor->DeclaringClass();
	declaringClass.data->getClassData().constructors.emplace_back(Constructor(inConstructor.get()));
	constructors.emplace_back(std::move(inConstructor));
	return true;
}

bool RegistrationManager::AddItem(std::unique_ptr<DestructorWrapperBase> inDestructor)
{
	Class declaringClass = inDestructor->DeclaringClass();
	declaringClass.data->getClassData().destructor = Destructor(inDestructor.get());
	destructors.emplace_back(std::move(inDestructor));
	return true;
}

bool RegistrationManager::AddItem(std::unique_ptr<MethodWrapperBase> inMethod)
{
	Class declaringClass = inMethod->DeclaringClass();
	declaringClass.data->getClassData().methods.emplace_back(Method(inMethod.get()));
	methods.emplace_back(std::move(inMethod));
	return true;
}

bool RegistrationManager::AddItem(std::unique_ptr<PropertyWrapperBase> inProperty)
{
	Class declaringClass = inProperty->DeclaringClass();
	declaringClass.data->getClassData().properties.emplace_back(Property(inProperty.get()));
	properties.emplace_back(std::move(inProperty));
	return true;
}

bool RegistrationManager::RegisterCustomName(Class &class_, char const *name)
{
	std::string strkey = name;
	customNameToClass[strkey] = class_;
	return true;
}

reflection::Class RegistrationManager::FindClassByCunstomName(char const *name)
{
	std::string strkey = name;
	auto found = customNameToClass.find(strkey);
	if (found != customNameToClass.end())
		return found->second;

	return Class();
}

}

