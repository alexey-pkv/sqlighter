cmake_minimum_required(VERSION 3.30)


project(SQLighter)


set(CMAKE_CXX_STANDARD 20)


add_subdirectory(Source)


### Clean 

add_custom_target(clean_build
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*
    COMMENT "Cleaning the build directory"
)


### Coverage 

add_custom_target(run_coverage
    COMMAND ${CMAKE_COMMAND} -E echo "Running coverage script..."
    COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/test_cover.sh
    DEPENDS sqlighter_tests
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running coverage collection and report generation"
)

add_dependencies(run_coverage sqlighter_tests)