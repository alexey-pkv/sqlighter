############################
### Targets		 ###########
############################

if (SQLIGHTER_BUILD_STATIC_LIBS)
	add_library(sqlighter STATIC ${SQLIGHTER_SOURCE})
	set_target_properties(sqlighter PROPERTIES POSITION_INDEPENDENT_CODE ON)
else ()
	add_library(sqlighter SHARED ${SQLIGHTER_SOURCE})
endif ()

set_target_properties(sqlighter PROPERTIES 
	OUTPUT_NAME "sqlighter"
	ARCHIVE_OUTPUT_DIRECTORY "${SQLIGHTER_BIN}"
	LIBRARY_OUTPUT_DIRECTORY "${SQLIGHTER_BIN}"
	RUNTIME_OUTPUT_DIRECTORY "${SQLIGHTER_BIN}")


target_include_directories(sqlighter PUBLIC ${SQLIGHTER_BIN})
target_include_directories(sqlighter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
	
if (SQLIGHTER_BUILD_FOR_COVER)
	target_compile_options(sqlighter PRIVATE -fprofile-arcs -ftest-coverage -g)
	target_link_options(sqlighter PRIVATE -fprofile-arcs -ftest-coverage -g)
endif ()