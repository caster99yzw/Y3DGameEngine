#pragma once

#include "Template/Less.h"
#include "Template/Invoke.h"

#include "CoreTypes.h"

/*
* All distance type is int32, heap only used in linear containers
*
* Max-heap : All node key value equal or greater than sub node key value, CompareType is Less<T>
* Min-heap : All node key value equal or less than sub node key value, CompareType is Greater<T>
*
*/

namespace HeapPrivate
{
	/**
	* Gets the index of the left child of node at Index.
	*
	* @param	Index Node for which the left child index is to be returned.
	* @returns	Index of the left child.
	*/
	template <typename DifferenceType>
	FORCEINLINE DifferenceType HeapGetLeftChildIndex(DifferenceType Index)
	{
		return Index * 2 + 1;
	}

	/**
	* Gets the parent index for node at Index.
	*
	* @param	Index node index.
	* @returns	Parent index.
	*/
	template <typename DifferenceType>
	FORCEINLINE DifferenceType HeapGetParentIndex(DifferenceType Index)
	{
		return (Index - 1) / 2;
	}

	/**
	* Checks if node located at Index is a leaf or not.
	*
	* @param	Index Node index.
	* @returns	true if node is a leaf, false otherwise.
	*/
	template <typename DifferenceType>
	FORCEINLINE bool HeapIsLeaf(DifferenceType Index, DifferenceType Length)
	{
		return HeapGetLeftChildIndex(Index) >= Length;
	}

	/**
	* Fixes a possible violation of order property between node at Index and a child.
	*
	*/
	template <typename RandomAccessIterator, typename DifferenceType, typename CompareType>
	FORCEINLINE void HeapSiftDown(RandomAccessIterator First, DifferenceType NodeIndex, DifferenceType Length, CompareType const& Comp)
	{
		while (!HeapIsLeaf(NodeIndex, Length))
		{
			DifferenceType const LeftChildIndex = HeapGetLeftChildIndex(NodeIndex);
			DifferenceType const RightChildIndex = LeftChildIndex + 1;

			DifferenceType ChildIndex = LeftChildIndex;
			if (RightChildIndex < Length)
			{
				ChildIndex = Invoke(Comp, *(First + LeftChildIndex), *(First + RightChildIndex)) ? LeftChildIndex : RightChildIndex;
			}
			
			if (!Invoke(Comp, *(First + ChildIndex), *(First + NodeIndex)))
			{
				break;
			}

			Swap(*(First + NodeIndex), *(First + ChildIndex));
			NodeIndex = ChildIndex;
		}
	}

	/** 
	* Fixes a possible violation of order property between node at NodeIndex and a parent.
	*
	*/
	template <typename RandomAccessIterator, typename DifferenceType, typename CompareType>
	FORCEINLINE void HeapSiftUp(RandomAccessIterator First, DifferenceType NodeIndex, DifferenceType RootIndex, CompareType const& Comp)
	{
		while (NodeIndex > RootIndex)
		{
			DifferenceType ParentIndex = HeapGetParentIndex(NodeIndex);
			if (!Invoke(Comp, *(First + NodeIndex), *(First + ParentIndex)))
			{
				break;
			}

			Swap(*(First + ParentIndex), *(First + NodeIndex));
			NodeIndex = ParentIndex;
		}
	}

	template <typename RandomAccessIterator, typename CompareType>
	FORCEINLINE void PopHeapImpl(RandomAccessIterator First, RandomAccessIterator NewLast, CompareType const& Comp)
	{
		Swap(*First, *NewLast);
		HeapSiftDown(First, (First - First), (NewLast - First), Comp);
	}

	//
	template <typename RandomAccessIterator, typename DifferenceType, typename CompareType>
	FORCEINLINE void MakeHeapImpl(RandomAccessIterator First, DifferenceType Length, CompareType Comp)
	{
		if (Length < 2) return;

		DifferenceType ParentIndex = (Length - 2) / 2;
		while (ParentIndex >= 0)
		{
			HeapSiftDown(First, ParentIndex, Length, Comp);
			ParentIndex--;
		}
	}

}

template <typename RandomAccessIterator, typename CompareType>
FORCEINLINE void PushHeap(RandomAccessIterator First, RandomAccessIterator Last, CompareType const& Comp)
{
	HeapPrivate::HeapSiftUp(First, (Last - First - 1), (First - First), Comp);
}

template <typename RandomAccessIterator, typename CompareType>
FORCEINLINE void PopHeap(RandomAccessIterator First, RandomAccessIterator Last, CompareType const& Comp)
{
	HeapPrivate::PopHeapImpl(First, (Last - 1), Comp);
}

/**
* Performs heap sort on the elements.
*
*/
template <typename RandomAccessIterator, typename CompareType>
FORCEINLINE void HeapSort(RandomAccessIterator First, RandomAccessIterator Last, CompareType const& Comp)
{
	while (Last - First > 1)
	{
		PopHeap(First, Last--, Comp);
	}
}

/*
* Builds an implicit min-heap from a range of elements.
*
*/
template <typename RandomAccessIterator, typename CompareType>
FORCEINLINE void MakeHeap(RandomAccessIterator First, RandomAccessIterator Last, CompareType const& Comp)
{
	HeapPrivate::MakeHeapImpl(First, (Last - First), Comp);
}