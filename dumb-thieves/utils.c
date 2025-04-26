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