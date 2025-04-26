#include <mpi.h>
#include <stdio.h>
#include "communication.h"
#include "utils.h"

void send_message(int dest, int rank) {
    increment_clock();
    MPI_Send(&lamport_clock, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
    printf("[Proces %d] Wysłał wiadomość do %d (czas lokalny = %d)\n", rank, dest, lamport_clock);
}

void receive_message(int rank) {
    MPI_Status status;
    int received_time;

    MPI_Recv(&received_time, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    update_clock(received_time);
    printf("[Proces %d] Otrzymał wiadomość od %d (czas lokalny = %d)\n", rank, status.MPI_SOURCE, lamport_clock);
}
