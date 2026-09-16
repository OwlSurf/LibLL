cmake_minimum_required(VERSION 3.16)

project(LibLL
    VERSION 0.1.0
    DESCRIPTION "Embedded circular doubly-linked list for C (intrusive)"
    LANGUAGES C
)

option(LIBLL_BUILD_TESTS "Build GoogleTest suite" OFF)
option(LIBLL_BUILD_EXAMPLES "Build example programs" ON)
option(LIBLL_INSTALL "Generate install targets" ON)

add_library(LibLL STATIC LibLL.c)
add_library(LibLL::LibLL ALIAS LibLL)

target_include_directories(LibLL
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(LibLL PROPERTIES
    OUTPUT_NAME ll
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER LibLL.h
)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(LibLL PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(LIBLL_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(LIBLL_BUILD_TESTS)
    enable_language(CXX)
    enable_testing()
    add_subdirectory(gtest)
endif()

if(LIBLL_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    install(TARGETS LibLL
        EXPORT LibLLTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(EXPORT LibLLTargets
        FILE LibLLTargets.cmake
        NAMESPACE LibLL::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LibLL
    )

    configure_package_config_file(
        cmake/LibLLConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/LibLLConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LibLL
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/LibLLConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/LibLLConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/LibLLConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LibLL
    )
endif()
