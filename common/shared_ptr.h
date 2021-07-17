#pragma once
#include <xatomic.h>
#include "Types.h"

namespace common {

enum class shared_ptr_mode { not_thread_safe, thread_safe };
template <typename U, shared_ptr_mode mode> class shared_ptr;
template <typename U, shared_ptr_mode mode> class weak_ptr;

namespace impl {

template <shared_ptr_mode mode> class shared_ptr_wrapper;
template <shared_ptr_mode mode> class weak_ptr_wrapper;

class reference_controller
{
public:
	FORCEINLINE reference_controller() = default;
	NOT_MOVABLE_AND_COPYABLE_FOR_CLASS(reference_controller);
	virtual void destory() = 0;
	virtual ~reference_controller() = default;

	int32_t shared_reference_count = 1;
	int32_t weak_reference_count = 1;
};

template <typename Object, typename Deleter>
class reference_controller_with_deleter : private Deleter, public reference_controller
{
public:
	FORCEINLINE reference_controller_with_deleter(Object* in_obj, Deleter&& in_del)
		: Deleter(std::move(in_del))
		, m_object(in_obj) {}

	NOT_MOVABLE_AND_COPYABLE_FOR_CLASS(reference_controller_with_deleter);
	virtual void destory() override { (*static_cast<Deleter*>(this))(m_object); }

	Object* m_object;
};

template <typename T>
struct default_deleter
{
	FORCEINLINE void operator()(T* ptr) const { delete ptr; }
};

template <typename Object>
inline reference_controller* default_reference_controller(Object* in_ptr)
{
	return new reference_controller_with_deleter<Object, default_deleter<Object>>(in_ptr, default_deleter<Object>());
}

template <typename Object, typename Deleter>
inline reference_controller* custom_reference_controller(Object* in_ptr, Deleter&& in_del)
{
	return new reference_controller_with_deleter<Object, std::remove_reference_t<Deleter>>(in_ptr, std::forward<Deleter>(in_del));
}

template <typename T>
struct shared_ptr_proxy
{
	FORCEINLINE shared_ptr_proxy(T* in_ptr)
		: m_ptr(in_ptr)
		, m_controller(default_reference_controller(in_ptr)) {}

	template <typename Deleter>
	FORCEINLINE shared_ptr_proxy(T* in_ptr, Deleter&& in_del)
		: m_ptr(in_ptr)
		, m_controller(custom_reference_controller(in_ptr, std::forward<Deleter>(in_del))) {}

