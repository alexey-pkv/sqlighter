file(GLOB_RECURSE SQLIGHTER_SOURCE
	"./*.cpp"
)


add_library(sqlighter SHARED ${SQLIGHTER_SOURCE})

set_target_properties(sqlighter PROPERTIES 
	OUTPUT_NAME "sqlighter"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin")


target_include_directories(sqlighter PRIVATE ${CMAKE_SOURCE_DIR}/Source/sqlighter)