#pragma once
#include "common/Types.h"

namespace common {

template <typename T, typename RealT = void>
using not_pod_array = std::enable_if_t<!is_pod_array<T>::Value, RealT>;

template <typename T, typename RealT = void>
using not_bounded_array = std::enable_if_t<!is_unbounded_array<T>::Value, RealT>;

template <typename T>
struct default_delete
{
	default_delete() = default;
	~default_delete() = default;
	ONLY_COPYABLE_FOR_CLASS(default_delete);

	template <typename U, typename = conv<T, U>>
	default_delete(const default_delete<U>&)
	{}
	template <typename U, typename = conv<T, U>>
	default_delete& operator=(const default_delete<U>&) { return *this; }
	void operator()(T* ptr) const { delete ptr; }
};

template <typename T>
struct default_delete<T[]>
{
	default_delete() = default;
	~default_delete() = default;
	ONLY_COPYABLE_FOR_CLASS(default_delete);

	template <typename U, typename = conv<T[], U[]>>
	default_delete(const default_delete<U[]>&) {}
	template <typename U, typename = conv<T[], U[]>>
	default_delete& operator=(const default_delete<U[]>&) { return *this; }
	void operator()(T* ptr) const { delete[] ptr; }
};

template <typename T, typename Deleter = default_delete<T>>
class unique_ptr : public Deleter
{
public:
	FORCEINLINE unique_ptr()
		: Deleter()
		, m_ptr(nullptr) {}

	FORCEINLINE unique_ptr(std::nullptr_t)
		: Deleter()
		, m_ptr(nullptr) {}

	FORCEINLINE unique_ptr& operator=(std::nullptr_t)
	{
		T* old_p = m_ptr;
		m_ptr = nullptr;
		deleter()(old_p);
		return *this;
	}

	unique_ptr(const unique_ptr&) = delete;
	unique_ptr& operator=(const unique_ptr&) = delete;

	template <typename U, typename = conv<T, U>>
	explicit FORCEINLINE unique_ptr(U* p)
		: Deleter()
		, m_ptr(p) {}
		
	template <typename U, typename = conv<T, U>>
	explicit FORCEINLINE unique_ptr(U* p, Deleter&& del)
		: Deleter(std::move(del))
		, m_ptr(p) {}
		
	template <typename U, typename = conv<T, U>>
	explicit FORCEINLINE unique_ptr(U* p, const Deleter& del)
		: Deleter(del)
		, m_ptr(p) {}
		
	FORCEINLINE ~unique_ptr()
	{
		deleter()(m_ptr);
	}
	
	FORCEINLINE unique_ptr(unique_ptr&& other) noexcept
		: Deleter(std::move(other.deleter()))
		  , m_ptr(other.m_ptr) 
	{
		other.m_ptr = nullptr;
	}

	FORCEINLINE unique_ptr& operator=(unique_ptr&& other) noexcept
	{
		if (this != &other) {
			T* old_p = m_ptr;
			m_ptr = other.m_ptr;
			other.m_ptr = nullptr;
			deleter()(old_p);
		}
		deleter() = std::move(other.deleter());
		return *this;
	}

	template <typename U, typename UDeleter, typename = conv<T, U>, typename = not_pod_array<U>>
	FORCEINLINE unique_ptr(unique_ptr<U, UDeleter>&& other)
		: Deleter(std::move(other.deleter()))
		, m_ptr(other.m_ptr)
	{
		other.Ptr = nullptr;
	}

	template <typename U, typename UDeleter, typename = conv<T, U>, typename = not_pod_array<U>>
	FORCEINLINE unique_ptr& operator=(unique_ptr<U, UDeleter>&& other) noexcept
	{
		T* old_p = m_ptr;
		m_ptr = other.m_ptr;
		other.m_ptr = nullptr;
		deleter()(old_p);

		deleter() = std::move(other.deleter());
		return *this;
	}

	FORCEINLINE bool valid() const { return m_ptr != nullptr; }
	FORCEINLINE explicit operator bool() const { return valid(); }
	FORCEINLINE bool operator!() const { return !valid(); }
	FORCEINLINE T* operator->() const { assert(valid()); return m_ptr; }
	FORCEINLINE T& operator*() const {  assert(valid()); return *m_ptr; }
	FORCEINLINE T* get() const { return m_ptr; }
	FORCEINLINE T* release() { T* ret = m_ptr; m_ptr = nullptr; return ret; }

	FORCEINLINE void Reset(T* p = nullptr)
	{
		if (m_ptr != p) {
			T* old_p = m_ptr;
			m_ptr = p;
			deleter()(old_p);
		}
	}

	FORCEINLINE Deleter& deleter()
	{
		return static_cast<Deleter&>(*this);
	}

	FORCEINLINE const Deleter& deleter() const
	{
		return static_cast<const Deleter&>(*this);
	}

private:
	T* m_ptr;
};
	
template <typename T, typename Deleter>
class unique_ptr<T[], Deleter> : public Deleter
{
public:
	FORCEINLINE unique_ptr()
		: Deleter()
		, m_ptr()
	{}

	FORCEINLINE unique_ptr(std::nullptr_t)
		: Deleter()
		, m_ptr()
	{}

