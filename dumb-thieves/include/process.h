#ifndef PROCESS_H
#define PROCESS_H

#include "queue.h"

typedef struct Process {
    int rank;
    int state;
    int lamport_clock;
    int house_ID;
    int ack_count;
    int houses_visited_count;
    
    Queue house_queue;
    Queue paser_queue;

    int processes_count;
    int parsers_count;
    int houses_count;
} Process;

void init_process(Process* process, int rank, int total_processes);
void update_lamport_clock(Process* process, int received_lamport_clock);

#endif
