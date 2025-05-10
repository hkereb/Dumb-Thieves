#include "logic.h"
#include "communication.h"
#include "utils.h"

void run_logic(const int num_houses, const int num_fences) {
    int rank, num_processes;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    Process process;
    init_process(&process, rank);

    srand(time(NULL) + rank);

    while (true) {
        // 1.
        process.house_ID = select_house(&process, num_houses);
        increment_clock(&process);

        printf("[P%d] SELECTED house: %d (clock: %d)\n", rank, process.house_ID, process.lamport_clock);

        // 2.
        increment_clock(&process); // each process needs to receive the same value! (that's why here, not in for loop in broadcast)
        Message req_house = {
            .type = MSG_REQ_HOUSE,
            .rank = rank,
            .lamport_clock = process.lamport_clock,
            .house_ID = process.house_ID
        };

        process.ack_count = 0;
        broadcast_message(&process, &req_house, num_processes);

        // (N - 1) ACK
        wait_for_acks(&process, num_processes - 1, num_processes);

        // 3.
        printf("[P%d] ENTERING house: %d (clock: %d)\n", rank, process.house_ID, process.lamport_clock);
        sleep(rand() % 2 + 1);

        // 4.
        increment_clock(&process);
        Message req_paser = {
            .type = MSG_REQ_FENCE,
            .rank = rank,
            .lamport_clock = process.lamport_clock,
            .house_ID = -1
        };

        process.ack_count = 0;
        broadcast_message(&process, &req_paser, num_processes);

        // (N - P) ACK
        wait_for_acks(&process, num_processes - num_fences, num_processes);
        
        // 5.
        printf("[P%d] USING fence (clock: %d)\n", rank, process.lamport_clock);
        sleep(1);
        
        // 6.
        leave_critical_sections(&process);
        
        process.houses_visited_count++;
        printf("[P%d] FINISHED job %d (clock: %d)\n", rank, process.houses_visited_count, process.lamport_clock);
        
        // 7.
        sleep(rand() % 3 + 1);
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) printf("All processes completed their work\n");
}