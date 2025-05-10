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

// Whenever a message is sent from one process to another, the message
// includes the time at which the message is sent from the sending
// process. The receiving process extracts the time stamp from the
// message, checks its own time, and if its own time is lower than
// the timestamp it updates its own time to that of the timestamp plus
// one. todo adjust the method?

// void update_lamport_clock(Process* process, int received_lamport_clock) {
//     process->lamport_clock++;
//
//     if (process->lamport_clock < received_lamport_clock) {
//         process->lamport_clock = received_lamport_clock;
//     }
// };
void update_lamport_clock(Process* process, int received_lamport_clock) {
    process->lamport_clock = max(process->lamport_clock, received_lamport_clock) + 1;
};




