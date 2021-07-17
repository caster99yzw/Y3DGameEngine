project (reflection)
message (STATUS "Configuring reflection start!")

# all source files 
set (CONSTRUCTOR
	constructor/constructor.cpp
	constructor/constructor.h
	constructor/constructor_wrapper.cpp
	constructor/constructor_wrapper.h
	constructor/constructor_wrapper_detail.h
	)

set (DESTRUCTOR
	destructor/destructor.cpp
	destructor/destructor.h
	destructor/destructor_wrapper.cpp
	destructor/destructor_wrapper.h
	destructor/destructor_wrapper_detail.h
	)

set (METHOD
	method/function_traits.h
	method/method.cpp
	method/method.h
	method/method_invoke.h
	method/method_wrapper.cpp
	method/method_wrapper.h
	method/method_wrapper_detail.h
	)

set (PROPERTY
	property/property.cpp
	property/property.h
	property/property_accessor.h
	property/property_wrapper.cpp
	property/property_wrapper.h
	property/property_wrapper_detail.h
	)

set (CLASS
	class/class.cpp
	class/class.h
	class/class_impl.h
	)

set (ARGUMENT
	argument/argument.cpp
	argument/argument.h
	argument/argument_impl.h
	)

set (PARAMETER
    parameter/parameter.cpp
    parameter/parameter.h
    parameter/parameter_compare.h
    parameter/parameter_infos.h
    parameter/parameter_wrapper.h
    )

set (REGISTRATION
	registration/registration.h
	registration/registration_bind.h
	registration/registration_executor.h
	registration/registration_impl.h
	registration/registration_manager.cpp
	registration/registration_manager.h
	)

set (COMMON
	CMakeLists.txt
	array_view.h
	core.h
	variant.h
	variant.cpp
	variant_impl.h
	)

source_group ("constructor" FILES ${CONSTRUCTOR})
source_group ("destructor" FILES ${DESTRUCTOR})
source_group ("method" FILES ${METHOD})
source_group ("property" FILES ${PROPERTY})
source_group ("class" FILES ${CLASS})
source_group ("argument" FILES ${ARGUMENT})
source_group ("parameter" FILES ${PARAMETER})
source_group ("registration" FILES ${REGISTRATION})
source_group ("" FILES ${COMMON})

set (SOURCE_FILES
	${CONSTRUCTOR}
	${DESTRUCTOR}
	${INSTANCE}
	${METHOD}
	${PROPERTY}
	${CLASS}
	${ARGUMENT}
	${PARAMETER}
	${REGISTRATION}
	${COMMON}
	)

# include floders 
include_directories (
		"${CMAKE_CURRENT_SOURCE_DIR}/."
		"${CMAKE_CURRENT_SOURCE_DIR}/.."
	)

# generate static lib 
add_library (${PROJECT_NAME} STATIC ${SOURCE_FILES})
set_property(TARGET reflection PROPERTY FOLDER LIBRARY)

message (STATUS "Configuring reflection succeed!")
	
