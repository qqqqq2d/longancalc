cmake_minimum_required(VERSION 3.10)

project(calc)


include_directories(source)

set(LIBS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs")


IF(UNIX)
    set(CMAKE_CXX_FLAGS "-std=c++17")

    link_directories("${LIBS_PATH}/unix/pdcurses/")
ELSE (WIN32)
    set(CMAKE_CXX_FLAGS "/std:c++latest")

    include_directories("${LIBS_PATH}/include")
    include_directories("${LIBS_PATH}/include/pdcurses")

    link_directories("${LIBS_PATH}/windows/pdcurses/")
ENDIF (UNIX)

add_executable (calc main.cpp)
    
IF(UNIX)
    target_link_libraries(calc m ncurses)
ELSE (WIN32)
    target_link_libraries(calc pdcurses.lib)
ENDIF (UNIX)

set(CMAKE_BUILD_TYPE Debug)
