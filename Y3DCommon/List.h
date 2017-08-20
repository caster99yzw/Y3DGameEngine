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

		// This version accept -1 additionally, do nothing. (can be used as 1.RemoveAt(1.FindLastOf(e)))
		void RemoveAt(INT32 nIndex);
		T GetAndRemoveAt(INT32 nIndex);

		

	private:

		template <typename P>
		void DoInsert(INT32 nIndex, P&& e);

	private:

		T*		m_pArray;
		INT32	m_nCount;
		INT32	m_nCapacity;

	};
}


