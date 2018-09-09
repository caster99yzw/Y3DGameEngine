#pragma once

/**
* Traits class which tests if a type is character.
*/
template<class T>
struct IsANSIChar
{
	enum { Value = false };
};

template<> struct IsANSIChar<			char> { enum { Value = true }; };
template<> struct IsANSIChar<signed		char> { enum { Value = true }; };
template<> struct IsANSIChar<unsigned	char> { enum { Value = true }; };
