############################
### Options				####
############################

option(BUILD_FOR_COVER "Build sqlight resources with the test executable for coverage" OFF)

############################
### Variables			####
############################

set(PROFILE_DIRECTORY "${CMAKE_SOURCE_DIR}/build/profile")

############################
### Packages			####
############################

find_package(GTest REQUIRED)

############################



### Configure variables and paths.


file(GLOB_RECURSE SQLIGHTER_TESTS_SOURCE
	"./*.cpp"
)

if (BUILD_FOR_COVER)
	file(GLOB_RECURSE SQLIGHTER_SOURCE
		"./../sqlighter/*.cpp"
	)
endif ()


### Configure the test target.
add_executable(sqlighter_tests EXCLUDE_FROM_ALL ${SQLIGHTER_TESTS_SOURCE})

if (BUILD_FOR_COVER)
	target_sources(sqlighter_tests PRIVATE ${SQLIGHTER_SOURCE})
endif ()

set_target_properties(sqlighter_tests PROPERTIES 
	OUTPUT_NAME "sqlighter_tests"
	RUNTIME_OUTPUT_DIRECTORY ${SQLIGHTER_BIN_DIR})

### Include Headers
target_include_directories(sqlighter_tests PRIVATE ${GTEST_INCLUDE_DIRS})
target_include_directories(sqlighter_tests PRIVATE ${CMAKE_SOURCE_DIR}/Source/sqlighter)
target_include_directories(sqlighter_tests PRIVATE ${CMAKE_SOURCE_DIR}/Source/tests/test_core)


### Link libraries
if (NOT BUILD_FOR_COVER)
	add_dependencies(sqlighter_tests sqlighter)
	target_link_libraries(sqlighter_tests sqlighter)
	
	message(STATUS "Not for cover")
else ()
	message(STATUS "For cover")
endif ()

target_link_libraries(sqlighter_tests sqlite3)
target_link_libraries(sqlighter_tests ${GTEST_LIBRARIES} pthread)


############################
### Coverage			####
############################

if (BUILD_FOR_COVER)
	
	target_compile_options(sqlighter_tests PRIVATE -fprofile-arcs -ftest-coverage)
	target_link_options(sqlighter_tests PRIVATE -fprofile-arcs -ftest-coverage)
	
	add_custom_target(cover
		COMMAND ${CMAKE_COMMAND} -E echo "Running coverage script..."
		COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/test_cover.sh
		DEPENDS sqlighter_tests
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Running coverage collection and report generation"
	)
	
	add_dependencies(cover sqlighter_tests)
	
endif ()

############################
### Unittests			####
############################

add_custom_target(test
	COMMAND ${CMAKE_COMMAND} -E echo "Running tests..."
	COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/test.sh
	DEPENDS sqlighter_tests
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMENT "Running unit tests"
)

add_dependencies(test sqlighter_tests)

############################
### Test for Leaks		####
############################

add_custom_target(leak_check
	COMMAND ${CMAKE_COMMAND} -E echo "Checking for memory leaks..."
	COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/leak_check.sh
	DEPENDS sqlighter_tests
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMENT "Check for memory leaks by running unit tests in valgrind"
)

add_dependencies(leak_check sqlighter_tests)