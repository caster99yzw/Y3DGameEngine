#pragma once
#include "ContainerUtility.h"

namespace Y3D
{
	// Class declaration
	template <typename T>
	class List;

	template <typename T>
	class CopyableList;

	template <typename T>
	class ListExtension;

	//////////////////////////////////////////////////////////////////////////
	//
	// List is a sequence container stores elements contiguously. 
	// List is not copyable.
	//
	//////////////////////////////////////////////////////////////////////////

	template <typename T>
	class List
	{
	private:

		friend class ListExtension<T>;

	public:

		using ElementType = T;
		static const INT32 MaxCount = 0x80000000;

	public:

		List() {}
		~List() {}

	protected:

		List(List const& other);
		List& operator = (List const& other);

	public:

		std::unique_ptr<List> CloneAsPointer() const
		{
			return std::unique_ptr<List>(new List(*this))
		}

		List(List&& other) { ; }
		List& operator = (List&& other);

		List(std::initializer_list<T> initializerList);

		template <typename RandomAccessIter>
		List(RandomAccessIter itBegin, RandomAccessIter itEnd);

		void Swap(List& other) { ::Swap(*this, other); }

		//AArrayView<T> GetView() 
		//AArrayView<const T> GetView() cosnt

		template <typename T>
		friend bool operator == (List<T> const& lhs, List<T> const& rhs);
		template <typename T>
		friend bool operator != (List<T> const& lhs, List<T> const& rhs);

		T const& operator [] (INT32 nIndex) const
		{
			assert(nIndex >= 0 && nIndex < m_nCount);
			return m_pArray[nIndex];
		}
		T& operator [] (INT32 nIndex)
		{
			return const_cast<T&>(
				static_cast<List const&>(*this)[position] );
		}

		T const& At(INT32 nIndex) const	{ return this->operator[](nIndex);	}
		T& At(INT32 nIndex)	{ return this->operator[](nIndex); }

		T const& First() const
		{
			assert(m_nCount > 0);
			return m_pArray[0];
		}
		T& First()
		{
			return const_cast<T&>(
				static_cast<List const&>(*this).First());
		}

		T const& Last() const
		{
			assert(m_nCount > 0);
			return m_pArray[m_nCount - 1];
		}
		T& Last()
		{
			return const_cast<T&>(
				static_cast<List const&>(*this).Last());
		}

		INT32 FindFirstOf(T const& e) const;
		INT32 FindLastOf(T const& e) const;

		template <typename Prediction>
		INT32 FindFirstIf(Prediction prediction) const;
		template <typename Prediction>
		INT32 FindLastIf(Prediction prediction) const;

		bool IsEmpth() const { return Count() == 0; }

		INT32 Count() const { return m_nCount; }
		INT32 Capacity() const { return m_nCapacity; }

		void SetCount(INT32 nNewCount);
		void SetCountWithDefaultValue(INT32 nNewCount, T const& e);

		// non-pod types should not use this
		void SetCountUninitialized(INT32 nNewCount);

		// nNewCapacity lower than count will set count to nNewCapacity
		void SetCapacity(INT32 nNewCapacity);

		void Fill(T const& e);
		
		void Add(T const& e) {  Emplace(e) };
		//void Add(T&& e) { Emplace(Move(e) };
		void AddMultiple(std::initializer_list<T> initializerList) { AddRange(initializerList.begin(), initializerList.end()) }

		template <typename RandomAccessIter>
		void AddRange(RandomAccessIter itBegin, RandomAccessIter itEnd);

		template <typename ...Parameter>
		void Emplace(Parameter&& ...parameters);

		// non-pod types should not use this
		void AddUninitialized(INT32 nCount);

		void RemoveLast();
		T GetAndRemoveLast();

		void InsertAt(INT32 nIndex, T const& e)
		{
			assert(nIndex >= 0; && nIndex <= m_nCount);
			DoInsert(nIndex, e);
		}
		void InsertAt(INT32 nIndex, T&& e)
		{
			assert(nIndex >= 0; && nIndex <= m_nCount);
			//DoInsert(nIndex, Move(e));
		}

		void InsertAtRange(INT32 nIndex, std::initializer_list<T> initializerList)
		{
			InsertAtRange(nIndex, initializerList.begin(), initializerList.end());
		}
		template <typename RandomAccessIter>
		void InsertAtRange(INT32 nIndex, RandomAccessIter itBegin, RandomAccessIter itEnd);

