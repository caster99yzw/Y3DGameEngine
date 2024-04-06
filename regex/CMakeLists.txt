project(regex)
message (STATUS "Configuring regex start!")

set (SOURCE_FILES
	tiny_regex.h
	tiny_regex.cpp
	)

# include floders of stl project 
include_directories (
	"${CMAKE_CURRENT_SOURCE_DIR}/."
	"${CMAKE_CURRENT_SOURCE_DIR}/.."
	)

add_library (${PROJECT_NAME} STATIC ${SOURCE_FILES})
set_property(TARGET regex PROPERTY FOLDER LIBRARY)

message (STATUS "Configuring regex succeed!")
	