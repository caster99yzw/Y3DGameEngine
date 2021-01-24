project (io)
message (STATUS "Configuring io start!")

# all source files 
set (STREAM
	stream/stream.cpp
	stream/stream.h
	)

source_group ("" FILES ${STREAM})

set (SOURCE_FILES
	${STREAM}
	)

# include floders 
include_directories (
		"${CMAKE_CURRENT_SOURCE_DIR}/."
		"${CMAKE_CURRENT_SOURCE_DIR}/.."
	)

# generate static lib 
add_library (${PROJECT_NAME} STATIC ${SOURCE_FILES})

message (STATUS "Configuring io succeed!")
	
