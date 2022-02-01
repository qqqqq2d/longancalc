cmake_minimum_required(VERSION 3.10)

project(ncmousew)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../src")
include_directories(/usr/include/ncurses)
set(CMAKE_CXX_FLAGS "-std=c++2a")
#set(CMAKE_CXX_FLAGS "-std=c++2b")
add_executable (ncmousew 
main.cpp
ncfunctions.cpp ncfunctions.h
calc_colors.h
        print_calc.cpp print_calc.h ../src/interface.h computer.cpp computer.h button.h ../src/init_result.h ../src/calculator.h ../src/constants.h)
target_link_libraries(ncmousew m ncurses stdc++)
