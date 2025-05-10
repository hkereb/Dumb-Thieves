#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>

#include "queue.h"

typedef enum {
    RESTING,
    WAITING_FOR_HOUSE,
    ROBBING_HOUSE,
    WAITING_FOR_FENCE,
    HAS_FENCE
} ProcessState;

typedef struct Process {
    int rank;
    ProcessState state;
    int lamport_clock;
    int last_req_clock;
    int house_ID;
    int ack_count;
    int houses_visited_count;
    Queue house_queue;
    Queue fence_queue;

    FILE* log_file;
} Process;

void init_process(Process* process, int rank);
void update_lamport_clock(Process* process, int received_lamport_clock);
void leave_critical_sections(Process* process);

#endif
