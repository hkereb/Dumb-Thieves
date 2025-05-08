#include "utils.h"

#include <stdio.h>

int lamport_clock = 0;

void increment_clock(int* lamport_clock) {
    (*lamport_clock)++;
}

void update_clock(int received_time) {
    if (lamport_clock < received_time) {
        lamport_clock = received_time;
    }
    lamport_clock++;
}

int select_house(Process process) {
    printf("[DEBUG] rank = %d, houses_visited = %d, rank = house_number = %d\n", process.rank, process.houses_visited_count, process.house_number);
    if (process.house_number == 0) return -1;
    return ((process.rank * (process.houses_visited_count + 1)) % process.house_number + 1);
}