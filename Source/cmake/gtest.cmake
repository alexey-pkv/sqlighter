include(FetchContent)


enable_testing()


find_package(GTest CONFIG QUIET)


if(GTest_FOUND)
	
	message(STATUS "Found GTest")

else()
	
	message(STATUS "GTest not found. Fetching 1.16.0")
	
	include(FetchContent)
	
	FetchContent_Declare(
		googletest
		GIT_REPOSITORY	https://github.com/google/googletest.git
		GIT_TAG			v1.16.0
	)
	
	FetchContent_MakeAvailable(googletest)
endif()


set(GTEST_INCLUDE_DIR "${gtest_SOURCE_DIR}/include" CACHE PATH "Path to GTest include directory")






