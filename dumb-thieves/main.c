#include "logic.h"
#include "utils.h"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    if (argc < 3) {
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        if (rank == 0) {
            fprintf(stderr, "Usage: %s <num_houses> <num_fences>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        #ifdef _WIN32
            if (access("logs", 0) == -1) {
                mkdir("logs", 0755);
            }
        #else
            struct stat st = {0};
            if (stat("logs", &st) == -1) {
                mkdir("logs", 0755);
            }
        #endif
    } else {
        sleep(1000);
    }
    
    char filename[64];
    snprintf(filename, sizeof(filename), "logs/log_%d.txt", rank);
    FILE* fp = freopen(filename, "w", stdout);
    if (!fp) {
        perror("freopen failed");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    setvbuf(stdout, NULL, _IOLBF, 0);

    int num_houses = atoi(argv[1]);
    int num_fences = atoi(argv[2]);

    run_logic(num_houses, num_fences);
    
    MPI_Finalize();
    return 0;
}