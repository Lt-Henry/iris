
#create an executable as target
add_library(iris SHARED Settings.cpp Spectrum.cpp Color.cpp Math.cpp 
Atmosphere.cpp Sensor.cpp BitMap.cpp)


#require pkg-config support
find_package(PkgConfig REQUIRED)


#lookup lua
pkg_check_modules(LUA lua)


if(NOT LUA_FOUND MATCHES 1)
	message("Using lua 5.2")
	pkg_check_modules(LUA lua5.2)
endif()

#lookup libpng
pkg_check_modules(PNG libpng)

target_include_directories (iris PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#add include dirs
include_directories(${LUA_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS})

#add library paths
link_directories(${LUA_LIBRARY_DIRS} ${PNG_LIBRARY_DIRS})

#add custom lib flags
#add_definitions(${LUA_CFLAGS_OTHER} -D_IRIS_SSE_ -D_IRIS_SSE4_)
add_definitions(${LUA_CFLAGS_OTHER})

#set link libraries
target_link_libraries(iris ${LUA_LIBRARIES} ${PNG_LIBRARIES})

#compiler extra options
set(CMAKE_CXX_FLAGS "-std=c++11 -march=native -O3")



