cmake_minimum_required(VERSION 3.16)
project(dumb_thieves C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c17")

add_executable(dumb_thieves
        main.c
        logic.c
        communication.c
        utils.c
)

target_include_directories(dumb_thieves PRIVATE include)

find_package(MPI REQUIRED)
target_link_libraries(dumb_thieves MPI::MPI_C)