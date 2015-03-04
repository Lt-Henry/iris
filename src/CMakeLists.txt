
#create an executable as target
add_executable(iris main.cpp Script.cpp Core.cpp MeshLoader.cpp Mesh.cpp Math.cpp Spectrum.cpp Color.cpp Camera.cpp Scene.cpp KdTree.cpp)

#require pkg-config support
find_package(PkgConfig REQUIRED)

#required pkg-config libs
pkg_check_modules(LUA REQUIRED lua)

#add include dirs
include_directories(${LUA_INCLUDE_DIRS})

#add library paths
link_directories(${LUA_LIBRARY_DIRS})

#add custom lib flags
add_definitions(${LUA_CFLAGS_OTHER} -D_IRIS_SSE_ -D_IRIS_SSE4_)

#set link libraries
target_link_libraries(iris ${LUA_LIBRARIES} freeimageplus pthread c++abi)

#set compiler
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

#compiler extra options
#set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -march=native -O3")
set(CMAKE_CXX_FLAGS "-std=c++11 -march=native -O3")



