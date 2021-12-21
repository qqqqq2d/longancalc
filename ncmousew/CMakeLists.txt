cmake_minimum_required(VERSION 3.10)

project(ncmousew)

include_directories(source)
include_directories(/usr/include/ncurses)
set(CMAKE_CXX_FLAGS "-std=c++2a")
add_executable (ncmousew main.cpp)
target_link_libraries(ncmousew m ncurses stdc++)
