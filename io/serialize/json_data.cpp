#include "json_data.h"
#include "io/stream/file_stream.h"

namespace io {

JsonData JsonData::New()
{
	JsonData data; data.m_doc = ujson::object();
	return data;
}

bool JsonData::Load(const char* file_name)
{
	Clear();
	std::string json_data_str;
	if (!FileStream::Load(file_name, &json_data_str))
		return false;

	auto result = ujson::parse(json_data_str.data(), json_data_str.size());
	if (result.error != ujson::error_code::no_error)
		return false;

	m_doc = std::move(result.value);
	return true;
}

bool JsonData::Save(const char* file_name) const 
{
	ujson::to_string_options simple_option;
	simple_option.indent_amount = 1;
	const std::string json_data_str = ujson::to_string(m_doc, simple_option);
	if (!FileStream::Write(file_name, json_data_str))
		return false;

	return true;
}

bool JsonData::IsValid() const
{
	return m_doc.is_object();
}

void JsonData::Clear()
{
	m_doc = ujson::null;
}

ujson::object* JsonData::GetRoot()
{
	return m_doc.to_object();
}

ujson::value* JsonData::FindItem(const char* name, ujson::object* reference)
{
	auto* start = (reference) ? reference : GetRoot();
	auto ret = FindItemImpl(start, name);
	return std::get<2>(ret);
}

ujson::value* JsonData::GetItem(const char* name, ujson::object* reference)
{
	auto* start = (reference) ? reference : GetRoot();
	auto ret = FindItemImpl(start, name);
	auto* found_p = std::get<0>(ret);
	auto* found_v = std::get<2>(ret);
	if (!found_v)
	{
		found_v = AddKeyValue(std::get<1>(ret), ujson::null, found_p);
	}
	return found_v;
}

ujson::value* JsonData::AddKeyValue(const char* name, ujson::value value, ujson::object* reference)
{
	auto* start = (reference) ? reference : GetRoot();
	auto ret = FindItemImpl(start, name);
	auto* found_p = std::get<0>(ret);
	auto* found_v = std::get<2>(ret);
	if (!found_v)
	{
		found_p->emplace_back(ujson::name_value_pair(std::get<1>(ret), std::move(value)));
		found_v = &(found_p->back().second);
	}
	else
	{
		*found_v = std::move(value);
	}
	return found_v;
}

JsonData::found_return_type JsonData::FindItemImpl(ujson::object* reference, const char* sub_name)
{
	std::string p;
	const auto* end = strchr(sub_name, '/');
	if (end)
	{
		p.assign(sub_name, end - sub_name);
		sub_name = end + 1;
	}

	for (auto& value : *reference)
	{
		if (value.second.is_object())
		{
			FindItemImpl(value.second.to_object(), sub_name);
		}

		if (value.first == sub_name)
		{
			return std::make_tuple(reference, sub_name, &value.second);
		}
	}

	return std::make_tuple(reference, sub_name, nullptr);
}

} // namespace io