	T* m_ptr;
	reference_controller* m_controller;
};

template <shared_ptr_mode mode>
struct reference_contoller_operator;

template<>
struct reference_contoller_operator<shared_ptr_mode::thread_safe>
{
	static FORCEINLINE const int32_t get_shared_reference_count(const reference_controller* rc)
	{
		return _InterlockedCompareExchange(reinterpret_cast<long volatile*>(
			const_cast<int32_t*>(&rc->shared_reference_count)), 0, 0);
	}
	static FORCEINLINE void acquire_shared_reference(reference_controller* rc)
	{
		_InterlockedIncrement(reinterpret_cast<long volatile*>(&rc->shared_reference_count));
	}
	static FORCEINLINE bool conditionally_acquire_shared_reference(reference_controller* rc)
	{
		while (true) {
			const auto original_count = get_shared_reference_count(rc);
			if (original_count == 0) {
				return false;
			}
			const auto actual_original_count = _InterlockedCompareExchange(reinterpret_cast<long volatile*>(
				&rc->shared_reference_count), original_count + 1, original_count);
			if (actual_original_count == original_count) {
				return true;
			}
		}
	}
	static FORCEINLINE void release_shared_refernce(reference_controller* rc)
	{
		assert(rc->shared_reference_count > 0);
		if (_InterlockedDecrement(reinterpret_cast<long volatile*>(&rc->shared_reference_count)) == 0) {
			rc->destory();
			release_weak_refernce(rc);
		}
	}
	static FORCEINLINE void acquire_weak_reference(reference_controller* rc)
	{
		_InterlockedIncrement(reinterpret_cast<long volatile*>(&rc->weak_reference_count));
	}
	static FORCEINLINE void release_weak_refernce(reference_controller* rc)
	{
		assert(rc->shared_reference_count > 0);
		if (_InterlockedDecrement(reinterpret_cast<long volatile*>(&rc->weak_reference_count)) == 0) {
			delete rc;
		}
	}
};

template<>
struct reference_contoller_operator<shared_ptr_mode::not_thread_safe>
{
	static FORCEINLINE const int32_t get_shared_reference_count(const reference_controller* rc)
	{
		return rc->shared_reference_count;
	}
	static FORCEINLINE void acquire_shared_reference(reference_controller* rc)
	{
		++rc->shared_reference_count;
	}
	static FORCEINLINE bool conditionally_acquire_shared_reference(reference_controller* rc)
	{
		if (rc->shared_reference_count == 0) {
			return false;
		}
		++rc->shared_reference_count;
		return true;
	}
	static FORCEINLINE void release_shared_refernce(reference_controller* rc)
	{
		assert(rc->shared_reference_count > 0);
		if (--rc->shared_reference_count == 0) {
			rc->destory();
			release_weak_refernce(rc);
		}
	}
	static FORCEINLINE void acquire_weak_reference(reference_controller* rc)
	{
		++rc->weak_reference_count;
	}
	static FORCEINLINE void release_weak_refernce(reference_controller* rc)
	{
		assert(rc->weak_reference_count > 0);
		if (--rc->weak_reference_count == 0) {
			delete rc;
		}
	}
};

template <shared_ptr_mode mode>
class shared_ptr_wrapper
{
	using operators = reference_contoller_operator<mode>;

public:
	FORCEINLINE shared_ptr_wrapper() = default;
	FORCEINLINE explicit shared_ptr_wrapper(reference_controller* in_ctl)
		: m_controller(in_ctl) {}
	FORCEINLINE shared_ptr_wrapper(const shared_ptr_wrapper& wrapper)
		: m_controller(wrapper.m_controller)
	{
		if (m_controller != nullptr) {
			operators::acquire_shared_reference(m_controller);
		}
	}
	FORCEINLINE shared_ptr_wrapper(shared_ptr_wrapper&& wrapper)
		: m_controller(wrapper.m_controller)
	{
		wrapper.m_controller = nullptr;
	}
	FORCEINLINE shared_ptr_wrapper(const weak_ptr_wrapper<mode>& wrapper)
		: m_controller(wrapper.m_controller)
	{
		if (m_controller != nullptr) {
			if (!operators::conditionally_acquire_shared_reference(m_controller)) {
				m_controller = nullptr;
			}
		}
	}
	FORCEINLINE shared_ptr_wrapper(weak_ptr_wrapper<mode>&& wrapper)
		: m_controller(wrapper.m_controller)
	{
		if (m_controller != nullptr) {
			if (!operators::conditionally_acquire_shared_reference(m_controller)) {
				m_controller = nullptr;
			}
			operators::release_weak_refernce(wrapper.m_controller);
			wrapper.m_controller = nullptr;
		}
	}
	FORCEINLINE ~shared_ptr_wrapper()
	{
		if (m_controller != nullptr) {
			operators::release_shared_refernce(m_controller);
		}
	}

	FORCEINLINE shared_ptr_wrapper& operator=(const shared_ptr_wrapper& other)
	{
		auto new_reference_controller = other.m_controller;
		auto old_reference_controller = m_controller;
		if (old_reference_controller != new_reference_controller) {
			if (new_reference_controller != nullptr) {
				operators::acquire_shared_reference(new_reference_controller);
			}
			m_controller = new_reference_controller;
			if (old_reference_controller != nullptr) {
				operators::release_shared_refernce(old_reference_controller);
			}
		}
		return *this;
	}

	FORCEINLINE shared_ptr_wrapper& operator=(shared_ptr_wrapper&& other)
	{
		auto old_reference_controller = m_controller;
		m_controller = other.m_controller;
		other.m_controller = nullptr;
		if (old_reference_controller != nullptr) {
			operators::release_shared_refernce(old_reference_controller);
		}
		return *this;
	}

