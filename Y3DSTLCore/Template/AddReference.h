#pragma once

namespace AddReference_Private
{
	template<typename T>
	struct AddReference
	{
		typedef T&		Lvalue;
		typedef T&&		Rvalue;
	};
}

/**
* AddLValueReference<type> will add any l-value from a type.
* AddRValueReference<type> will add any r-value from a type.
*/
template<typename T> struct AddLValueReference	{ typedef typename AddReference_Private::AddReference<T>::Lvalue Type; };
template<typename T> struct AddRValueReference	{ typedef typename AddReference_Private::AddReference<T>::Rvalue Type; };