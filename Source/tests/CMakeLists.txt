find_package(GTest REQUIRED)


file(GLOB_RECURSE SQLIGHTER_TESTS_SOURCE
	"./*.cpp"
)

file(GLOB_RECURSE SQLIGHTER_SOURCE
	"./../sqlighter/*.cpp"
)



add_executable(sqlighter_tests EXCLUDE_FROM_ALL ${SQLIGHTER_SOURCE} ${SQLIGHTER_TESTS_SOURCE})


set_target_properties(sqlighter_tests PROPERTIES 
	OUTPUT_NAME "sqlighter_tests"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin")

target_include_directories(sqlighter_tests PRIVATE ${GTEST_INCLUDE_DIRS})
target_include_directories(sqlighter_tests PRIVATE ${CMAKE_SOURCE_DIR}/Source/sqlighter)


target_link_libraries(sqlighter_tests sqlite3)
target_link_libraries(sqlighter_tests ${GTEST_LIBRARIES} pthread)