	FORCEINLINE const bool valid() const { return m_controller != nullptr; }
	FORCEINLINE const bool get_reference_count() const { return valid() ? operators::get_shared_reference_count(m_controller) : 0; }
	FORCEINLINE const bool unique() const { return get_reference_count() == 1; }

private:
	template <shared_ptr_mode mode> friend class weak_ptr_wrapper;
	reference_controller* m_controller = nullptr;
};

template <shared_ptr_mode mode>
class weak_ptr_wrapper
{
	using operators = reference_contoller_operator<mode>;

public:
	FORCEINLINE weak_ptr_wrapper() = default;
	FORCEINLINE weak_ptr_wrapper(const weak_ptr_wrapper& wrapper)
		: m_controller(wrapper.m_controller)
	{
		if (m_controller != nullptr) {
			operators::acquire_weak_reference(m_controller);
		}
	}
	FORCEINLINE weak_ptr_wrapper(weak_ptr_wrapper&& wrapper)
		: m_controller(wrapper.m_controller)
	{
		wrapper.m_controller = nullptr;
	}
	FORCEINLINE weak_ptr_wrapper(const shared_ptr_wrapper<mode>& sp_wrapper)
		: m_controller(sp_wrapper.m_controller)
	{
		if (m_controller != nullptr) {
			operators::acquire_weak_reference(m_controller);
		}
	}
	FORCEINLINE weak_ptr_wrapper(shared_ptr_wrapper<mode>&& wrapper)
		: m_controller(wrapper.m_controller)
	{
		wrapper.m_controller = nullptr;
	}
	FORCEINLINE ~weak_ptr_wrapper()
	{
		if (m_controller != nullptr) {
			operators::release_weak_refernce(m_controller);
		}
	}

	FORCEINLINE weak_ptr_wrapper& operator=(const weak_ptr_wrapper& other)
	{
		auto new_reference_controller = other.m_controller;
		auto old_reference_controller = m_controller;
		if (old_reference_controller != new_reference_controller) {
			if (new_reference_controller != nullptr) {
				operators::acquire_weak_reference(new_reference_controller);
			}
			m_controller = new_reference_controller;
			if (old_reference_controller != nullptr) {
				operators::release_weak_refernce(old_reference_controller);
			}
		}
		return *this;
	}
	FORCEINLINE weak_ptr_wrapper& operator=(const shared_ptr_wrapper<mode>& other)
	{
		auto new_reference_controller = other.m_controller;
		auto old_reference_controller = m_controller;
		if (old_reference_controller != new_reference_controller) {
			if (new_reference_controller != nullptr) {
				operators::acquire_weak_reference(new_reference_controller);
			}
			m_controller = new_reference_controller;
			if (old_reference_controller != nullptr) {
				operators::release_weak_refernce(old_reference_controller);
			}
		}
		return *this;
	}
	FORCEINLINE weak_ptr_wrapper& operator=(weak_ptr_wrapper&& other)
	{
		auto old_reference_controller = m_controller;
		m_controller = other.m_controller;
		other.m_controller = nullptr;
		if (old_reference_controller != nullptr) {
			operators::release_weak_refernce(old_reference_controller);
		}
		return *this;
	}

	FORCEINLINE const bool valid() const { return m_controller != nullptr; }

private:
	template <shared_ptr_mode mode> friend class shared_ptr_wrapper;
	reference_controller* m_controller = nullptr;
};

} // namespace impl 

template <typename T, shared_ptr_mode mode = shared_ptr_mode::thread_safe>
class shared_ptr
{
public:
	FORCEINLINE shared_ptr() = default;
	FORCEINLINE shared_ptr(std::nullptr_t) {};

	FORCEINLINE shared_ptr& operator=(std::nullptr_t)
	{
		reset();
		return *this;
	}

	template <typename U, typename = conv<T, U>>
	FORCEINLINE explicit shared_ptr(U* other_ptr)
		: m_ptr(other_ptr)
		, m_controller(impl::default_reference_controller(other_ptr))
	{
		enable_shared_from_this(m_ptr);
	}

