#pragma once
#include "reflection/core.h"

namespace reflection
{

class RegistrationExecutor
{
public:
	RegistrationExecutor() = default;
	~RegistrationExecutor()
	{
		for (auto &item : registrationList)
			item.second();
	}

	bool AddRegistrationList(void const *key, std::function<void(void)> callable = std::function<void(void)>())
	{
		auto found = std::find_if(registrationList.begin(), registrationList.end(),
								[=](RegistrationItem const &other) { return key == other.first; });

		if (found != registrationList.end())
		{
			found->second = std::move(callable);
			return false;
		}
		registrationList.emplace_back(std::make_pair(key, callable));
		return true;
	}
	
private:
	using RegistrationItem = std::pair<void const *, std::function<void(void)>>;
	using RegistrationList = std::vector<RegistrationItem>;
	RegistrationList registrationList;
};

}

