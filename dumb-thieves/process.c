#include "utils.h"
#include "process.h"

void init_process(Process* process, int rank) {
    process->rank = rank;
    process->state = RESTING;
    process->lamport_clock = 0;
    process->last_req_clock = 0;
    process->house_ID = -1;
    process->ack_count = 0;
    process->houses_visited_count = 0;
    init_queue(&process->house_queue);
    init_queue(&process->fence_queue);
}

void update_clock_upon_recv(Process* process, int received_lamport_clock) {
    process->lamport_clock = max(process->lamport_clock, received_lamport_clock) + 1;
};

void increment_clock_by_one(Process* process) {
    process->lamport_clock++;
}


