cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")

PROJECT(dlambda)

INCLUDE (CheckIncludeFiles)
include (FindPkgConfig)
include(CheckCXXSourceCompiles)

if (CMAKE_CXX_COMPILER MATCHES ".*clang")
  set(CMAKE_COMPILER_IS_CLANGXX 1)
endif ()

# build type
set(CMAKE_BUILD_TYPE Debug)

#if you don't want the full compiler output, remove the following line
set(CMAKE_VERBOSE_MAKEFILE ON)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.52.0 COMPONENTS thread filesystem chrono system REQUIRED )
if (NOT Boost_FOUND)
  message(SEND_ERROR "Boost not found")
endif (NOT Boost_FOUND)

find_package( LLVM )
if (NOT LLVM_FOUND)
  message(SEND_ERROR "LLVM not found." )
endif (NOT LLVM_FOUND)

# compile option (Debug) 
if (CMAKE_COMPILER_IS_CLANGXX)
  set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Wno-unused-parameter -Werror -std=c++11 ${LLVM_CFLAGS}")
  set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Wextra -Wno-unused-parameter -Werror")
else()
  set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Wno-array-bounds -Werror -std=c++11 ${LLVM_CFLAGS}")
  set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Wextra -Wno-array-bounds -Werror")
endif()

# compile option (Release)
if (CMAKE_COMPILER_IS_CLANGXX)
  set(CMAKE_CXX_FLAGS_RELEASE "-W -Wall -Wextra -Wno-unused-parameter -Werror -std=c++11 -O3 ${LLVM_CFLAGS}")
  set(CMAKE_C_FLAGS_RELEASE "-W -Wall -Wextra -Wno-unused-parameter -Werror -O3")
else()
  set(CMAKE_CXX_FLAGS_RELEASE "-W -Wall -Wextra -Wno-array-bounds -Werror -std=c++11 -O3 ${LLVM_CFLAGS}")
  set(CMAKE_C_FLAGS_RELEASE "-W -Wall -Wextra -Wno-array-bounds -Werror -O3")
endif()


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/include ${Boost_INCLUDE_DIRS} ${LLVM_INCLUDE_DIR} )
link_directories( ${Boost_LIBRARY_DIRS} ${LLVM_LIBRARY_DIR} )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lz -lpthread -lffi -ldl -lm -lncurses" )
subdirs( include src )

