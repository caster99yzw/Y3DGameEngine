set(TEST_TEMPRORY Source.cpp)

# include floders of stl project 
include_directories (
		"${CMAKE_CURRENT_SOURCE_DIR}/."
		"${CMAKE_CURRENT_SOURCE_DIR}/.."
	)

add_executable (test Source.cpp)
target_link_libraries (test common stl)
