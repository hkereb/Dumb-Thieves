#include <mpi.h>
#include <stdio.h>
#include "logic.h"

#include <stdlib.h>
#include <unistd.h>

#include "communication.h"
#include "utils.h"

void run_logic(const int house_number, const int fence_number) {
    printf("[DEBUG] house_number = %d, fence_number = %d\n", house_number, fence_number);

    int rank, size = 0; // todo find out why size ALWAYS 0!
    printf("[DEBUG] size = %d\n", size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Process process = {
    //     .rank = rank,
    //     .state = RESTING,
    //     .lamport_clock = 0,
    //     .ack_count = 0,
    //     .houses_visited_count = 0
    // };
    // init_queue(&process.house_queue);
    // init_queue(&process.fence_queue);

    Process process;
    init_process(&process, rank, size, fence_number, house_number);

    while(true) {
        process.house_ID = select_house(process);
        increment_clock(&process.lamport_clock);

        Message req_house = {
            .type = MSG_REQ_HOUSE,
            .rank = rank,
            .lamport_clock = process.lamport_clock,
            .house_ID = process.house_ID
        };

        broadcast_message(&req_house, size);
        increment_clock(&process.lamport_clock);
        process.ack_count = 0;
        printf("[Process %d] SENT REQ_HOUSE for house %d\n", rank, process.house_ID);

        wait_for_acks(&process, size - 1); // house acks

        printf("[Process %d] ENTERED HOUSE %d\n", rank, process.house_ID);
        sleep(rand() % 2 + 1);

        // increment_clock(&process.lamport_clock);
        //
        // Message req_paser = {
        //     .type = MSG_REQ_PASER,
        //     .rank = rank,
        //     .lamport_clock = process.lamport_clock,
        //     .house_ID = -1
        // };
        //
        // broadcast_message(&req_paser, size);
        // process.ack_count = 0;
        //
        // wait_for_acks(&process, size - num_fences);
        //
        // printf("Process %d accessing paser\n", rank);
        //
        // leave_pasers(&process);

        process.houses_visited_count++;

        sleep(rand() % 5 + 1);
    }

    MPI_Barrier(MPI_COMM_WORLD);
}
