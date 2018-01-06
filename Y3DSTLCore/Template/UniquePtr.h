#pragma once

#include "CoreTypes.h"
#include "Template/IsArray.h"
#include "Template/Template.h"
#include "Template/EnableIf.h"
#include "Template/PointerIsConvertibleFromTo.h"

// Use this when you need an object's lifetime to be strictly bound to the lifetime of a single smart pointer.
//
// This class is non-copyable - ownership can only be transferred via a move operation, e.g.:
//
// TUniquePtr<MyClass> Ptr1(new MyClass);    // The MyClass object is owned by Ptr1.
// TUniquePtr<MyClass> Ptr2(Ptr1);           // Error - TUniquePtr is not copyable
// TUniquePtr<MyClass> Ptr3(MoveTemp(Ptr1)); // Ptr3 now owns the MyClass object - Ptr1 is now nullptr.

template <typename T>
struct DefalutDeltete
{
	DefalutDeltete() = default;
	DefalutDeltete(DefalutDeltete const&) = default;
	DefalutDeltete& operator = (DefalutDeltete const&) = default;
	~DefalutDeltete() = default;

	template <
		typename U, 
		typename = typename EnableIf<PointerIsConvertibleFromTo<U, T>::Value>::Type
	>
	DefalutDeltete(DefalutDeltete<U> const&)
	{
	}

	template <
		typename U,
		typename = typename EnableIf<PointerIsConvertibleFromTo<U, T>::Value>::Type
	>
	DefalutDeltete& operator = (DefalutDeltete<U> const&)
	{
		return *this;
	}

	void operator()(T* Ptr) const
	{
		delete Ptr;
	}
};

template <typename T>
struct DefalutDeltete<T[]>
{
	DefalutDeltete() = default;
	DefalutDeltete(DefalutDeltete const&) = default;
	DefalutDeltete& operator = (DefalutDeltete const&) = default;
	~DefalutDeltete() = default;

	template <
		typename U,
		typename = typename EnableIf<PointerIsConvertibleFromTo<U[], T[]>::Value>::Type
	>
	DefalutDeltete(DefalutDeltete<U[]> const&)
	{
	}

	template <
		typename U,
		typename = typename EnableIf<PointerIsConvertibleFromTo<U[], T[]>::Value>::Type
	>
	DefalutDeltete& operator = (DefalutDeltete<U[]> const&)
	{
		return *this;
	}

	template <typename U>
	typename EnableIf<PointerIsConvertibleFromTo<U[], T[]>::Value>::Type operator()(U* Ptr) const
	{
		delete [] Ptr;
	}
};

template <typename T, typename Deleter = DefalutDeltete<T>>
class UniquePtr : private Deleter
{
	template <typename OtherT, typename OtherDeleter>
	friend class UniquePtr;

public:

	FORCEINLINE UniquePtr() : Ptr(nullptr)
	{
	}

	FORCEINLINE explicit UniquePtr(T* InPtr) : Ptr(InPtr)
	{
	}

	FORCEINLINE UniquePtr(TYPE_OF_NULLPTR) : Ptr(nullptr)
	{
	}

	FORCEINLINE UniquePtr(UniquePtr&& Other)
		: Deleter	(Move(Other.GetDeleter()))
		, Ptr		(Other.Ptr)
	{
	}

	template <
		typename OtherT,
		typename OtherDeleter,
		typename = typename EnableIf<!IsArray<OtherT>::Value>::Type
	>
	FORCEINLINE UniquePtr(UniquePtr<OtherT, OtherDeleter>&& Other)
		: Deleter	(Move(Other.GetDeleter()))
		, Ptr		(Other.Ptr)
	{
	}

	FORCEINLINE UniquePtr& operator = (UniquePtr&& Other)
	{
		if (this != &Other)
		{
			T* OldPtr = Ptr;
			Ptr = Other.Ptr;
			Other.Ptr = nullptr;
			GetDeleter()(OldPtr);
		}

		GetDeleter() = Move(Other.GetDeleter());

		return *this;
	}

	template <typename OtherT, typename OtherDeleter> 
	FORCEINLINE typename EnableIf<!IsArray<OtherT>::value_type, UniquePtr&>::Type operator = (UniquePtr<OtherT, OtherDeleter>&& Other)
	{
		T* OldPtr = Ptr;
		Ptr = Other.Ptr;
		Other.Ptr = nullptr;
		GetDeleter()(OldPtr);

		GetDeleter() = MoveTemp(Other.GetDeleter());

		return *this;
	}

	FORCEINLINE UniquePtr& operator = (TYPE_OF_NULLPTR)
	{
		T* OldPtr = Ptr;
		Ptr = nullptr;
		GetDeleter()(OldPtr);

		return *this;
	}

	FORCEINLINE ~UniquePtr()
	{
		GetDeleter()(Ptr);
	}

	bool IsVaild() const
	{
		return Ptr != nullptr;
	}

	FORCEINLINE explicit operator bool() const 
	{
		return IsVaild();
	}

	FORCEINLINE bool operator !() const
	{
		return !IsVaild();
	}

	FORCEINLINE T* operator ->() const
	{
		return Ptr;
	}

	FORCEINLINE T& operator *() const
	{
		return *Ptr;
	}

	FORCEINLINE T* Get() const
	{
		return Ptr;
	}

	/**
	* Relinquishes control of the owned object to the caller and nulls the TUniquePtr.
	*
	* @return The pointer to the object that was owned by the TUniquePtr, or nullptr if no object was being owned.
	*/
	FORCEINLINE T* Release()
	{
		T* Result = Ptr;
		Ptr = nullptr;
		return Result;
	}

	FORCEINLINE void Reset(T* InPtr = nullptr)
	{
		T* OldPtr = Ptr;
		Ptr = InPtr;
		GetDeleter()(OldPtr);
	}

	FORCEINLINE Deleter& GetDeleter()
	{
		return static_cast<Deleter&>(*this);
	}

	FORCEINLINE Deleter const& GetDeleter() const
	{
		return static_cast<Deleter const&>(*this);
	}

private:

	// Non-copyable
	UniquePtr(const UniquePtr&);
	UniquePtr& operator = (const UniquePtr&);

	T* Ptr;
};

template <typename T, typename Deleter>
class UniquePtr<T[], Deleter> : private Deleter
{


	/**
	* Relinquishes control of the owned object to the caller and nulls the TUniquePtr.
	*
	* @return The pointer to the object that was owned by the TUniquePtr, or nullptr if no object was being owned.
	*/
	FORCEINLINE T* Release()
	{
	}
};