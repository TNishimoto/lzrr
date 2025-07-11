cmake_minimum_required(VERSION 2.8)

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

#LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib)
if(NOT TARGET libdivsufsort)
include_directories(${CMAKE_BINARY_DIR}/module/libdivsufsort/include)
add_subdirectory(module/libdivsufsort)
endif()

INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(module)

#FILE(GLOB SOURCES module/libdivsufsort/sa.cpp)

#### Modules
#libdivsufsort directory is here.
#Please modify it appropriately.
#LINK_DIRECTORIES(~/lib)
#INCLUDE_DIRECTORIES(~/include)



add_executable(compress.out main/compress_main.cpp)
add_executable(decompress.out main/decompress_main.cpp)
#add_executable(rev.out rev_main.cpp)
add_executable(delta.out main/delta_main.cpp)


target_link_libraries(compress.out divsufsort divsufsort64)
target_link_libraries(decompress.out divsufsort divsufsort64)
#target_link_libraries(rev.out fn)

target_link_libraries(delta.out divsufsort divsufsort64)

