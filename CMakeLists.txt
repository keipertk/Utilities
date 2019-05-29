cmake_minimum_required(VERSION 3.8)
project(utilities VERSION 1.0.0)

find_package(cpp REQUIRED)
cpp_option(BUILD_TESTS ON)

add_subdirectory(utilities)

if(BUILD_TESTS)
    cpp_find_or_build_dependency(NAME Catch2 URL github.com/catchorg/Catch2)
    enable_testing()
    add_subdirectory(tests)
endif()