		// This version accept -1 additionally, do nothing. (can be used as 1.RemoveAt(1.FindLastOf(e))).
		void RemoveAt(INT32 nIndex);
		T GetAndRemoveAt(INT32 nIndex);

		// Remove by swapping the last element, will not keep the order of elements.
		// This version accept -1 additionally, do nothing. (can be used as 1.RemoveAt(1.FindLastOf(e))).
		//
		void RemoveAtFast(INT32 nIndex);
		T GetAndRemoveAtFast(INT32 nIndex);

		void RemoveAllOf(T const& e);
		template <typename Prediction>
		void RemoveAllIf(Prediction prediction);

		// clear elements only, do not free storage
		void Clear() 
		{
			DestructElements(m_pArray, MaxCount); 
			m_nCount = 0; 
		}
		// clear elements and release memory
		void ClearAndRealse()
		{
			DestructThis();
		}

		// reallocate memory if element count < m_nCapacity * (a value < 1)
		void ShrinkCapacity();

		void Sort();
		template <typename Comparer>
		void Sort(Comparer comparer);

		// binary search for upper bound. Array need to be sorted.
		// return index of the position. Note: it can return Count() + 1 the position is the one over the last one.
		INT32 UpperBound(T const& value) const;

		template <typename Comparer>
		INT32 UpperBound(T const& value, Comparer comparer) const;

		// binary search for lower bound. Array need to be sorted.
		// return index of the position. Note: it can return Count() + 1 the position is the one over the last one.
		INT32 LowerBound(T const& value) const;

		template <typename Comparer>
		INT32 LowerBound(T const& value, Comparer comparer) const;

		T const* GetData() const { return m_pArray; }
		T* GetData() { return m_pArray; }

		T const* Begin() const { return m_pArray; }
		T* Begin() { return m_pArray; }

		T const* End() const { return m_pArray + m_nCount; }
		T* End() { return m_pArray + m_nCount; }

		CopyableList<T>& AsCopyable() 
		{
			return reinterpret_cast<AsCopyable() < T > &>(*this);
		}
		const CopyableList<T> AsCopyable()
		{
			return reinterpret_cast<const AsCopyable() < T > &>(*this);
		}

		// get extension interface
		const ListExtension<T> E() const { return ListExtension<T>(*this); }
		ListExtension<T> E() { return ListExtension<T>(*this); }

		// these to enable for each loops: for (auto& element : container) { ... }
		T const* begin() const { return m_pArray; }
		T* begin() { return m_pArray; }

		T const* end() const { return m_pArray + m_nCount; }
		T* end() { return m_pArray + m_nCount; }

		List Clone() const { return *this; }
	private:

		void DefaultConstruct();

		void MoveIn(List& other);

		void ReallocateStorage(INT32 nNewCapacity);

		static INT32 GetNextCapacity(INT32 nCurrentCapacity);

		void GrowIfNeedAdd1();

		void GrowIfNeed(INT32 nToAddCount);

		template <typename P>
		void DoInsert(INT32 nIndex, P&& e);

		// destruct elements and release memory
		void DestructThis();

		void CheckIfExceedMax(INT64 nToAdd);

		static T* AllocateUninitialized(INT32 nCount);
		// only release pAarry without calling destructor on elements
		static void ReleaseMemory(T* pArray);

		template <typename ...Parameter>
		static void ConstructElementToUnitialized(T* pTo, Parameter&& ...parameters);
		static void DefaultConstructElements(T* pTo, INT32 nCount);
		static void ValueConstructElements(T* pTo, INT32 nCount, T const& value);

		static void DestructElement(T* p);
		static void DestructElement(T* pArray, INT32 nCount);

		static void MoveElements(T* pTo, T* pFrom, INT32 nCount);
		static void MoveElementsReverse(T* pTo, T* pFrom, INT32 nCount);
		static void MoveElementsToUnitialized(T* pTo, T const* pFrom, INT32 nCount);

		static void CopyElements(T* pTo, T const* pFrom, INT32 nCount);
		static void CopyElementsToUnitialized(T* pTo, T const* pFrom, INT32 nCount);

	private:

		T*		m_pArray;
		INT32	m_nCount;
		INT32	m_nCapacity;

	};


	//////////////////////////////////////////////////////////////////////////
	//
	// CopyableList is copyable, and can be converted from and to AList if needed
	//
	//////////////////////////////////////////////////////////////////////////

