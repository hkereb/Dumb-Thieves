#include "logic.h"
#include "communication.h"
#include "utils.h"

void run_logic(int num_houses, int num_pasers) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    Process process;
    init_process(&process, rank);
    srand(time(NULL) + rank); 
    
    while (1) { 
        // 1.
        process.house_ID = select_house(&process, num_houses);
        increment_clock(&process);
        
        printf("[%d] Selected house %d (clock: %d)\n", rank, process.house_ID, process.lamport_clock);
        fflush(stdout);
        
        // 2.
        Message req_house = {
            .type = MSG_REQ_HOUSE,
            .rank = rank,
            .lamport_clock = process.lamport_clock,
            .house_ID = process.house_ID
        };
        
        broadcast_message(&process, &req_house, size);
        process.ack_count = 0;
        
        // (N - 1) ACK
        wait_for_acks(&process, size - 1, size);
        
        // 3.
        printf("[%d] ENTERING house %d\n", rank, process.house_ID);
        fflush(stdout);
        sleep(rand() % 2 + 1); 
        
        // 4.
        increment_clock(&process);
        Message req_paser = {
            .type = MSG_REQ_PASER,
            .rank = rank,
            .lamport_clock = process.lamport_clock,
            .house_ID = -1
        };
        
        broadcast_message(&process, &req_paser, size);
        process.ack_count = 0;
        
        // (N - P) ACK
        wait_for_acks(&process, size - num_pasers, size);
        
        // 5.
        printf("[%d] USING PASER (clock: %d)\n", rank, process.lamport_clock);
        fflush(stdout);
        sleep(1);
        
        // 6.
        leave_critical_sections(&process);
        
        process.houses_visited_count++;
        printf("[%d] Finished visit %d (clock: %d)\n", rank, process.houses_visited_count, process.lamport_clock);
        fflush(stdout);
        // 7.
        sleep(rand() % 3 + 1);
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        printf("All processes completed their work\n");
        fflush(stdout);
    }
}