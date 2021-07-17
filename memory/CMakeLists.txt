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
	stack_allocator.cpp
	stack_allocator.h
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
set_property(TARGET memory PROPERTY FOLDER LIBRARY)

message (STATUS "Configuring memory succeed!")
	
