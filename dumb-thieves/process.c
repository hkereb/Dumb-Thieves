#include "utils.h"
#include "process.h"

void init_process(Process* process, int rank) {
    process->rank = rank;
    process->state = 0;
    process->lamport_clock = 0;
    process->house_ID = -1;
    process->ack_count = 0;
    process->houses_visited_count = 0;
    init_queue(&process->house_queue);
    init_queue(&process->paser_queue);
}

void update_lamport_clock(Process* process, int received_lamport_clock) {
    process->lamport_clock++;

    if (process->lamport_clock < received_lamport_clock) {
        process->lamport_clock = received_lamport_clock;
    }
};




