cmake_minimum_required(VERSION 3.7)
project(untitled2)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES Simulator/simulator.c Assembly/assembler.c)
set(SOURCE_FILES_ASSEMBLY Assembly/assembler.c)
add_executable(untitled2 ${SOURCE_FILES})
add_executable(assembly ${SOURCE_FILES_ASSEMBLY})