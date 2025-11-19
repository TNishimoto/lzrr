cmake_minimum_required(VERSION 3.10)

project(lzrr CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CPP_STATIC)
set(CMAKE_CXX_FLAGS "-m64 -Wall -Wextra -static")
else(CPP_STATIC)
set(CMAKE_CXX_FLAGS "-m64 -Wall -Wextra")
endif(CPP_STATIC)

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    message("Release Mode")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_FLAGS_DEBUG  "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -DNDEBUG")

add_subdirectory(module/libsais)

#if(NOT TARGET libdivsufsort)
#include_directories(${CMAKE_BINARY_DIR}/module/libdivsufsort/include)
#add_subdirectory(module/libdivsufsort)
#endif()
INCLUDE_DIRECTORIES(module)

add_executable(compress.out main/compress_main.cpp)
add_executable(decompress.out main/decompress_main.cpp)
add_executable(delta.out main/delta_main.cpp)


target_link_libraries(compress.out libsais)
target_link_libraries(decompress.out libsais)
target_link_libraries(delta.out libsais)

