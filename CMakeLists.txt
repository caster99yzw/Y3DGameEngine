cmake_minimum_required (VERSION 3.0)
project (Y3DGameEngine)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(EXECTUTE "Execute")
set(LIBRARY "Library")

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

add_subdirectory (stl)
add_subdirectory (common)
add_subdirectory (io)
add_subdirectory (memory)
add_subdirectory (reflection)
add_subdirectory (console)
