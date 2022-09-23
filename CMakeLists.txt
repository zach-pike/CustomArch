cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 20)

project(CustomArch)

file(GLOB_RECURSE EMU_SRC "src/emulator/*.cpp")

add_executable(emulator ${EMU_SRC})

include_directories("src/")

target_link_libraries(emulator)