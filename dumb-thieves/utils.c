#include "utils.h"

int lamport_clock = 0;

void increment_clock() {
    lamport_clock++;
}

void update_clock(int received_time) {
    if (lamport_clock < received_time) {
        lamport_clock = received_time;
    }
    lamport_clock++;
}

int select_house(Process* process, int num_houses) {
    int house_ID = ((process->rank * (process->houses_visited_count + 1)) % num_houses + 1);
    return house_ID;
}