cmake_minimum_required(VERSION 2.6)

project(uMatch)
set(uMatch_VERSION_MAJOR 1)
set(uMatch_VERSION_MINOR 0)
set(uMatch_VERSION_PATCH 0)

# get latest GIT hash from git describe
exec_program(
  "git"
  ${CMAKE_CURRENT_SOURCE_DIR}
  ARGS "describe --always"
  OUTPUT_VARIABLE uMatch_WC_REVISION
)

set(uMatch_VERSION_STRING
  "${uMatch_VERSION_MAJOR}.${uMatch_VERSION_MINOR}.${uMatch_VERSION_PATCH}")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
  message(STATUS "Setting Default Build Type : Debug ")
endif(NOT CMAKE_BUILD_TYPE)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${uMatch_SOURCE_DIR}/config.h.in"
  "${uMatch_SOURCE_DIR}/config.h"
  )

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(Boost 1.51 REQUIRED COMPONENTS
  system
  thread
  program_options
  )
include_directories(${Boost_INCLUDE_DIRS})

find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})

find_package(QUICKFIX REQUIRED)
include_directories(${QUICKFIX_INCLUDE_DIRS})

# find_package(SOCI REQUIRED)
# include_directories(${SOCI_INCLUDE_DIRS})

# find_package(SOCIPOSTGRESQL REQUIRED)
# include_directories(${SOCIPOSTGRESQL_INCLUDE_DIRS})

# with clang, disable the overloaded-virtual warnings from QuickFIX
IF(NOT WIN32)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wno-overloaded-virtual ")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wno-overloaded-virtual ")
ENDIF(NOT WIN32)

include_directories(${uMatch_SOURCE_DIR}/common)

set(EXECUTABLE_OUTPUT_PATH ${uMatch_SOURCE_DIR}/bin)

# add subdirectories to build

add_subdirectory(common)
add_subdirectory(esm)
