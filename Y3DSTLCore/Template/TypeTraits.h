#pragma once

/**
* TIsSame
*
* implementation of std::is_same trait.
*/
template<typename T, typename U>	struct IsSame			{ enum { Value = false }; };
template<typename T>				struct IsSame<T, T>		{ enum { Value = true }; };
