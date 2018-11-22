cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)
project( Interlace )

find_package( OpenCV REQUIRED )

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

file( GLOB SOURCES ./src/*.h ./src/*.cpp )

add_executable( Interlace ${SOURCES} )
target_link_libraries( Interlace ${Boost_LIBRARIES} ${OpenCV_LIBS} )
