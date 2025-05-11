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
#include <stdarg.h>

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
#include "communication.h"

int select_house(Process* process, int num_houses);
int max(int a, int b);
const char* state_to_string(ProcessState state);
const char* msg_type_to_string(MessageType type);
void custom_printf(FILE* out, const char* fmt, ...);

#endif //UTILS_H
