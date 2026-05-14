cmake_minimum_required(VERSION 3.11)

project(Ponca-ATime)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( PONCA_CONFIGURE_EXAMPLES CACHE BOOL OFF)
set( PONCA_CONFIGURE_TESTS    CACHE BOOL OFF)
set( PONCA_CONFIGURE_DOC      CACHE BOOL OFF)
add_subdirectory(external/ponca)

add_library(poncatime SHARED
        export.h
        curvatureEstimation.h
        curvatureEstimation.cpp)
target_link_libraries(poncatime PUBLIC Ponca::Fitting Ponca::SpatialPartitioning)
target_include_directories(poncatime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Running with local eigen: EIGEN3_INCLUDE_DIRS is computed by Ponca
if(NOT Eigen3_FOUND)
    target_include_directories(poncatime PUBLIC ${EIGEN3_INCLUDE_DIRS})
endif()

add_executable(poncatime-test cpp/main.cpp)
target_link_libraries(poncatime-test PUBLIC poncatime)
target_include_directories(poncatime-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

