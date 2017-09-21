cmake_minimum_required(VERSION 3.7)
project(untitled2)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES Simulator/simulator.c)
set(SOURCE_FILES_ASSEMBLY Assembly/assembler.c)
set(SOURCE_FILES_LINKER Linker/linker.c)
add_executable(simulator ${SOURCE_FILES})
add_executable(assembler ${SOURCE_FILES_ASSEMBLY})
add_executable(linker ${SOURCE_FILES_LINKER})