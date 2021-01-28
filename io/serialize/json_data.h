#pragma once
#include <tuple>
#include "ujson.h"

namespace io {

class JsonData
{
public:
	static JsonData New();
	bool Load(const char* file_name);
	bool Save(const char* file_name) const;

	bool IsValid() const;
	void Clear();
	ujson::object* GetRoot();

	ujson::value* FindItem(const char* name, ujson::object* reference = nullptr);
	ujson::value* GetItem(const char* name, ujson::object* reference = nullptr);
	ujson::value* AddKeyValue(const char* name, ujson::value value, ujson::object* reference = nullptr);

private:
	using found_return_type = std::tuple<ujson::object*, const char*, ujson::value*>;
	found_return_type FindItemImpl(ujson::object* reference, const char* sub_name);
	ujson::value m_doc = ujson::null;
};

} // namespace io
