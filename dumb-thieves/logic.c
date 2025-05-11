#include "logic.h"

#include <mpi.h>
#include <pthread.h>

#include "communication.h"
#include "utils.h"
#include "process.h"

void run_logic(const int num_houses, const int num_fences) {
    int rank, num_processes;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    Process process;
    init_process(&process, rank);

    pthread_t thread;
    pthread_create(&thread, NULL, listener_thread, &process);

    srand(time(NULL) + rank);

    while (true) {
        // 1.
        process.house_ID = select_house(&process, num_houses);
        increment_clock_by_one(&process);

        printf("[P%d] (clock: %d) SELECTED house: %d\n", rank, process.lamport_clock, process.house_ID);

        // 2.
        increment_clock_by_one(&process); // each process needs to receive the same value! (that's why here, not in for loop in broadcast)
        process.last_req_clock = process.lamport_clock;
        Message req_house = {
            .type = MSG_REQ_HOUSE,
            .rank = rank,
            .lamport_clock = process.lamport_clock,
            .house_ID = process.house_ID
        };

        process.ack_count = 0;
        broadcast_message(&process, &req_house, num_processes);
        process.state = WAITING_FOR_HOUSE;

        // (N - 1) ACK
        wait_for_ack(&process, num_processes - 1);

        // 3.
        process.state = ROBBING_HOUSE;
        increment_clock_by_one(&process);
        printf("[P%d] (clock: %d) ENTERING house: %d\n", rank, process.lamport_clock, process.house_ID);
        sleep(rand() % 2 + 1);

        // 4.
        increment_clock_by_one(&process);
        process.last_req_clock = process.lamport_clock;
        Message req_fence = {
            .type = MSG_REQ_FENCE,
            .rank = rank,
            .lamport_clock = process.lamport_clock,
            .house_ID = -1
        };

        process.ack_count = 0;
        broadcast_message(&process, &req_fence, num_processes);
        process.state = WAITING_FOR_FENCE;

        // (N - P) ACK
        wait_for_ack(&process, num_processes - num_fences);

        // 5.
        process.state = HAS_FENCE;
        printf("[P%d] (clock: %d) USING fence \n", rank, process.lamport_clock);
        
        // 6.
        leave_critical_sections(&process);
        process.houses_visited_count++;
        printf("[P%d] (clock: %d) FINISHED job %d\n", rank, process.lamport_clock, process.houses_visited_count);
        
        // 7.
        process.state = RESTING;
        sleep(rand() % 3 + 1);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) printf("All processes completed their work\n");
}

void leave_critical_sections(Process* process) {
    Request req;

    increment_clock_by_one(process);

    // house queue
    while (!is_queue_empty(&process->house_queue)) {
        dequeue(&process->house_queue, &req);

        Message ack = {
            .type = MSG_ACK,
            .rank = process->rank,
            .lamport_clock = process->lamport_clock,
            .house_ID = -1
        };
        send_message(process, &ack, req.rank);
    }

    // fence queue
    while (!is_queue_empty(&process->fence_queue)) {
        dequeue(&process->fence_queue, &req);

        Message ack = {
            .type = MSG_ACK,
            .rank = process->rank,
            .lamport_clock = process->lamport_clock,
            .house_ID = -1
        };
        send_message(process, &ack, req.rank);
    }
}
