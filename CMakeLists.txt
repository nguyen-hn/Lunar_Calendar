cmake_minimum_required(VERSION 3.20)
project(lunar LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(LUNAR_BUILD_TESTS "Build test" OFF)

add_subdirectory(src)
add_subdirectory(apps)

if(LUNAR_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
