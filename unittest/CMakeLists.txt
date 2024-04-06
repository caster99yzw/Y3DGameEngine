project(unittest)
message (STATUS "Configuring unittest start!")

set (SRC
	src/001_regex.cpp
	)

source_group ("src" FILES ${SRC})

# include floders of stl project 
include_directories (
	"${CMAKE_CURRENT_SOURCE_DIR}/."
	"${CMAKE_CURRENT_SOURCE_DIR}/.."
	"${CMAKE_CURRENT_SOURCE_DIR}/googletest/include"
	)

add_executable (unittest ${SRC} gtest_main.cpp)
set_property(TARGET unittest PROPERTY FOLDER UNITTEST)
target_link_libraries (unittest gtest regex)

add_subdirectory (googletest)

message (STATUS "Configuring unittest succeed!")
	