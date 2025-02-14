############################
### Targets		 ###########
############################

if (SQLIGHTER_BUILD_STATIC_LIBS)
	MESSAGE(STATUS "Building sqlighter as a static library")
	
	add_library(sqlighter STATIC ${SQLIGHTER_SOURCE})
	set_target_properties(sqlighter PROPERTIES POSITION_INDEPENDENT_CODE ON)
else ()
	MESSAGE(STATUS "Building sqlighter as a dynamic library")
	
	add_library(sqlighter SHARED ${SQLIGHTER_SOURCE})
endif ()

set_target_properties(sqlighter PROPERTIES 
	OUTPUT_NAME "sqlighter"
	ARCHIVE_OUTPUT_DIRECTORY "${SQLIGHTER_BIN}"
	LIBRARY_OUTPUT_DIRECTORY "${SQLIGHTER_BIN}"
	RUNTIME_OUTPUT_DIRECTORY "${SQLIGHTER_BIN}")

foreach(CONFIG_TYPE Debug Release RelWithDebInfo MinSizeRel)
	set_target_properties(sqlighter PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_TYPE} "${SQLIGHTER_BIN}"
		LIBRARY_OUTPUT_DIRECTORY_${CONFIG_TYPE} "${SQLIGHTER_BIN}"
		RUNTIME_OUTPUT_DIRECTORY_${CONFIG_TYPE} "${SQLIGHTER_BIN}"
	)
endforeach()


target_link_libraries(sqlighter sqlite3)


MESSAGE(STATUS "target_include_directories SQLITE3_SOURCE_DIR       : ${SQLITE3_SOURCE_DIR}")
MESSAGE(STATUS "target_include_directories SQLIGHTER_BIN            : ${SQLIGHTER_BIN}")
MESSAGE(STATUS "target_include_directories CMAKE_CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")

target_include_directories(sqlighter PUBLIC ${SQLITE3_SOURCE_DIR})
target_include_directories(sqlighter PUBLIC ${SQLIGHTER_BIN})
target_include_directories(sqlighter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
	
if (SQLIGHTER_BUILD_FOR_COVER)
	target_compile_options(sqlighter PRIVATE -fprofile-arcs -ftest-coverage -g)
	target_link_options(sqlighter PRIVATE -fprofile-arcs -ftest-coverage -g)
endif ()