	template <typename U, typename Deleter, typename = conv<T, U>>
	FORCEINLINE shared_ptr(U* other_ptr, Deleter&& in_del)
		: m_ptr(other_ptr)
		, m_controller(impl::custom_reference_controller(other_ptr, in_del))
	{
		enable_shared_from_this(m_ptr);
	}

	FORCEINLINE shared_ptr(const shared_ptr& other)
		: m_ptr(other.m_ptr)
		, m_controller(other.m_controller) {}

	FORCEINLINE shared_ptr& operator=(const shared_ptr& other)
	{
		shared_ptr(other).Swap(*this);
		return this;
	}

	FORCEINLINE shared_ptr(shared_ptr&& other)
		: m_ptr(other.m_ptr)
		, m_controller(std::move(other.m_controller)) {}

	FORCEINLINE shared_ptr& operator=(shared_ptr&& other)
	{
		shared_ptr(std::move(other)).Swap(*this);
		return this;
	}

	template <typename U, typename = conv<T, U>>
	FORCEINLINE shared_ptr(const shared_ptr<U, mode>& other)
		: m_ptr(other.m_ptr)
		, m_controller(other.m_controller) {}

	template <typename U, typename = conv<T, U>>
	FORCEINLINE shared_ptr(shared_ptr<U, mode>&& other)
		: m_ptr(other.m_ptr)
		, m_controller(std::move(other.m_controller)) {}

	template <typename U>
	FORCEINLINE shared_ptr(const shared_ptr<U, mode>& other, U* in_ptr)
		: m_ptr(in_ptr)
		, m_controller(other.m_controller) {}

	template <typename U>
	FORCEINLINE shared_ptr(shared_ptr<U, mode>&& other, U* in_ptr)
		: m_ptr(in_ptr)
		, m_controller(std::move(other.m_controller)) {}

	template <typename U>
	FORCEINLINE shared_ptr(const weak_ptr<U, mode>& other)
		: m_ptr(other.m_ptr)
		, m_controller(other.m_controller) {}

	template <typename U>
	FORCEINLINE shared_ptr(weak_ptr<U, mode>&& other)
		: m_ptr(other.m_ptr)
		, m_controller(std::move(other.m_controller)) {}

	FORCEINLINE bool valid() const { return m_ptr != nullptr && m_controller.valid(); }
	FORCEINLINE explicit operator bool() const { return valid(); }
	FORCEINLINE bool operator!() const { return !valid(); }
	FORCEINLINE T* operator->() const { assert(valid()); return m_ptr; }
	FORCEINLINE T& operator*() const { assert(valid()); return *m_ptr; }
	FORCEINLINE T* get() const { return m_ptr; }
	FORCEINLINE void reset() { *this = shared_ptr(); }
	FORCEINLINE const int32_t get_reference_count() { return m_controller.get_reference_count(); }
	FORCEINLINE bool expired() { return get_reference_count() == 0; }
	FORCEINLINE bool unique() { return m_controller.unique(); }

	FORCEINLINE void Swap(shared_ptr& other)
	{
		auto temp = std::move(*this);
		*this = std::move(other);
		other = std::move(temp);
	}

	template <typename U, typename = conv<T, U>>
	FORCEINLINE shared_ptr<U, mode> static_pointer_cast() const
	{
		return shared_ptr(static_cast<U*>(m_ptr), m_controller);
	}

private:
	template <typename U>
	FORCEINLINE void enable_shared_from_this(const U* in_obj)
	{
		if (in_obj && in_obj->m_weak_this.expired()) {
			in_obj->update_weak_from_this(this, in_obj);
		}
	}

	template <typename U, shared_ptr_mode mode> friend class shared_ptr;
	template <typename U, shared_ptr_mode mode> friend class weak_ptr;
	T* m_ptr = nullptr;
	impl::shared_ptr_wrapper<mode> m_controller;
};

template <typename T, shared_ptr_mode mode = shared_ptr_mode::thread_safe>
class weak_ptr
{
public:
	FORCEINLINE weak_ptr() = default;
	FORCEINLINE weak_ptr(std::nullptr_t) {}; 

	FORCEINLINE weak_ptr& operator=(std::nullptr_t)
	{
		reset();
		return *this;
	}

