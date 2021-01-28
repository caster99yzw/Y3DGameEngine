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

set (SERIALIZE
	serialize/double-conversion.cc
	serialize/double-conversion.h
	serialize/json_data.cpp
	serialize/json_data.h
	serialize/ujson.cpp
	serialize/ujson.h
	)

source_group ("stream" FILES ${STREAM})
source_group ("serialize" FILES ${SERIALIZE})

set (SOURCE_FILES
	${STREAM}
	${SERIALIZE}
	)

# include floders 
include_directories (
		"${CMAKE_CURRENT_SOURCE_DIR}/."
		"${CMAKE_CURRENT_SOURCE_DIR}/.."
	)

# generate static lib 
add_library (${PROJECT_NAME} STATIC ${SOURCE_FILES})

message (STATUS "Configuring io succeed!")
	