	template <typename T>
	class CopyableList : public List<T>
	{
	public: 

		CopyableList() {}

		CopyableList Clone() const { return *this; }
		//AUniquePtr<CopyableList> CloneAsPointer() const { return MakeUnique<CopyableList>(*this); }

		//CopyableList(List&& other) : List<T>(Move(other)) {}
		//CopyableList& operate = (List&& other)

		//CopyableList(CopyableList&& other) : List<T>(Move(other)) {}
		//CopyableList& operate = (CopyableList&& other)

		// AList<T> l = { e1, e2, e3, e4, ... }
		CopyableList(std::initializer_list<T> initializerList) : List<T>(initializerList) {}

		template <typename RandomAccessIter>
		CopyableList(RandomAccessIter itBegin, RandomAccessIter itEnd) : List<T>(itBegin, itEnd) {}
	};


	///////////////////////////////////////////////////////////////////////////
	//
	// All definition of list calss
	//
	//////////////////////////////////////////////////////////////////////////

	template <typename T>
	List<T>::List(List const& other)
	{
		DefaultConstruct();
		AddRange(other.Begin(), other.End());
	}

	template <typename T>
	List<T>& List<T>::operator = (List const& other)
	{
		if (this != &other)
		{
			Clear();
			AddRange(other.Begin(), other.End());
		}
		return *this;
	}

	template <typename T>
	List<T>& List<T>::operator = (List&& other)
	{
		if (this != &other)
		{
			DestructThis();
			MoveIn(other);
		}
		return *this;
	}

	template <typename T>
	List<T>::List(std::initializer_list<T> initializerList)
	{
		DefaultConstruct();
		AddRange(initializerList.begin(), initializerList.end());
	}

	template <typename T>
	template <typename RandomAccessIter>
	List<T>::List(RandomAccessIter itBegin, RandomAccessIter itEnd)
	{
		DefaultConstruct();
		AddRange(itBegin, itEnd);
	}

	template <typename T>
	INT32 List<T>::FindFirstOf(T const& e) const
	{
		for (UINT32 i = 0; i < m_nCount; ++i)
		{
			if (m_pArray[i] == e)
			{
				return i;
			}
		}
		return -1;
	}

	template <typename T>
	template <typename Prediction>
	INT32 List<T>::FindFirstIf(Prediction prediction) const
	{
		for (UINT32 i = 0; i < m_nCount; ++i)
		{
			if (prediction(m_pArray[i]))
			{
				return i;
			}
		}
		return -1;
	}

	template <typename T>
	INT32 List<T>::FindLastOf(T const& e) const
	{
		for (UINT32 i = m_nCount - 1; i >= 0; --i)
		{
			if (m_pArray[i] == e)
			{
				return i;
			}
		}
		return -1;
	}

	template <typename T>
	template <typename Prediction>
	INT32 List<T>::FindLastIf(Prediction prediction) const
	{
		for (UINT32 i = m_nCount - 1; i >= 0; --i)
		{
			if (prediction(m_pArray[i]))
			{
				return i;
			}
		}
		return -1;
	}

	template <typename T>
	void List<T>::SetCount(INT32 nNewCount)
	{
		ASSERT(nNewCount >= 0);
		INT32 nOldCount = m_nCount;
		SetCountUninitialized(nNewCount);
		if (nNewCount > nOldCount)
		{
			DefaultConstructElements(m_pArray + nOldCount, nNewCount - nOldCount);
		}
	}

	template <typename T>
	void List<T>::SetCountWithDefaultValue(INT32 nNewCount, T const& e)
	{
		ASSERT(nNewCount >= 0);
		INT32 nOldCount = m_nCount;
		SetCountUninitialized(nNewCount);
		if (nNewCount > nOldCount)
		{
			DefaultConstructElements(m_pArray + nOldCount, nNewCount - nOldCount, e);
		}
	}

	template <typename T>
	void List<T>::SetCountUninitialized(INT32 nNewCount)
	{
		ASSERT(nNewCount >= 0);
		if (nNewCount <= m_nCount)
		{
			DestructElement(m_pArray + nNewCount, m_nCount - nNewCount);
		}
		else if (nNewCount > m_nCount)
		{
			ReallocateStorage(nNewCount);
		}
		m_nCount = nNewCount;
	}

