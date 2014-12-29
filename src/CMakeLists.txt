
#create an executable as target
add_executable(iris main.cpp Core.cpp MeshLoader.cpp Mesh.cpp Math.cpp Spectrum.cpp Color.cpp)

#require pkg-config support
find_package(PkgConfig REQUIRED)

#required pkg-config libs
pkg_check_modules(GSL REQUIRED gsl)

#add include dirs
include_directories(${GSL_INCLUDE_DIRS})

#add library paths
link_directories(${GSL_LIBRARY_DIRS})

#add custom lib flags
add_definitions(${GSL_CFLAGS_OTHER} -D_IRIS_SSE_ -D_IRIS_SSE4_)

#set link libraries
target_link_libraries(iris ${GSL_LIBRARIES} freeimageplus pthread c++abi)

#set compiler
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

#compiler extra options
set(CMAKE_CXX_FLAGS "-std=c++0x -stdlib=libc++ -march=native")




