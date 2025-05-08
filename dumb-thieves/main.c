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
    const int house_number = atoi(argv[1]);
    const int fence_number = atoi(argv[2]);

    run_logic(house_number, fence_number);

    MPI_Finalize();
    return 0;
}