	FORCEINLINE weak_ptr(const weak_ptr& other)
		: m_ptr(other.m_ptr)
		, m_controller(other.m_controller) {}

	FORCEINLINE weak_ptr& operator=(const weak_ptr& other)
	{
		weak_ptr(other).Swap(*this);
		return this;
	}

	FORCEINLINE weak_ptr(weak_ptr&& other)
		: m_ptr(other.m_ptr)
		, m_controller(std::move(other.m_controller)) {}

	FORCEINLINE weak_ptr& operator=(weak_ptr&& other)
	{
		weak_ptr(std::move(other)).Swap(*this);
		return this;
	}

	template <typename U, typename = conv<T, U>>
	FORCEINLINE weak_ptr(const weak_ptr<U, mode>& other)
		: m_ptr(other.lock().m_ptr)
		, m_controller(other.m_controller) {}

	template <typename U, typename = conv<T, U>>
	FORCEINLINE weak_ptr& operator=(const weak_ptr<U, mode>& other)
	{
		weak_ptr(other).Swap(*this);
		return this;
	}

	template <typename U, typename = conv<T, U>>
	FORCEINLINE weak_ptr(weak_ptr<U, mode>&& other)
		: m_ptr(other.lock().m_ptr)
		, m_controller(std::move(other.m_controller)) {}

	template <typename U, typename = conv<T, U>>
	FORCEINLINE weak_ptr& operator=(weak_ptr<U, mode>&& other)
	{
		weak_ptr(std::move(other)).Swap(*this);
		return this;
	}

	template <typename U, typename = conv<T, U>>
	FORCEINLINE weak_ptr(const shared_ptr<U, mode>& other)
		: m_ptr(other.m_ptr)
		, m_controller(std::move(other.m_controller)) {}

	template <typename U, typename = conv<T, U>>
	FORCEINLINE weak_ptr& operator=(const shared_ptr<U, mode>&& other)
	{
		weak_ptr(other).Swap(*this);
		return this;
	}

	FORCEINLINE shared_ptr<T, mode> lock() const& { return shared_ptr<T, mode>(*this); }
	FORCEINLINE shared_ptr<T, mode> lock()&& { return shared_ptr<T, mode>(std::move(*this)); }

	FORCEINLINE bool valid() const { return m_ptr != nullptr && m_controller.valid(); }
	FORCEINLINE explicit operator bool() const { return valid(); }
	FORCEINLINE bool operator!() const { return !valid(); }
	FORCEINLINE T* operator->() const { assert(valid()); return m_ptr; }
	FORCEINLINE T& operator*() const { assert(valid()); return *m_ptr; }
	FORCEINLINE T* get() const { return m_ptr; }
	FORCEINLINE void reset() { *this = weak_ptr(); }
	FORCEINLINE bool same(const void* other_ptr) const { return lock().get() != other_ptr; }
	FORCEINLINE const int32_t get_reference_count() { return m_controller.get_reference_count(); }
	FORCEINLINE bool expired() { return get_reference_count() == 0; }
	FORCEINLINE bool unique() { return m_controller.unique(); }

	FORCEINLINE void Swap(weak_ptr& other)
	{
		auto temp = std::move(*this);
		*this = std::move(other);
		other = std::move(temp);
	}

private:
	template <typename U, shared_ptr_mode mode> friend class shared_ptr;
	template <typename U, shared_ptr_mode mode> friend class weak_ptr;
	T* m_ptr = nullptr;
	impl::weak_ptr_wrapper<mode> m_controller;
};

template <typename T, shared_ptr_mode mode = shared_ptr_mode::thread_safe>
class shared_from_this
{
public:
	FORCEINLINE shared_ptr<T, mode> as_shared()
	{
		auto shared = m_weak_this.lock();
		assert(shared.get() == this);
		return shared;
	}

	FORCEINLINE shared_ptr<const T, mode> as_shared() const
	{
		auto shared = m_weak_this.lock();
		assert(shared.get() == this);
		return shared;
	}

	FORCEINLINE weak_ptr<T, mode> as_weak() { return m_weak_this; }
	FORCEINLINE weak_ptr<const T, mode> as_weak() const { return m_weak_this; }