	FORCEINLINE unique_ptr& operator=(std::nullptr_t)
	{
		T* old_p = m_ptr;
		m_ptr = nullptr;
		deleter()(old_p);
		return *this;
	}

	unique_ptr(const unique_ptr&) = delete;
	unique_ptr& operator=(const unique_ptr&) = delete;

	template <typename U, typename = conv<T[], U[]>>
	FORCEINLINE explicit  unique_ptr(U* p)
		: Deleter()
		, m_ptr(p)
	{}
		
	template <typename U, typename = conv<T[], U[]>>
	FORCEINLINE explicit unique_ptr(U* p, Deleter&& del)
		: Deleter(std::move(del))
		, m_ptr(p)
	{}
		
	template <typename U, typename = conv<T[], U[]>>
	FORCEINLINE explicit unique_ptr(U* p, const Deleter& del)
		: Deleter(del)
		, m_ptr(p)
	{}
		
	FORCEINLINE ~unique_ptr()
	{
		deleter()(m_ptr);
	}
	
	FORCEINLINE unique_ptr(unique_ptr&& other) noexcept
		: Deleter(std::move(other.deleter()))
		  , m_ptr(other.m_ptr) 
	{
		other.m_ptr = nullptr;
	}

	FORCEINLINE unique_ptr& operator=(unique_ptr&& other) noexcept
	{
		if (this != &other) {
			T* old_p = m_ptr;
			m_ptr = other.m_ptr;
			other.m_ptr = nullptr;
			deleter()(old_p);
		}
		deleter() = std::move(other.deleter());
		return *this;
	}

	template <typename U, typename UDeleter, typename = conv<T[], U[]>>
	FORCEINLINE unique_ptr(unique_ptr<U, UDeleter>&& other)
		: Deleter(std::move(other.deleter()))
		, m_ptr(other.m_ptr)
	{
		other.Ptr = nullptr;
	}

	template <typename U, typename UDeleter, typename = conv<T[], U[]>>
	FORCEINLINE unique_ptr& operator=(unique_ptr<U, UDeleter>&& other) noexcept
	{
		T* old_p = m_ptr;
		m_ptr = other.m_ptr;
		other.m_ptr = nullptr;
		deleter()(old_p);

		deleter() = std::move(other.deleter());
		return *this;
	}

	FORCEINLINE bool valid() const { return m_ptr != nullptr; }
	FORCEINLINE explicit operator bool() const { return valid(); }
	FORCEINLINE bool operator!() const { return !valid(); }
	FORCEINLINE T* operator->() const { assert(valid()); return m_ptr; }
	FORCEINLINE T& operator*() const {  assert(valid()); return *m_ptr; }
	FORCEINLINE T* get() const { return m_ptr; }
	FORCEINLINE T* release() { T* ret = m_ptr; m_ptr = nullptr; return ret; }

	template <typename U, typename = conv<T[], U[]>>
	FORCEINLINE void reset(T* p = nullptr)
	{
		T* old_p = m_ptr;
		m_ptr = p;
		deleter()(old_p);
	}

	FORCEINLINE void reset(std::nullptr_t)
	{
		T* old_p = m_ptr;
		m_ptr = nullptr;
		deleter()(old_p);
	}

	FORCEINLINE Deleter& deleter()
	{
		return static_cast<Deleter&>(*this);
	}

	FORCEINLINE const Deleter& deleter() const
	{
		return static_cast<const Deleter&>(*this);
	}

private:
	T* m_ptr;
};

template <typename LhsT, typename RhsT>
FORCEINLINE bool operator==(const unique_ptr<LhsT>& lhs, const unique_ptr<RhsT>& rhs)
{
	return lhs.Get() == rhs.Get();
}

template <typename T>
FORCEINLINE bool operator==(const unique_ptr<T>& lhs, const unique_ptr<T>& rhs)
{
	return lhs.Get() == rhs.Get();
}

template <typename LhsT, typename RhsT>
FORCEINLINE bool operator!=(const unique_ptr<LhsT>& lhs, const unique_ptr<RhsT>& rhs)
{
	return lhs.Get() != rhs.Get();
}

template <typename T>
FORCEINLINE bool operator!=(const unique_ptr<T>& lhs, const unique_ptr<T>& rhs)
{
	return lhs.Get() != rhs.Get();
}

template <typename T>
FORCEINLINE bool operator==(const unique_ptr<T>& lhs, std::nullptr_t)
{
	return !lhs.IsValid();
}

template <typename T>
FORCEINLINE bool operator==(std::nullptr_t, const unique_ptr<T>& lhs)
{
	return !lhs.IsValid();
}

template <typename T>
FORCEINLINE bool operator!=(const unique_ptr<T>& lhs, std::nullptr_t)
{
	return lhs.IsValid();
}

template <typename T>
FORCEINLINE bool operator!=(std::nullptr_t, const unique_ptr<T>& rhs)
{
	return rhs.IsValid();
}

template <typename T, typename... TArgs>
FORCEINLINE not_pod_array<T, unique_ptr<T>> make_unique(TArgs&&... args)
{
	return unique_ptr<T>(new T(std::forward<TArgs>(args)...));
}

template <typename T>
FORCEINLINE not_bounded_array<T, unique_ptr<T>> make_unique(std::size_t size)
{
	return unique_ptr<T>(new std::remove_all_extents_t<T>[size]());
}

} // namespace common
