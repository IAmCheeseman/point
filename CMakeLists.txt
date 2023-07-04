cmake_minimum_required(VERSION 3.26)

set(CMAKE_C_FLAGS "-fsanitize=address -Wall")

project(point)
file(GLOB_RECURSE SRC src/*.c)
add_executable(point ${SRC})
target_link_libraries(point PUBLIC m SDL2 SDL2_image SDL2_ttf lua)