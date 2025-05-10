cmake_minimum_required(VERSION 3.16)
project(dumb_thieves C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c17")

add_executable(dumb_thieves
        main.c
        logic.c
        communication.c
        utils.c
        process.c
        queue.c
)

target_include_directories(dumb_thieves PRIVATE ${PROJECT_SOURCE_DIR}/include)

find_package(MPI REQUIRED)
find_package(Threads REQUIRED)
target_link_libraries(dumb_thieves
        MPI::MPI_C
        Threads::Threads
)