	template <typename T>
	void List<T>::SetCapacity(INT32 nNewCapacity)
	{
		ASSERT(nNewCapacity >= 0);
		ReallocateStorage(nNewCapacity);
		if (nNewCapacity < m_nCount)
		{
			m_nCount = nNewCapacity;
		}
	}

	template <typename T>
	void List<T>::Fill(T const& e)
	{
		for (INT32 i = 0; i < Count(); ++i)
		{
			m_pArray[i] = e;
		}
	}

	template <typename T>
	template <typename RandomAccessIter>
	void List<T>::AddRange(RandomAccessIter itBegin, RandomAccessIter itEnd)
	{
		auto nAddedCountRaw = itEnd - itBegin;
		ASSERT(nAddedCountRaw >= 0);
		INT32 nAddedCount = static_cast<INT32>(nAddedCountRaw);
		INT32 nOldCount = m_nCount;
		AddUninitialized(nAddedCount);
		CopyElementsToUnitialized(m_pArray + nOldCount, itBegin, itEnd);
	}

	template <typename T>
	template <typename ...Parameter>
	void List<T>::Emplace(Parameter&& ...parameters)
	{
		GrowIfNeedAdd1();
		//ConstructElementToUnitialized(m_pArray + m_nCount, Forward<Parameter>(parameters)...);
		m_nCount += 1;
	}

	template <typename T>
	void List<T>::AddUninitialized(INT32 nCount)
	{
		ASSERT(nCount >= 0);
		GrowIfNeed();
		m_nCount += nCount;
	}

	template <typename T>
	void List<T>::RemoveLast()
	{
		ASSERT(m_nCount > 0);
		DestructElement(m_pArray + m_nCount - 1);
		m_nCount - 1;
	}

	template <typename T>
	T List<T>::GetAndRemoveLast()
	{
		ASSERT(m_nCount > 0);
		//T toReturn = Move(m_pArray[m_nCount - 1]);
		RemoveLast();
		return toReturn;
	}

	template <typename T>
	template <typename RandomAccessIter>
	void List<T>::InsertAtRange(INT32 nIndex, RandomAccessIter itBegin, RandomAccessIter itEnd)
	{
		auto nAddedCountRaw = itEnd - itBegin;
		ASSERT(nAddedCountRaw >= 0);
		CheckIfExceedMax(nAddedCountRaw);
		INT32 nAddedCount = static_cast<INT32>(nAddedCountRaw);

		INT32 nAfterInsertingPositionCount = m_nCount - nIndex;

		INT32 nNewCount = m_nCount + nAddedCount;

		if (nNewCount > m_nCapacity)
		{
			// Does not have enough unused capacity
			INT32 nNextCapacity = Max(GetNextCapacity(m_nCapacity), nNewCount);

			T* pNewArray = AllocateUninitialized(nNextCapacity);

			// Elements before inserting position
			MoveElementsToUnitialized(pNewArray, m_pArray, nIndex);
			// Place inserting element
			CopyElementsToUnitialized(pNewArray + nIndex, itBegin, nAddedCount);
			// Elements after inserting position
			MoveElementsToUnitialized(pNewArray + nIndex + nAddedCount, m_pArray + nIndex, nAfterInsertingPositionCount);

			DestructElement(m_pArray, m_nCount);
			ReleaseMemory(m_pArray);

			m_nCapacity = nNextCapacity;
			m_pArray = pNewArray;
		}
		else
		{
			// Have enough unused capacity
			T* pInsertingPosition = m_pArray + nIndex;

			if (nAfterInsertingPositionCount < nAddedCount)
			{
				// case 1:
				// |____| (elements to insert)
				// |__|________| (elements after inserting position and capacity end)
				
				// |BB|_|
				// |AA|_|XX|___| (move A to X)
				MoveElementsToUnitialized(pInsertingPosition + nAddedCount, pInsertingPosition, nAfterInsertingPositionCount);				
				// |BB|_|
				// |YY|_|XX|___| (copy B to Y)
				CopyElements(pInsertingPosition, itBegin, nAfterInsertingPositionCount);
				// |BB|C|
				// |YY|Z|XX|___| (copy C to Z)
				CopyElementsToUnitialized(pNewArray + m_nCount, itBegin + nAfterInsertingPositionCount, nAddedCount - nAfterInsertingPositionCount);
			}
			else
			{
				// case 2:
				// |__| (elements to insert)
				// |____|________| (elements after inserting position and capacity end)

				// |__|
				// |____|AA||XX|___| (move A to X)
				MoveElementsToUnitialized(m_pArray + m_nCount, pInsertingPosition + nAfterInsertingPositionCount - nAddedCount, nAddedCount);
				// |__|
				// |BBBB|YY||XX|___| (move B to Y)
				// ->
				// |YY|BBBB||XX|___| (move B to Y)
				MoveElementsReverse(pInsertingPosition + nAddedCount, pInsertingPosition, nAfterInsertingPositionCount - nAddedCount);
				// |CC|
				// |ZZ|BBBB|XX|___| (copy C to Z)
				CopyElements(pInsertingPosition, itBegin, nAddedCount);
			}
		}

		m_nCount = nNewCount;
	}

