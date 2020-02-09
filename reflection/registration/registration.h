#pragma once
#include "registration_executor.h"

namespace reflection
{
namespace detail
{
struct BindConstructor {};
struct BindMethod {};
struct BindProperty {};
}

template <typename... T>
class Bind {};


class Registation
{
public:
	template <typename ClassType>
	class CustomClasss
	{
	public:
		CustomClasss(char const *name);
		~CustomClasss();

		template <typename... ParamTs>
		Bind<detail::BindConstructor, ClassType, ParamTs...> Constructor();
		/* Overloading funtion:
		 * struct A {
		 * void f () const;
		 * void f (int,int);
		 * }
		 * In this case, we have to cast the result of &A::f in order to remove the ambiguity
		 * for example:
		 * Method("method", static_cast<void(A::*)() const>(&A::f))
		 * Method("method", static_cast<void(A::*)(int,int)>(&A::f))
		**/
		template <typename FuncType>
		Bind<detail::BindMethod, FuncType, ClassType> Method(char const *name, FuncType func);
		template <typename PropType>
		Bind<detail::BindProperty, PropType, ClassType> Property(char const *name, PropType prop);

	protected:
		CustomClasss(std::shared_ptr<RegistrationExecutor> &inExecutor);

		std::shared_ptr<RegistrationExecutor> executor;
		template <typename... T>
		friend class Bind;
	};

private:
	std::shared_ptr<RegistrationExecutor> execute;
};

}

#include "registration_impl.h"

