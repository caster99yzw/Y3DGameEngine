project (common)
message (STATUS "Configuring common start!")

set (COMMON_MATH
		Math/Y3DMathFunc.h
		Math/Y3DMatrix.h
		Math/Y3DMatrixFunc.cpp
		Math/Y3DMatrixFunc.h
		Math/Y3DPI.h
		Math/Y3DQuaternion.h
		Math/Y3DQuatFunc.h
		Math/Y3DVector.h
		Math/Y3DVectorFunc.h	
	)

set (COMMON_CAMERA
		Camera/CameraEntity.cpp
		Camera/CameraEntity.h
		Camera/Y3DCamera.cpp
		Camera/Y3DCamera.h
		Camera/Y3DCameraDesc.h
		Camera/Y3DCameraOrtho.cpp
		Camera/Y3DCameraOrtho.h
		Camera/Y3DCameraPerspective.cpp
		Camera/Y3DCameraPerspective.h
	)

source_group ("Math" FILES ${COMMON_MATH})
source_group ("Camera" FILES ${COMMON_CAMERA})

set (COMMON_TEMPRORY Y3DMacro.h Types.h BitContainer.h DurationForProfile.h)

# all source files of stl project
set (SOURCE_FILES
		${COMMON_MATH}
		${COMMON_CAMERA}
		${COMMON_TEMPRORY}
	)

# include floders of stl project 
include_directories (
		"${CMAKE_CURRENT_SOURCE_DIR}/."
		"${CMAKE_CURRENT_SOURCE_DIR}/.."
	)

# generate static lib 
add_library (${PROJECT_NAME} STATIC ${SOURCE_FILES})

message (STATUS "Configuring common succeed!")
	
