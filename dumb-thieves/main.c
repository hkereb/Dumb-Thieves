#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "logic.h"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <num_houses> <num_pasers>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }
    int num_houses = atoi(argv[1]);
    int num_processes = atoi(argv[2]);
    int num_pasers = atoi(argv[3]);

    run_logic(num_houses, num_pasers);

    MPI_Finalize();
    return 0;
}