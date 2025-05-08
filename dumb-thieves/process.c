#include <stdio.h>
#include <unistd.h>

#include "process.h"


void init_process(Process* process, const int rank, int process_number, int fence_number, int house_number) {
    process->rank = rank;
    process->state = RESTING;
    process->lamport_clock = 0;
    process->house_ID = -1;
    process->ack_count = 0;
    process->houses_visited_count = 0;

    init_queue(&process->house_queue);
    init_queue(&process->fence_queue);

    process->process_number = process_number;
    process->fence_number = 0;
    process->house_number = 0;
};

void update_lamport_clock(Process* process, int received_lamport_clock) {
    process->lamport_clock++;

    if (process->lamport_clock < received_lamport_clock) {
        process->lamport_clock = received_lamport_clock;
    }
};




