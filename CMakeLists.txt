cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_COMPILER "gcc-11")
set(CMAKE_CXX_COMPILER "/usr/bin/g++-11")

project(CustomArch)

file(GLOB_RECURSE EMU_SRC "src/emulator/*.cpp")
file(GLOB_RECURSE ASSEM_SRC "src/assembler/backend/*.cpp")

add_executable(emulator ${EMU_SRC})
add_executable(assembler ${ASSEM_SRC} "src/assembler/main.cpp")

include_directories("src/assembler/backend")
include_directories("src/")

target_link_libraries(emulator dl fmt)
target_link_libraries(assembler fmt)