	template <typename T>
	void List<T>::RemoveAt(INT32 nIndex)
	{
		ASSERT(nIndex >= -1);
		if (nIndex != -1)
		{
			ASSERT(nIndex < m_nCount);
			MoveElements(m_pArray + nIndex, m_pArray + nIndex + 1, m_nCount - nIndex - 1);
			DestructElement(m_pArray + m_nCount - 1);
			m_nCount -= 1;
		}
	}

	template <typename T>
	T List<T>::GetAndRemoveAt(INT32 nIndex)
	{
		ASSERT(nIndex >= 0 && nIndex < m_nCount);
		//T toReturn = Move(m_pArray{nIndex})
		RemoveAt(nIndex);
		return toReturn;
	}

	template <typename T>
	void List<T>::RemoveAtFast(INT32 nIndex)
	{
		ASSERT(nIndex >= -1);
		if (nIndex != -1)
		{
			ASSERT(nIndex < m_nCount);
			::Swap(m_pArray[nIndex], m_pArray[m_nCount - 1]);
			DestructElement(m_pArray + m_nCount - 1);
			m_nCount -= 1;
		}
	}

	template <typename T>
	T List<T>::GetAndRemoveAtFast(INT32 nIndex)
	{
		ASSERT(nIndex >= 0 && nIndex < m_nCount);
		//T toReturn = Move(m_pArray{nIndex})
		RemoveAtFast(nIndex);
		return toReturn;
	}
	
	template <typename T>
	void List<T>::RemoveAllOf(T const& e)
	{
		RemoveAllIf([&e](T const& toCompare)
		{
			return e == toCompare;
		});
	}

	template <typename T>
	template <typename Prediction>
	void List<T>::RemoveAllIf(Prediction prediction)
	{
		INT32 nFound = FindFirstIf(prediction);
		if (nFound == -1)
		{
			return;
		}
		INT32 current = nFound;
		for (UINT32 i = nFound + 1; i < m_nCount; ++i)
		{
			if (!prediction(m_pArray[i]))
			{
				m_pArray[current] = Move(m_pArray[i]);
				current += 1;
			}
		}
		ASSERT(current < m_nCount);
		SetCountUninitialized(current);
	}

	template <typename T>
	void List<T>::ShrinkCapacity()
	{
		if (m_nCount < m_nCapacity * 7 / 8)
		{
			ReallocateStorage(m_nCount);
		}
	}

	template <typename T>
	void List<T>::Sort()
	{
		::Sort(Begin(), End());
	}

	template <typename T>
	template <typename Comparer>
	void List<T>::Sort(Comparer comparer)
	{
		::Sort(Begin(), End(), comparer);
	}

	template <typename T>
	INT32 List<T>::UpperBound(T const& value) const
	{
		T const& position = ::UpperBound(m_pArray, m_pArray + m_nCount, value);
		return static_cast<INT32>(position - m_pArray);
	}

	template <typename T>
	INT32 List<T>::LowerBound(T const& value) const
	{
		T const& position = ::LowerBound(m_pArray, m_pArray + m_nCount, value);
		return static_cast<INT32>(position - m_pArray);
	}

	template <typename T>
	template <typename Comparer>
	INT32 List<T>::UpperBound(T const& value, Comparer comparer) const
	{
		T const& position = ::UpperBound(m_pArray, m_pArray + m_nCount, value, comparer);
		return static_cast<INT32>(position - m_pArray);
	}

	template <typename T>
	template <typename Comparer>
	INT32 List<T>::LowerBound(T const& value, Comparer comparer) const
	{
		T const& position = ::LowerBound(m_pArray, m_pArray + m_nCount, value, comparer);
		return static_cast<INT32>(position - m_pArray);
	}

