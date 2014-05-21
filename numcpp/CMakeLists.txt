
add_library(numcpp SHARED
    core/core.cpp
    base/base.cpp)
    # io/io.cpp
    # linalg/linalg.cpp)
target_link_libraries (numcpp)

#target_link_libraries (numcpp fftw3 fftw3f hdf5 lapack cairo)
