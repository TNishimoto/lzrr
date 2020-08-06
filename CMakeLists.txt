cmake_minimum_required(VERSION 2.8)

if(CPP_STATIC)
set(CMAKE_CXX_FLAGS "-std=c++14 -m64 -Wall -Wextra -static")
else(CPP_STATIC)
set(CMAKE_CXX_FLAGS "-std=c++14 -m64 -Wall -Wextra")
endif(CPP_STATIC)

set(CMAKE_CXX_FLAGS_DEBUG  "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -DNDEBUG")
INCLUDE_DIRECTORIES(src)
INCLUDE_DIRECTORIES(src/common)
INCLUDE_DIRECTORIES(src/debug)
INCLUDE_DIRECTORIES(src/parse)

LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib)
INCLUDE_DIRECTORIES(./include)

FILE(GLOB SOURCES src/*.cpp src/common/*.cpp src/parse/*.cpp src/debug/*.cpp)

#### Modules
if(USELIB)
#libdivsufsort directory is here.
#Please modify it appropriately.
LINK_DIRECTORIES(~/lib)
INCLUDE_DIRECTORIES(~/include)
else()
set(MYMOD_SOURCE_DIR ${CMAKE_SOURCE_DIR}/module)
set(MYMOD_BINARY_DIR ${CMAKE_BINARY_DIR}/module)
add_subdirectory(${MYMOD_SOURCE_DIR}/libdivsufsort ${MYMOD_BINARY_DIR}/libdivsufsort)
include_directories(${MYMOD_BINARY_DIR}/libdivsufsort/include)
endif()




#LINK_DIRECTORIES(~/lib)
#include_directories(~/include)

add_library(fn STATIC ${SOURCES})
target_link_libraries(fn divsufsort divsufsort64)

add_executable(compress.out compress_main.cpp)
add_executable(decompress.out decompress_main.cpp)

target_link_libraries(compress.out fn)
target_link_libraries(decompress.out fn)


