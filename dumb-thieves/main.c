#include <mpi.h>
#include <stdio.h>
#include "logic.h"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    run_logic();

    MPI_Finalize();
    return 0;
}