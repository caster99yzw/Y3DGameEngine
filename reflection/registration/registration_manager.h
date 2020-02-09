#pragma once
#include "reflection/class/class.h"

namespace reflection
{

class RegistrationManager
{
public:
	RegistrationManager() = default;
	RegistrationManager(RegistrationManager const &) = delete;
	RegistrationManager &operator=(RegistrationManager const &) = delete;
	
	static RegistrationManager &Instance();

	ClassData *AddItem(std::unique_ptr<ClassData> inClass);
	bool AddItem(std::unique_ptr<ConstructorWrapperBase> inConstructor);
	bool AddItem(std::unique_ptr<DestructorWrapperBase> inDestructor);
	bool AddItem(std::unique_ptr<MethodWrapperBase> inMethod);
	bool AddItem(std::unique_ptr<PropertyWrapperBase> inProperty);

	bool RegisterCustomName(Class &class_, char const *name);
	Class FindClassByCunstomName(char const *name);

private:
	std::vector<std::unique_ptr<ClassData>> classData;
	std::vector<std::unique_ptr<ConstructorWrapperBase>> constructors;
	std::vector<std::unique_ptr<DestructorWrapperBase>> destructors;
	std::vector<std::unique_ptr<MethodWrapperBase>> methods;
	std::vector<std::unique_ptr<PropertyWrapperBase>> properties;

	std::unordered_map<std::string, Class> customNameToClass;
};

template<typename Item>
void StoreItem(Item item)
{
	auto &manager = RegistrationManager::Instance();
	auto ret = manager.AddItem(std::move(item));
	assert(ret);
}

}

