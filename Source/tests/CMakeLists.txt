########################################
### Target: sqlighter_tests	 ###########
########################################

add_executable(sqlighter_tests EXCLUDE_FROM_ALL)


file(GLOB_RECURSE SQLIGHTER_TESTS_SOURCE
	"./*.cpp"
)

target_sources(sqlighter_tests PRIVATE ${SQLIGHTER_TESTS_SOURCE})

message(STATUS "Path ${SQLIGHTER_BIN}")

set_target_properties(sqlighter_tests PROPERTIES
	OUTPUT_NAME "sqlighter_tests"
	ARCHIVE_OUTPUT_DIRECTORY "${SQLIGHTER_BIN}"
	LIBRARY_OUTPUT_DIRECTORY "${SQLIGHTER_BIN}"
	RUNTIME_OUTPUT_DIRECTORY "${SQLIGHTER_BIN}")

add_dependencies(sqlighter_tests sqlighter)

if (SQLIGHTER_BUILD_FOR_COVER)
	target_compile_options(sqlighter_tests PRIVATE -fprofile-arcs -ftest-coverage -g)
	target_link_options(sqlighter_tests PRIVATE -fprofile-arcs -ftest-coverage -g)
endif ()

### Include Headers

message(STATUS "target_include_directories GTEST_INCLUDE_DIR: ${GTEST_INCLUDE_DIR}")
message(STATUS "target_include_directories CMAKE_SOURCE_DIR:  ${CMAKE_SOURCE_DIR}")
message(STATUS "target_include_directories SQLIGHTER_BIN:     ${SQLIGHTER_BIN}")

target_include_directories(sqlighter_tests PRIVATE ${GTEST_INCLUDE_DIR})
target_include_directories(sqlighter_tests PRIVATE ${CMAKE_SOURCE_DIR}/Source/tests/test_core)
target_include_directories(sqlighter_tests PRIVATE ${SQLIGHTER_BIN})

target_link_libraries(sqlighter_tests sqlighter)
target_link_libraries(sqlighter_tests sqlite3)
target_link_libraries(sqlighter_tests gtest gtest_main pthread)


############################
### Script: Coverage	####
############################

if (SQLIGHTER_BUILD_FOR_COVER)

	add_custom_target(cover
		COMMAND ${CMAKE_COMMAND} -E echo "Running coverage script..."
		COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/linux/test_cover.sh ${CMAKE_BINARY_DIR}
		DEPENDS sqlighter_tests
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Running coverage collection and report generation"
	)
	
	add_dependencies(cover sqlighter_tests)
	
endif ()

############################
### Script: Unittests	####
############################

if (CYGWIN)
	set(TEST_COMMAND ${CMAKE_SOURCE_DIR}/scripts/win/test.bat ${CMAKE_BINARY_DIR})
elseif(UNIX)
	set(TEST_COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/linux/test.sh ${CMAKE_BINARY_DIR})
else ()
	message(FATAL_ERROR "System not supported")
endif()

message(STATUS ${CMAKE_BINARY_DIR})

add_custom_target(test
	COMMAND ${CMAKE_COMMAND} -E echo "Running tests..."
	COMMAND ${TEST_COMMAND}
	DEPENDS sqlighter_tests
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMENT "Running unit tests"
)

add_dependencies(test sqlighter_tests)

####################################
### Script: Test for Leaks		####
####################################

add_custom_target(leak_check
	COMMAND ${CMAKE_COMMAND} -E echo "Checking for memory leaks..."
	COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/linux/leak_check.sh ${CMAKE_BINARY_DIR}
	DEPENDS sqlighter_tests
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMENT "Check for memory leaks by running unit tests in valgrind"
)

add_dependencies(leak_check sqlighter_tests)