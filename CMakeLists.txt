cmake_minimum_required(VERSION 3.15...4.2)

option( BUILD_SHARED_LIBS "Build using shared libraries" OFF )
option( RAST_BUILD_SAMPLES "Build samples for the rasterizer library" ON )

set( RAST_VERSION_MAJOR 0 )
set( RAST_VERSION_MINOR 0 )
set( RAST_VERSION_PATCH 1 )
set( RAST_VERSION ${RAST_VERSION_MAJOR}.${RAST_VERSION_MINOR}.${RAST_VERSION_PATCH})

set_property( GLOBAL PROPERTY USE_FOLDERS ON )

project( rast VERSION ${RAST_VERSION} LANGUAGES C CXX )

if(MSVC)
    add_compile_options(/MP)
endif(MSVC)

add_subdirectory( externals EXCLUDE_FROM_ALL )
# add_subdirectory( math )
# add_subdirectory( graphics )

if(RAST_BUILD_SAMPLES)
# add_subdirectory(samples)
endif(RAST_BUILD_SAMPLES)