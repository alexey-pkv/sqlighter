find_package(SQLite3 CONFIG QUIET)


if(SQLite3_FOUND)
	
	message(STATUS "Found SQLite3: ${SQLite3_INCLUDE_DIRS}, ${SQLite3_LIBRARIES}")
	
else()
	
	message(STATUS "SQLite3 not found. Fetching 3.49")
	
	include(FetchContent)
	
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
	
	
	add_library(sqlite3 STATIC ${sqlite3_SOURCE_DIR}/sqlite3.c)
	target_include_directories(sqlite3 PUBLIC ${sqlite3_SOURCE_DIR})
endif()


set(SQLITE3_SOURCE_DIR ${sqlite3_SOURCE_DIR} CACHE PATH "Path to SQLite3 source directory")
