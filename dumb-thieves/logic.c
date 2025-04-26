#include <mpi.h>
#include <stdio.h>
#include "logic.h"
#include "communication.h"
#include "utils.h"

void run_logic() {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Process 0 sends a message to everyone
        for (int i = 1; i < size; i++) {
            send_message(i, rank);
        }
    } else {
        // others receive it
        receive_message(rank);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        printf("[Process %d] All messages have been sent and received\n", rank);
    }
}