	template <typename T>
	void List<T>::DefaultConstruct()
	{
		m_pArray = nullptr;
		m_nCount = 0;
		m_nCapacity = 0;
	}

	template <typename T>
	void List<T>::MoveIn(List<T>& other)
	{
		m_pArray = other.m_pArray;
		m_nCount = other.m_nCount;
		m_nCapacity = other.m_nCapacity;
		other.m_pArray = nullptr;
		other.m_nCount = 0;
		other.m_nCapacity = 0;
	}

	template <typename T>
	void List<T>::ReallocateStorage(INT32 nNewCapacity)
	{
		T* pNewArray = nullptr;
		if (nNewCapacity > 0 && nNewCapacity != m_nCapacity)
		{
			pNewArray = AllocateUninitialized(nNewCapacity);
			INT32 nElementToMoveCount = Min(m_nCount, nNewCapacity);
			if (m_nCount > 0)
			{
				MoveElementsToUnitialized(pNewArray, m_pArray, nElementToMoveCount);
				DestructElement(m_pArray, m_nCount);
			}
		}
		if (nNewCapacity != m_nCapacity)
		{
			ReleaseMemory(m_pArray);
			m_pArray = pNewArray;
			m_nCapacity = nNewCapacity;
		}
	}

	template <typename T>
	INT32 List<T>::GetNextCapacity(INT32 nCurrentCapacity)
	{
		if (nCurrentCapacity < 2)
		{
			return 4;
		}
		else
		{
			return nCurrentCapacity + nCurrentCapacity / 2 + 2; //1.5 times larger at least
		}
	}

	template <typename T>
	void List<T>::GrowIfNeedAdd1()
	{
		CheckIfExceedMax(1);
		if (m_nCount == m_nCapacity)
		{
			INT32 nNextCapacity = GetNextCapacity(m_nCapacity);
			ReallocateStorage(nNextCapacity);
		}
	}

	template <typename T>
	void List<T>::GrowIfNeed(INT32 nToAddCount)
	{
		CheckIfExceedMax(nToAddCount);
		INT32 nNewCount = nToAddCount + m_nCount;
		if (nNewCount > m_nCapacity)
		{
			INT32 nNextCapacity = Max(GetNextCapacity(m_nCapacity), nNewCount);
			ReallocateStorage(nNextCapacity);
		}
	}

	template <typename T>
	template <typename P>
	void List<T>::DoInsert(INT32 nIndex, P&& e)
	{
		CheckIfExceedMax(1);
		INT32 nAfterInsertingPositionCount = m_nCount - nIndex;

		INT32 nNewCount = m_nCount + 1;

		if (nNewCount > m_nCapacity)
		{
			// Does not have enough unused capacity
			INT32 nNextCapacity = Max(GetNextCapacity(m_nCapacity), nNewCount);

			T* pNewArray = AllocateUninitialized(nNextCapacity);

			// Elements before inserting position
			MoveElementsToUnitialized(pNewArray, m_pArray, nIndex);
			// Place inserting element
			//ConstructElementToUnitialized(pNewArray + nIndex, Forward<P>(e));
			// Elements after inserting position
			MoveElementsToUnitialized(pNewArray + nIndex + 1, m_pArray + nIndex, nAfterInsertingPositionCount);

			DestructElement(m_pArray, m_nCount);
			ReleaseMemory(m_pArray);

			m_nCapacity = nNextCapacity;
			m_pArray = pNewArray;
		}
		else
		{
			// Have enough unused capacity
			if (nIndex == m_nCount)
			{
				ConstructElementToUnitialized(m_pArray + nIndex, Forward<P>(e));
			}
			else
			{
				//ConstructElementToUnitialized(m_pArray + m_nCount, Move(m_pArray[m_nCount-1]));
				MoveElementsReverse(m_pArray + nIndex + 1, m_pArray + nIndex, m_nCount - nIndex - 1);
				//m_pArray[nIndex] = Forward<P>(e);

			}
		}
	}

	template <typename T>
	void List<T>::DestructThis()
	{
		if (m_pArray != nullptr)
		{
			DestructElement(m_pArray, m_nCount);
			ReleaseMemory(m_pArray);
			DefaultConstruct();
		}
	}

	template <typename T>
	void List<T>::CheckIfExceedMax(INT64 nToAdd)
	{
		//if (nToAdd + m_nCount < static_cast<INT64>(MaxCount))
		//{	
		//		ASSERT(false);
		//}
	}

