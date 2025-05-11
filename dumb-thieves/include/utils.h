//
// Created by hkere on 26/04/2025.
//

#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <mpi.h>
#include <stdio.h>

#include "process.h"
#include "communication.h"

int select_house(Process* process, int num_houses);
int max(int a, int b);
const char* state_to_string(ProcessState state);
const char* msg_type_to_string(MessageType type);

#endif //UTILS_H
