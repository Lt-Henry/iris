
#minimum cmake version
cmake_minimum_required (VERSION 2.6)

#project name
project (Iris)

#create an executable as target
add_executable(iris main.cpp Core.cpp)

#require pkg-config support
find_package(PkgConfig REQUIRED)

#required pkg-config libs
pkg_check_modules(LUA REQUIRED lua5.1)
pkg_check_modules(GSL REQUIRED gsl)

#add include dirs
include_directories(${LUA_INCLUDE_DIRS} ${GSL_INCLUDE_DIRS})

#add library paths
link_directories(${LUA_LIBRARY_DIRS} ${GSL_LIBRARY_DIRS})

#add custom lib flags
add_definitions(${LUA_CFLAGS_OTHER} ${GSL_CFLAGS_OTHER})

#set link libraries
target_link_libraries(iris ${LUA_LIBRARIES} ${GSL_LIBRARIES})

#gcc extra options
set(CMAKE_CXX_FLAGS "-std=c++0x")
