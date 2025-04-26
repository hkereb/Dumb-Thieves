//
// Created by hkere on 26/04/2025.
//

#ifndef UTILS_H
#define UTILS_H

extern int lamport_clock;

void increment_clock();
void update_clock(int received_time);

#endif //UTILS_H
