cmake_minimum_required(VERSION 3.29)


############################
### Project Settings	####
############################

project(SQLighter VERSION 0.3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


############################
### Options				####
############################

option(SQLIGHTER_BUILD_STATIC_LIBS	"Build libraries as static" OFF)
option(SQLIGHTER_BUILD_FOR_COVER	"Build sqlight resources with the test executable for coverage" OFF)


#############################
### Variables ###############
#############################

set(SQLIGHTER_BIN "${CMAKE_BINARY_DIR}/bin")

if (MSVC)
	# MSVC puts everything in a extra directory because reasons.
	set(SQLIGHTER_BIN_OUTPUT "${SQLIGHTER_BIN}/${CMAKE_CFG_INTDIR}")
else ()
	set(SQLIGHTER_BIN_OUTPUT "${SQLIGHTER_BIN}")
endif ()

message(STATUS "    Bin directory   : ${SQLIGHTER_BIN}")
message(STATUS "    Output directory: ${SQLIGHTER_BIN_OUTPUT}")

file(GLOB_RECURSE SQLIGHTER_SOURCE
	"${CMAKE_CURRENT_SOURCE_DIR}/src/sqlighter/*.cpp"
)

file(GLOB_RECURSE SQLIGHTER_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/sqlighter/*.h"
)


############################
### Include Projects #######
############################

add_subdirectory(src/cmake)
add_subdirectory(src/sqlighter)

if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
	add_subdirectory(src/tests)
endif ()