############################
### Targets		 ###########
############################

if (SQLIGHTER_BUILD_STATIC_LIBS)
	message(STATUS "Building sqlighter as a static library")
	
	add_library(sqlighter STATIC ${SQLIGHTER_SOURCE})
	set_target_properties(sqlighter PROPERTIES POSITION_INDEPENDENT_CODE ON)
else ()
	message(STATUS "Building sqlighter as a dynamic library")
	
	add_library(sqlighter SHARED ${SQLIGHTER_SOURCE})
endif ()

set_target_properties(sqlighter PROPERTIES 
	OUTPUT_NAME "sqlighter"
	ARCHIVE_OUTPUT_DIRECTORY "${SQLIGHTER_BIN}"
	LIBRARY_OUTPUT_DIRECTORY "${SQLIGHTER_BIN}"
	RUNTIME_OUTPUT_DIRECTORY "${SQLIGHTER_BIN}")


target_link_libraries(sqlighter sqlite3)

message(STATUS "    Directory SQLITE3_SOURCE_DIR       : ${SQLITE3_SOURCE_DIR}")
message(STATUS "    Directory SQLIGHTER_BIN            : ${SQLIGHTER_BIN}")
message(STATUS "    Directory CMAKE_CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")

target_include_directories(sqlighter PUBLIC ${SQLITE3_SOURCE_DIR})
target_include_directories(sqlighter PUBLIC ${SQLIGHTER_BIN})
target_include_directories(sqlighter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
	
if (SQLIGHTER_BUILD_FOR_COVER)
	target_compile_options(sqlighter PRIVATE -fprofile-arcs -ftest-coverage -g)
	target_link_options(sqlighter PRIVATE -fprofile-arcs -ftest-coverage -g)
endif ()
