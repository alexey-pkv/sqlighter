message(STATUS "Building SQLighter version << ${SQLighter_VERSION} >>")


configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/../sqlighter/sqlighter_version.h.in"
    "${SQLIGHTER_BIN}/sqlighter_version.h"
    @ONLY
)
