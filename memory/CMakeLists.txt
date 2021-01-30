project (memory)
message (STATUS "Configuring memory start!")

# all source files 
set (SOURCE
	allocator.cpp
	allocator.h
	freelist_allocator.cpp
	freelist_allocator.h
	memory_utility.cpp
	memory_utility.h
	)

source_group ("" FILES ${SOURCE})

set (SOURCE_FILES
	${SOURCE}
	)

# include floders 
include_directories (
		"${CMAKE_CURRENT_SOURCE_DIR}/."
		"${CMAKE_CURRENT_SOURCE_DIR}/.."
	)

# generate static lib 
add_library (${PROJECT_NAME} STATIC ${SOURCE_FILES})

message (STATUS "Configuring memory succeed!")
	
