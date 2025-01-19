############################
### Options				####
############################

option(BUILD_STATIC_LIBS "Build libraries as static" OFF)

############################
### Build Directory ########
############################

set(SQLIGHTER_BIN_DIR "${CMAKE_SOURCE_DIR}/build/bin")

include_directories("${SQLIGHTER_BIN_DIR}")

############################
### Include Projects #######
############################

add_subdirectory(sqlighter)
add_subdirectory(tests)