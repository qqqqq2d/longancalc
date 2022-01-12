cmake_minimum_required(VERSION 3.10)

project(ncmousew)

include_directories(source)
include_directories(/usr/include/ncurses)
set(CMAKE_CXX_FLAGS "-std=c++2a")
#set(CMAKE_CXX_FLAGS "-std=c++2b")
add_executable (ncmousew 
main.cpp
ncfunctions.cpp ncfunctions.h
calc_colors.h
        print_calc.cpp print_calc.h)
target_link_libraries(ncmousew m ncurses stdc++)
