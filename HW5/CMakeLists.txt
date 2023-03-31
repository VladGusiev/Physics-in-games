cmake_minimum_required(VERSION 3.17)
project(HW5 C) #Write your project name

set(CMAKE_C_STANDARD 99)

include_directories(include)
link_directories(lib/x64)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

add_executable(HW5 main.c) #Write your project name

target_link_libraries(HW5 libfreeglut.a opengl32.dll libglu32.a) #Write your project name
