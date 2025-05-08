//
// Created by hkere on 26/04/2025.
//

#ifndef UTILS_H
#define UTILS_H
#include "process.h"

extern int lamport_clock;

void increment_clock(int* lamport_clock);
void update_clock(int received_time);
int select_house(Process process);

#endif //UTILS_H
