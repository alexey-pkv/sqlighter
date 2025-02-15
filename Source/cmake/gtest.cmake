include(FetchContent)


message(STATUS "Fetching GTest 1.16.0")


enable_testing()


FetchContent_Declare(
	googletest
	GIT_REPOSITORY	https://github.com/google/googletest.git
	GIT_TAG			v1.16.0
)

FetchContent_MakeAvailable(googletest)


set(GTEST_INCLUDE_DIR "${gtest_SOURCE_DIR}/include" CACHE PATH "Path to GTest include directory")






