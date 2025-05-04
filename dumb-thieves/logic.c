#include <mpi.h>
#include <stdio.h>
#include "logic.h"
#include "communication.h"
#include "utils.h"

void run_logic(int num_houses, int num_pasers) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Process process = {
        .rank = rank,
        .state = 0,
        .lamport_clock = 0,
        .ack_count = 0,
        .houses_visited_count = 0
    };
    init_queue(&process.house_queue);
    init_queue(&process.paser_queue);

    while(1) {
        process.house_ID = select_house(&process, num_houses);
        increment_clock(&process.lamport_clock);

        Message req_house = {
            .type = MSG_REQ_HOUSE,
            .rank = rank,
            .lamport_clock = process.lamport_clock,
            .house_ID = process.house_ID
        };

        broadcast_message(&req_house, size);
        process.ack_count = 0;

        wait_for_acks(&process, size - 1); // house acks

        printf("Process %d entering house %d\n", rank, process.house_ID);
        sleep(rand() % 2 + 1);

        increment_clock(&process.lamport_clock);

        Message req_paser = {
            .type = MSG_REQ_PASER,
            .rank = rank,
            .lamport_clock = process.lamport_clock,
            .house_ID = -1
        };

        broadcast_message(&req_paser, size);
        process.ack_count = 0;

        wait_for_acks(&process, size - num_pasers);

        printf("Process %d accessing paser\n", rank);

        leave_pasers(&process);

        process.houses_visited_count++;

        sleep(rand() % 5 + 1);
    }

    MPI_Barrier(MPI_COMM_WORLD);
}