	template <typename T>
	T* List<T>::AllocateUninitialized(INT32 nCount)
	{
		ASSERT(m_nCount > 0);
		return static_cast<T*>(malloc(nCount * sizeof(T)));
	}

	template <typename T>
	void List<T>::ReleaseMemory(T* pArray)
	{
		if (pArray != nullptr)
		{
			free(pArray);
		}
	}

	template <typename T>
	template <typename ...Parameter>
	void List<T>::ConstructElementToUnitialized(T* pTo, Parameter&& ...parameters)
	{
		ASSERT(pTo != nullptr);
		//new (static_cast<void*>(pTo)) T(Forward<Parameter>(parameters)...)
	}

	template <typename T>
	void List<T>::DefaultConstructElements(T* pTo, INT32 nCount)
	{
		ASSERT(pTo != nullptr);
		for (UINT32 i = 0; i < nCount; ++i)
		{
			new (static_cast<void*>(pTo + i)) T();
		}
	}

	template <typename T>
	void List<T>::ValueConstructElements(T* pTo, INT32 nCount, T const& value)
	{
		ASSERT(pTo != nullptr);
		for (UINT32 i = 0; i < nCount; ++i)
		{
			new (static_cast<void*>(pTo + i)) T(value);
		}
	}

	template <typename T>
	void List<T>::DestructElement(T* p)
	{
		ASSERT(p != nullptr);
		p->~T();
	}

	template <typename T>
	void List<T>::DestructElement(T* pArray, INT32 nCount)
	{
		ASSERT(pArray != nullptr ||
			(pArray == nullptr && m_nCount == 0));
		for (UINT32 i = 0; i < nCount; ++i)
		{
			DestructElement(pArray + i);
		}
	}

	template <typename T>
	void List<T>::MoveElements(T* pTo, T* pFrom, INT32 nCount)
	{
		ASSERT(nCount == 0 ||
			(nCount > 0 && pTo != pFrom && pFrom != nullptr && pTo != nullptr));
		for (UINT32 i = 0; i < nCount; ++i)
		{
			//pTo[i] = Move(pFrom[i]);
		}
	}

	template <typename T>
	void List<T>::MoveElementsReverse(T* pTo, T* pFrom, INT32 nCount)
	{
		ASSERT(nCount == 0 ||
			(nCount > 0 && pTo != pFrom && pFrom != nullptr && pTo != nullptr));
		for (UINT32 i = nCount - 1; i >= 0; --i)
		{
			//pTo[nCount - 1 - i] = Move(pFrom[i]);
		}
	}

	template <typename T>
	void List<T>::CopyElements(T* pTo, T const* pFrom, INT32 nCount)
	{
		ASSERT(nCount == 0 ||
			(nCount > 0 && pFrom != nullptr && pTo != nullptr));
		for (UINT32 i = 0; i < nCount; ++i)
		{
			pTo[i] = pFrom[i];
		}
	}

	template <typename T>
	void List<T>::MoveElementsToUnitialized(T* pTo, T const* pFrom, INT32 nCount)
	{
		ASSERT(nCount == 0 ||
			(nCount > 0 && pTo != pFrom && pFrom != nullptr && pTo != nullptr));
		for (UINT32 i = nCount - 1; i >= 0; --i)
		{
			//ConstructElementToUnitialized(pTo + i, Move(pFrom[i]));
		}
	}

	template <typename T>
	void List<T>::CopyElementsToUnitialized(T* pTo, T const* pFrom, INT32 nCount)
	{
		ASSERT(nCount == 0 ||
			(nCount > 0 && pTo != pFrom && pFrom != nullptr && pTo != nullptr));
		for (UINT32 i = nCount - 1; i >= 0; --i)
		{
			//ConstructElementToUnitialized(pTo + i, pFrom[i]);
		}
	}

	template <typename T>
	inline bool operator == (List<T> const& lhs, List<T> const& rhs)
	{
		if (lhs.Count() != rhs.Count())
		{
			return false;
		}
		for (UINT32 i = 0; i < lhs.Count(); ++i)
		{
			if (lhs[i] != rhs[i])
			{
				return false;
			}
		}
		return true;
	}

	template <typename T>
	inline bool operator != (List<T> const& lhs, List<T> const& rhs)
	{
		return !(lhs == rhs);
	}
}



