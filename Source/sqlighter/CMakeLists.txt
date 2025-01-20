############################
### Define Version #########
############################

set(SQLIGHTER_VERSION_MAJOR 1)
set(SQLIGHTER_VERSION_MINOR 0)
set(SQLIGHTER_VERSION_PATCH 0)

set(SQLIGHTER_VERSION "${SQLIGHTER_VERSION_MAJOR}.${SQLIGHTER_VERSION_MINOR}.${SQLIGHTER_VERSION_PATCH}")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/sqlighter_version.h.in"
    "${SQLIGHTER_BIN}/sqlighter_version.h"
    @ONLY
)


############################
### Targets		 ###########
############################

if (NOT BUILD_FOR_COVER)
	
	if (BUILD_STATIC_LIBS)
		add_library(sqlighter STATIC ${SQLIGHTER_SOURCE})
	else ()
		add_library(sqlighter SHARED ${SQLIGHTER_SOURCE})
	endif ()
	
	set_target_properties(sqlighter PROPERTIES 
		OUTPUT_NAME "sqlighter"
		ARCHIVE_OUTPUT_DIRECTORY "${SQLIGHTER_BIN}"
		LIBRARY_OUTPUT_DIRECTORY "${SQLIGHTER_BIN}"
		RUNTIME_OUTPUT_DIRECTORY "${SQLIGHTER_BIN}")
	
	
	target_include_directories(sqlighter PRIVATE ${SQLIGHTER_BIN})
	target_include_directories(sqlighter PRIVATE ${CMAKE_SOURCE_DIR}/Source/sqlighter)

endif ()