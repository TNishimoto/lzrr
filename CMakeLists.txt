cmake_minimum_required(VERSION 2.8)
project(lzrr CXX)

set(CMAKE_CXX_FLAGS "-std=c++14 -fexceptions -m64")
#set(CMAKE_CXX_FLAGS_DEBUG  "-g -O0 -DDEBUG -DSLOWDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG  "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -DNDEBUG")


if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    message("Release Mode")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

# Set PROJECT_IS_TOP_LEVEL to ON if if this is the top level project (not
# if this is included as a subproject of something else). Note that this is
# handled automatically for CMake >= 3.21.
if (CMAKE_VERSION VERSION_LESS 3.21)
    if ("${CMAKE_PROJECT_NAME}" STREQUAL "${PROJECT_NAME}")
        message("SET PROJECT_IS_TOP_LEVEL ON IN ${PROJECT_NAME}")
        set (PROJECT_IS_TOP_LEVEL ON)
    else()
        message("SET PROJECT_IS_TOP_LEVEL OFF IN ${PROJECT_NAME}")
    set (PROJECT_IS_TOP_LEVEL OFF)    
    endif()
endif ()


set(CMAKE_CXX_FLAGS_DEBUG  "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -DNDEBUG")
#INCLUDE_DIRECTORIES(src)
#INCLUDE_DIRECTORIES(src/common)
#INCLUDE_DIRECTORIES(src/debug)
#INCLUDE_DIRECTORIES(src/parse)

LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib)
INCLUDE_DIRECTORIES(./include)

FILE(GLOB SOURCES src/*.cpp src/common/*.cpp src/parse/*.cpp src/debug/*.cpp)

#### Modules
#if(USELIB)
#libdivsufsort directory is here.
#Please modify it appropriately.
LINK_DIRECTORIES(~/lib)
INCLUDE_DIRECTORIES(~/include)
#endif()


# For stool
if(NOT TARGET stool)
add_subdirectory(module/stool)
endif()


#####

INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(module)
FILE(GLOB SOURCES lib/*.cpp lib/common/*.cpp lib/parse/*.cpp lib/debug/*.cpp)
FILE(GLOB LIBDIVSUFOSRT module/libdivsufsort/sa.cpp)


if (PROJECT_IS_TOP_LEVEL)
add_library(fn STATIC ${SOURCES} ${LIBDIVSUFOSRT})
target_link_libraries(fn divsufsort divsufsort64 stool)

add_executable(compress.out compress_main.cpp)
add_executable(decompress.out decompress_main.cpp)
add_executable(rev.out rev_main.cpp)


target_link_libraries(compress.out fn)
target_link_libraries(decompress.out fn)
target_link_libraries(rev.out fn)
endif()


