project (io)
message (STATUS "Configuring io start!")

# all source files 
set (STREAM
	stream/binary_stream.cpp
	stream/binary_stream.h
	stream/stream.cpp
	stream/stream.h
	stream/file_stream.cpp
	stream/file_stream.h
	stream/stream_reader.cpp
	stream/stream_reader.h
	stream/stream_writer.cpp
	stream/stream_writer.h
	)

source_group ("stream" FILES ${STREAM})

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
	
