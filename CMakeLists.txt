cmake_minimum_required(VERSION 3.30)


project(SQLighter)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(Source)


### Clean 

add_custom_target(purge
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*
    COMMENT "Cleaning the build directory"
)


