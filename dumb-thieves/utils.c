#include "utils.h"

void increment_clock(Process* process) {
    process->lamport_clock++;
}

void update_clock(Process* process, int received_time) {
    if (process->lamport_clock < received_time) {
        process->lamport_clock = received_time;
    }
    process->lamport_clock++;
}

int select_house(Process* process, int num_houses) {
    return  ((process->rank * (process->houses_visited_count + 1)) % num_houses + 1);
}

int max(int a, int b) {
    if (a > b) return a;

    return b;
}
