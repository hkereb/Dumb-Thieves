#include "logic.h"
#include "utils.h"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    if (argc < 3) {
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        if (rank == 0) {
            fprintf(stderr, "Usage: %s <num_houses> <num_pasers>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }
    
    int num_houses = atoi(argv[1]);
    int num_pasers = atoi(argv[2]);
    
    run_logic(num_houses, num_pasers);
    
    MPI_Finalize();
    return 0;
}