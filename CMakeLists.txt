cmake_minimum_required(VERSION 3.30)


############################
### Project Settings	####
############################

project(SQLighter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


############################
### Options				####
############################

option(BUILD_STATIC_LIBS	"Build libraries as static" OFF)
option(BUILD_FOR_COVER		"Build sqlight resources with the test executable for coverage" OFF)

#############################
### Variables ###############
#############################

set(SQLIGHTER_BIN "${CMAKE_BINARY_DIR}/bin")

file(GLOB_RECURSE SQLIGHTER_SOURCE
	"./Source/sqlighter/*.cpp"
)

file(GLOB_RECURSE SQLIGHTER_HEADERS
	"./Source/sqlighter/*.h"
)

############################
### Clean 	 ###############
############################

add_custom_target(purge
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/
    COMMENT "Cleaning the build directory"
)


############################
### Include Projects #######
############################

add_subdirectory(Source)