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

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#define mkdir(dir, mode) _mkdir(dir)
#define access _access
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include "process.h"

void increment_clock(Process* process);
void update_clock(Process* process, int received_time);
int select_house(Process* process, int num_houses);

#endif //UTILS_H
