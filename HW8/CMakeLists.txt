cmake_minimum_required(VERSION 3.25)
project(HW8 C)

set(CMAKE_C_STANDARD 99)

include_directories(include)
link_directories(lib/x64)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

add_executable(HW8 main.c) #Write your project name
