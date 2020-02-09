#pragma once
#include "reflection/registration/registration_manager.h"

namespace reflection
{
template<typename T>
Class CreateOrGetClass()
{
	static Class val = Class(RegistrationManager().Instance().AddItem(MakeClassData<T>()));
	return val;
}

template <typename T>
Class reflection::Class::Get()
{
	return CreateOrGetClass<T>();
}

template <typename T>
Class reflection::Class::Get(T &&object)
{
	return Class();

}

}


