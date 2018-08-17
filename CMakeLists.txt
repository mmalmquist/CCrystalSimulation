CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
ENABLE_LANGUAGE( C )
PROJECT( CCrystalSimulation )

SET( CMAKE_VERBOSE_MAKEFILE ON )
SET( CMAKE_C_COMPILER "gcc" )
ADD_DEFINITIONS( -std=gnu99 -Wall -Wextra -Wpedantic )

if ( CMAKE_BUILD_TYPE STREQUAL "Release" )
  ADD_DEFINITIONS(
    -O3
    -mtune=native
    -march=native
    )
endif ( CMAKE_BUILD_TYPE STREQUAL "Release" )
if ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
  ADD_DEFINITIONS(
    -Og
    -ggdb
    -Werror
    )
endif ( CMAKE_BUILD_TYPE STREQUAL "Debug" )

FILE( GLOB_RECURSE HDRS
  ${PROJECT_SOURCE_DIR}/src/*.h
  )
FILE( GLOB_RECURSE SRCS
  ${PROJECT_SOURCE_DIR}/src/*.c
  )

CONFIGURE_FILE( configuration/root_directory.h.in configuration/root_directory.h )
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR}/configuration )


# Find the GTK module using pkg-config
INCLUDE( FindPkgConfig )
PKG_CHECK_MODULES( GTK3 REQUIRED "gtk+-3.0" )

# Add the path to its header files to the compiler command line
INCLUDE_DIRECTORIES( ${GTK3_INCLUDE_DIRS} )
LINK_DIRECTORIES( ${GTK3_LIBRARY_DIRS} )

# Add any compiler flags it requires
ADD_DEFINITIONS( ${GTK3_CFLAGS_OTHER} )

# Add the makefile target for your executable and link in the GTK library
ADD_EXECUTABLE( ${CMAKE_PROJECT_NAME} ${HDRS} ${SRCS} )
TARGET_LINK_LIBRARIES( ${CMAKE_PROJECT_NAME}
  ${GTK3_LIBRARIES}
  pthread
  m
  )
