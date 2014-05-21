project (NUMCPP CXX)

cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall" )
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g" )
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -msse2 -ffast-math -fomit-frame-pointer -fPIC -fstrict-aliasing -march=core2 -mtune=core2" ) # -fopenmp -ftree-vectorizer-verbose=5

#INCLUDE (CheckIncludeFiles)
# usage: CHECK_INCLUDE_FILES (<header> <RESULT_VARIABLE> )

#CHECK_INCLUDE_FILES (malloc.h HAVE_MALLOC_H)
#CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

#find_package (CLAPACK)
#if (CLAPACK_FOUND)
#  include_directories(${BZIP_INCLUDE_DIRS})
#  target_link_libraries (helloworld ${BZIP2_LIBRARIES})
#endif (CLAPACK_FOUND)

add_subdirectory (numcpp)
# add_subdirectory (examples)
# add_subdirectory (tests)
# add_subdirectory (performance)
