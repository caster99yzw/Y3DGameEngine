#pragma once

/**
* RemoveReference<type> will remove any references from a type.
*/
template <typename T> struct RemoveReference		{ typedef T Type; };
template <typename T> struct RemoveReference<T& >	{ typedef T Type; };
template <typename T> struct RemoveReference<T&&>	{ typedef T Type; };