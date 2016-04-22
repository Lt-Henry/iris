
#create an executable as target
add_executable(cli main.cpp)


#set link libraries
target_link_libraries(cli iris)

#set compiler
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

#compiler extra options
set(CMAKE_CXX_FLAGS "-std=c++11 -march=native -O3")
#set(CMAKE_CXX_FLAGS "-std=c++11 -march=native -O3")

