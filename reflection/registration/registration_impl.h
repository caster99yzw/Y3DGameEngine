#pragma once
#include "registration_bind.h"
#include "reflection/class/class.h"
#include "reflection/registration/registration_manager.h"

namespace reflection
{

template <typename ClassType>
Registation::CustomClasss<ClassType>::CustomClasss(char const *name)
	: executor(std::make_shared<RegistrationExecutor>())
{
	auto class_ = Class::Get<ClassType>();
	RegistrationManager::Instance().RegisterCustomName(class_, name);
}

template <typename ClassType>
Registation::CustomClasss<ClassType>::CustomClasss(std::shared_ptr<RegistrationExecutor> &inExecutor)
	: executor(inExecutor)
{
}

template <typename ClassType>
Registation::CustomClasss<ClassType>::~CustomClasss()
{
}

template <typename ClassType>
template <typename... ParamTs>
Bind<detail::BindConstructor, ClassType, ParamTs...> Registation::CustomClasss<ClassType>::Constructor()
{
	assert(executor);
	return { executor };
}

template <typename ClassType>
template <typename FuncType>
Bind<detail::BindMethod, FuncType, ClassType> Registation::CustomClasss<ClassType>::Method(char const *name, FuncType func)
{
	assert(executor);
	return { executor, name, func };
}

template <typename ClassType>
template <typename PropType>
Bind<detail::BindProperty, PropType, ClassType> Registation::CustomClasss<ClassType>::Property(char const *name, PropType prop)
{
	assert(executor);
	return { executor, name, prop };
}

}



