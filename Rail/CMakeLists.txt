cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(Rail VERSION 1.0 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_library(lib
    include/Simulation.h
    src/Simulation.cpp
    src/Train.cpp
    src/Train_func.cpp
    src/Stazione.cpp
    src/Binario.cpp 
    src/Binario.h
    src/Stazione.h
    src/Train.h
)

add_executable(sim
    test/main.cpp
)

target_include_directories(lib PRIVATE src include)
target_include_directories(sim PRIVATE src include)

target_link_libraries(sim lib)

file(
COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/line_description.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/timetables.txt
DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}
)
