project(console)
message (STATUS "Configuring console start!")

# include floders of stl project 
include_directories (
		"${CMAKE_CURRENT_SOURCE_DIR}/."
		"${CMAKE_CURRENT_SOURCE_DIR}/.."
	)

add_executable (console Source.cpp)
target_link_libraries (console common stl reflection)

message (STATUS "Configuring console succeed!")
	