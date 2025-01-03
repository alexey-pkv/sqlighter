find_package(GTest REQUIRED)


### Configure variables and paths.
set(BIN_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin")
set(PROFILE_DIRECTORY "${CMAKE_SOURCE_DIR}/build/profile")


file(GLOB_RECURSE SQLIGHTER_TESTS_SOURCE
	"./*.cpp"
)

file(GLOB_RECURSE SQLIGHTER_SOURCE
	"./../sqlighter/*.cpp"
)


### Configure the test target.
add_executable(sqlighter_tests EXCLUDE_FROM_ALL ${SQLIGHTER_SOURCE} ${SQLIGHTER_TESTS_SOURCE})

set_target_properties(sqlighter_tests PROPERTIES 
	OUTPUT_NAME "sqlighter_tests"
	RUNTIME_OUTPUT_DIRECTORY ${BIN_DIRECTORY})


### Include Headers
target_include_directories(sqlighter_tests PRIVATE ${GTEST_INCLUDE_DIRS})
target_include_directories(sqlighter_tests PRIVATE ${CMAKE_SOURCE_DIR}/Source/sqlighter)
target_include_directories(sqlighter_tests PRIVATE ${CMAKE_SOURCE_DIR}/Source/tests/test_core)


### Link libraries
target_link_libraries(sqlighter_tests sqlite3)
target_link_libraries(sqlighter_tests ${GTEST_LIBRARIES} pthread)


### Add Coverage config
target_compile_options(sqlighter_tests PRIVATE -fprofile-arcs -ftest-coverage)
target_link_options(sqlighter_tests PRIVATE -fprofile-arcs -ftest-coverage)

