cmake_minimum_required(VERSION 3.16.3)

project(CustomArch)

file(GLOB_RECURSE EMU_SRC "src/emulator/*.cpp")
file(GLOB_RECURSE ASSEM_SRC "src/assembler/*.cpp")

add_executable(emulator ${EMU_SRC})
add_executable(assembler ${ASSEM_SRC})

target_link_libraries(emulator dl)