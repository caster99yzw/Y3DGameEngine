project (stl)
message (STATUS "Configuring stl start!")

set (STL_ALGORITHMS
		Algorithms/BinaryHeap.h
		Algorithms/BinarySearch.h
	)

set (STL_CONTAINERS
		Containers/Allocator.h
		Containers/Construct.h
		Containers/Deque.h
		Containers/HashTable.h
		Containers/IsSafetyFillMemset.h
		Containers/IteratorBase.h
		Containers/List.h
		Containers/map.h
		Containers/Queue.h
		Containers/Set.h
		Containers/Slist.h
		Containers/Stack.h
		Containers/Uninitialized.h
		Containers/Array.h
	)

set (STL_TEMPLATE
		Template/AddReference.h
		Template/AndOrNot.h
		Template/Decay.h
		Template/EnableIf.h
		Template/Greater.h
		Template/IdentityFunctor.h
		Template/Invoke.h
		Template/IsArithmetic.h
		Template/IsArray.h
		Template/IsCharacter.h
		Template/IsContiguousContainer.h
		Template/IsPODType.h
		Template/IsPointer.h
		Template/IsTrivially.h
		Template/Junction.h
		Template/Less.h
		Template/PointerIsConvertibleFromTo.h
		Template/RemoveCV.h
		Template/RemoveReference.h
		Template/Source.cpp
		Template/Template.h
		Template/TypeHash.h
		Template/TypeTraits.h
	)

source_group ("algorithms" FILES ${STL_ALGORITHMS})
source_group ("Containers" FILES ${STL_CONTAINERS})
source_group ("Template" FILES ${STL_TEMPLATE})

set (STL_TEMPRORY CoreTypes.h)

# all source files of stl project
set (SOURCE_FILES
		${STL_ALGORITHMS}
		${STL_CONTAINERS}
	    ${STL_TEMPLATE}
		${STL_TEMPRORY}
	)

# include floders of stl project 
include_directories (
		"${CMAKE_CURRENT_SOURCE_DIR}/."
		"${CMAKE_CURRENT_SOURCE_DIR}/.."
	)

# generate static lib 
add_library (${PROJECT_NAME} STATIC ${SOURCE_FILES})

message (STATUS "Configuring stl succeed!")
	
