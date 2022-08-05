cmake_minimum_required(VERSION 3.16.3)

project(CustomArch)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE EMU_SRC "src/emulator/*.cpp")
file(GLOB_RECURSE ASSEM_SRC "src/assembler/*.cpp")

add_executable(emulator ${EMU_SRC})
add_executable(assembler ${ASSEM_SRC})

include_directories("src/assembler")

target_link_libraries(emulator dl)
target_link_libraries(assembler fmt)