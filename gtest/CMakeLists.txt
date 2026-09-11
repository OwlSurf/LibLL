cmake_minimum_required(VERSION 3.16)

# Standalone: cmake -S gtest -B gtest/build
# Or from repo root: cmake -S . -B build -DLIBLL_BUILD_TESTS=ON

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(LibLLTests LANGUAGES C CXX)
    add_library(LibLL STATIC ${CMAKE_CURRENT_SOURCE_DIR}/../LibLL.c)
    add_library(LibLL::LibLL ALIAS LibLL)
    target_include_directories(LibLL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.18.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(runtest test.cpp)
target_link_libraries(runtest PRIVATE LibLL::LibLL GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(runtest)