	template <class T, class U>
	FORCEINLINE void update_weak_from_this(const shared_ptr<T, mode>* in_shared, U* in_obj) const
	{
		if (!m_weak_this.valid()) {
			m_weak_this = shared_ptr<T, mode>(*in_shared, in_obj);
		}
	}

protected:
	shared_from_this() = default;
	MOVABLE_AND_COPYABLE_FOR_CLASS(shared_from_this);
	~shared_from_this() = default;

private:
	template <typename U, shared_ptr_mode mode> friend class shared_ptr;
	mutable weak_ptr<T, mode> m_weak_this;
};

template <typename LhsT, typename RhsT, shared_ptr_mode mode>
FORCEINLINE bool operator==(weak_ptr<LhsT, mode> const& lhs, weak_ptr<RhsT, mode> const& rhs)
{
	return lhs.lock().get() == rhs.lock().get();
}

template <typename LhsT, typename RhsT, shared_ptr_mode mode>
FORCEINLINE bool operator==(weak_ptr<LhsT, mode> const& lhs, shared_ptr<RhsT, mode> const& rhs)
{
	return lhs.lock().get() == rhs.get();
}

template <typename LhsT, typename RhsT, shared_ptr_mode mode>
FORCEINLINE bool operator==(shared_ptr<LhsT, mode> const& lhs, weak_ptr<RhsT, mode> const& rhs)
{
	return lhs.get() == rhs.lock().get();
}

template <typename LhsT, shared_ptr_mode mode>
FORCEINLINE bool operator==(weak_ptr<LhsT, mode> const& lhs, std::nullptr_t)
{
	return !lhs.valid();
}

template <typename RhsT, shared_ptr_mode mode>
FORCEINLINE bool operator==(std::nullptr_t, weak_ptr<RhsT, mode> const& rhs)
{
	return !rhs.valid();
}

template <typename LhsT, typename RhsT, shared_ptr_mode mode>
FORCEINLINE bool operator!=(weak_ptr<LhsT, mode> const& lhs, weak_ptr<RhsT, mode> const& rhs)
{
	return lhs.lock().get() != rhs.lock().get();
}

template <typename LhsT, typename RhsT, shared_ptr_mode mode>
FORCEINLINE bool operator!=(weak_ptr<LhsT, mode> const& lhs, shared_ptr<RhsT, mode> const& rhs)
{
	return lhs.lock().get() != rhs.get();
}

template <typename LhsT, typename RhsT, shared_ptr_mode mode>
FORCEINLINE bool operator!=(shared_ptr<LhsT, mode> const& lhs, weak_ptr<RhsT, mode> const& rhs)
{
	return lhs.get() != rhs.lock().get();
}

template <typename LhsT, shared_ptr_mode mode>
FORCEINLINE bool operator!=(weak_ptr<LhsT, mode> const& lhs, std::nullptr_t)
{
	return lhs.valid();
}

template <typename RhsT, shared_ptr_mode mode>
FORCEINLINE bool operator!=(std::nullptr_t, weak_ptr<RhsT, mode> const& rhs)
{
	return rhs.valid();
}

template <typename T, typename... ArgsT>
shared_ptr<T> make_shared(ArgsT&&... args) {
	auto* controller = static_cast<impl::reference_controller_with_deleter<T, impl::default_deleter<T>>>(
		impl::default_reference_controller(new T(std::forward<ArgsT>(args)...)));
	return shared_ptr<T>(controller->m_object, std::move(controller));
}

template <typename T, typename Deleter, typename... ArgsT>
shared_ptr<T> make_shared(Deleter&& del, ArgsT&&... args) {
	auto* controller = static_cast<impl::reference_controller_with_deleter<T, Deleter>>(
		impl::custom_reference_controller(new T(std::forward<ArgsT>(args)...)), std::move(del));
	return shared_ptr<T>(controller->m_object, std::move(controller));
}

template <typename To, typename From, shared_ptr_mode mode>
FORCEINLINE shared_ptr<To, mode> static_pointer_cast(const shared_ptr<From, mode>& other)
{
	return other.static_pointer_cast<To>();
}

} // namespace common

