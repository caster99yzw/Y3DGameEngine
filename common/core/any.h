#pragma once
#include <any>

namespace common
{

template <std::size_t len, std::size_t align>
class any
{
	enum class operation : uint8_t { copy, move, dtor, comp, addr, caddr, type };
	enum class policy : uint8_t { owner, ref, cref };

	using storage_type = std::aligned_storage_t<len + !len, align>;
	using vtable_type = const void* (*)(const operation, const any&, void*);

public:
	
private:
	union { 
		storage_type m_storage;
		const void* m_instance;
	};
	vtable_type m_vtable;
	policy m_mode;
};
	
} // namespace common