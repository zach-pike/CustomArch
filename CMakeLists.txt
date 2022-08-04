cmake_minimum_required(VERSION 3.16.3)

project(my_arch)

file(GLOB_RECURSE SRC "src/*.cpp")

add_executable(main ${SRC})

target_link_libraries(main dl)