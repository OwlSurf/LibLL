cmake_minimum_required(VERSION 3.21.2)

project(TestLibLL)

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
	  googletest
	  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
	  )

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)  

include_directories(../)
add_library(LibLL STATIC ../LibLL.c)

enable_testing()
add_executable(
	  runtest
	  test.cpp
	  )
target_link_libraries(
	      runtest
	      LibLL
	      gtest_main
	      )

include(GoogleTest)
gtest_discover_tests(runtest)
