#include <mpi.h>
#include <stdio.h>
#include "communication.h"
#include "utils.h"

void send_message(int dest, int rank) {
    increment_clock();
    MPI_Send(&lamport_clock, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
    printf("[Process %d] Sent message to %d (local time = %d)\n", rank, dest, lamport_clock);
}

void receive_message(int rank) {
    MPI_Status status;
    int received_time;

    MPI_Recv(&received_time, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    update_clock(received_time);
    printf("[Process %d] Received message from %d (local time = %d)\n", rank, status.MPI_SOURCE, lamport_clock);
}
