#pragma once



namespace Y3D
{
	template <class T>
	inline void Swap(T& left, T& right)
	{
		//T temp = Move(left);
	}


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

		List Clone() const { return *this; }
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
		friend BOOL operator == (List<T> const& lhs, List<T> const& rhs);
		template <typename T>
		friend BOOL operator != (List<T> const& lhs, List<T> const& rhs);

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
		void SetCapacity();

		void FIll(T const& e);
		
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

		template <typename ..Parameter>
		static void ConstrctElementToUnitialized(T* pTo, Parameter&& ...parameter);
		static void DefaultConstructElements(T* pTo, INT32 nCount);
		static void ValueConstructElements(T* pTo, INT32 nCount, T const& value);

		static void DestructElement(T* p);
		static void DestructElement(T* pArray, INT32 nCount);

		static void MoveElements(T* pTo, T* pFrom, INT32 nCount);
		static void MoveElementsReverse(T* pTo, T* pFrom, INT32 nCount);
		static void MoveElementsUnitialized(T* pTo, T const* pFrom, INT32 nCount);

		static void CopyElements(T* pTo, T const* pFrom, INT32 nCount);
		static void CopyElementsUnitialized(T* pTo, T const* pFrom, INT32 nCount);

	private:

		T*		m_pArray;
		INT32	m_nCount;
		INT32	m_nCapacity;

	};
}


