include(FetchContent)


message(STATUS "Fetching SQLite 3.49.0")


FetchContent_Declare(
	sqlite3
	URL https://www.sqlite.org/2025/sqlite-amalgamation-3490000.zip
	URL_HASH SHA3_256=138b6922eb274c953c9d8f3249c39706ad3e40f3eef5d81bbeb73509b41f52c9
)

FetchContent_MakeAvailable(sqlite3)

if(EXISTS "${sqlite3_SOURCE_DIR}/sqlite3.h")
	message(STATUS "Found sqlite3.h at ${sqlite3_SOURCE_DIR}/sqlite3.h")
else()
	message(FATAL_ERROR "sqlite3.h not found in ${sqlite3_SOURCE_DIR}")
endif()


if (APPLE)
	add_library(sqlite3 SHARED ${sqlite3_SOURCE_DIR}/sqlite3.c)
	target_link_options(sqlite3 PRIVATE "-undefined" "dynamic_lookup")
elseif (UNIX)
	add_library(sqlite3 SHARED ${sqlite3_SOURCE_DIR}/sqlite3.c)
	target_link_libraries(sqlite3 PRIVATE dl)
else () 
	add_library(sqlite3 STATIC ${sqlite3_SOURCE_DIR}/sqlite3.c)
endif ()

target_include_directories(sqlite3 PUBLIC ${sqlite3_SOURCE_DIR})


set(SQLITE3_SOURCE_DIR ${sqlite3_SOURCE_DIR} CACHE PATH "Path to SQLite3 source directory")
