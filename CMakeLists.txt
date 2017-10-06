cmake_minimum_required(VERSION 3.3.2)
project(crequests)

#set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/root")

option(BUILD_SHARED_LIBS
       "Global flag to cause add_library to create shared libraries if on." ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   set(CXXFLAGS "-Wall -Werror -Wextra -std=c++11 -O3 -Wold-style-cast -Wstrict-aliasing -Wshadow -pedantic-errors -Weffc++")
endif()

if (NOT CONFIGURED_ONCE)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXXFLAGS}"
       CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/crequests")
add_subdirectory("crequests")
add_subdirectory("test")

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")

